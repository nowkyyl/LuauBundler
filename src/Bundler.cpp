#include "Bundler.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

#include <nlohmann/json.hpp>

#include "CacheManager.h"
#include "ThreadPool.h"
#include "Utils.h"

namespace fs = std::filesystem;

static fs::path findLuarc(fs::path start) {
  for (auto dir = start; !dir.empty(); dir = dir.parent_path()) {
    fs::path candidate = dir / ".luaurc";
    if (fs::exists(candidate))
      return candidate;
    if (dir == dir.root_path())
      break;
  }
  return {};
}

bool starts_with(const std::string &str, const std::string &prefix) {
  return str.size() >= prefix.size() &&
         std::equal(prefix.begin(), prefix.end(), str.begin());
}

std::optional<fs::path> tryFindFile(const fs::path &basePath) {
  std::vector<fs::path> candidates{
      fs::path(basePath).replace_extension(".lua"),
      fs::path(basePath).replace_extension(".luau"), basePath / "init.lua",
      basePath / "init.luau", basePath};

  for (const auto &candidate : candidates)
    if (fs::exists(candidate) && fs::is_regular_file(candidate))
      return fs::weakly_canonical(candidate);
  return std::nullopt;
}

fs::path findExistingFile(const fs::path &basePath) {
  if (auto found = tryFindFile(basePath))
    return *found;
  throw std::runtime_error("Can't resolve path: " + basePath.string());
}

Bundler::Bundler(const fs::path &entryFile, size_t threadCount)
    : entryPath(entryFile) {
  fs::path luarcPath = findLuarc(entryFile.parent_path());
  baseDir =
      !luarcPath.empty() ? luarcPath.parent_path() : entryFile.parent_path();
  baseDir = fs::weakly_canonical(baseDir);
  cacheManager = std::make_unique<CacheManager>(baseDir);
  cacheManager->load();
  threadPool = std::make_unique<ThreadPool>(threadCount);
  loadAliasesFromLuarc(luarcPath);
}

void Bundler::loadAliasesFromLuarc(const fs::path &luarcPath) {
  if (luarcPath.empty() || !fs::exists(luarcPath))
    return;
  std::ifstream inFile(luarcPath);
  if (!inFile.is_open())
    return;

  try {
    nlohmann::json config;
    inFile >> config;
    if (config.contains("aliases") && config["aliases"].is_object()) {
      for (auto &[alias, pathJson] : config["aliases"].items()) {
        aliases[alias] =
            fs::weakly_canonical(baseDir / pathJson.get<std::string>());
      }
    }
  } catch (...) {
  }
}

fs::path Bundler::resolveRequirePath(const std::string &raw,
                                     const fs::path &filePath) {
  if (raw.rfind("@self/", 0) == 0)
    return findExistingFile(filePath.parent_path() / raw.substr(6));

  if (raw.rfind("@", 0) == 0) {
    auto rest = raw.substr(1);
    auto slash = rest.find('/');
    std::string alias =
        (slash != std::string::npos) ? rest.substr(0, slash) : rest;
    std::string suffix =
        (slash != std::string::npos) ? rest.substr(slash + 1) : "";

    if (aliases.find(alias) != aliases.end())
      return findExistingFile(aliases[alias] / suffix);
    throw std::runtime_error("Unknown alias: @" + alias);
  }

  fs::path rawPath(raw);

  if (rawPath.is_relative() &&
      (starts_with(raw, "./") || starts_with(raw, "../"))) {
    fs::path currentDir = filePath.parent_path();
    while (true) {
      fs::path tryPath = currentDir / rawPath;
      if (auto found = tryFindFile(tryPath))
        return *found;
      if (currentDir == baseDir || currentDir == currentDir.root_path())
        break;
      currentDir = currentDir.parent_path();
    }
    fs::path fallback = baseDir / rawPath;
    if (auto found = tryFindFile(fallback))
      return *found;

    throw std::runtime_error("Can't resolve: " + raw);
  }

  fs::path root = baseDir / rawPath;
  if (auto found = tryFindFile(root))
    return *found;

  throw std::runtime_error("Can't resolve: " + raw);
}

void Bundler::bundle() {
  std::function<void(const fs::path &, const std::string &)> process =
      [&](const fs::path &filePath, const std::string &rawName) {
        std::string key = utils::normalizeRawModuleName(rawName);
        if (_moduleProcessed(key))
          return;

        std::string content = utils::readFile(filePath);
        std::string hash = cacheManager->sha256(content);

        if (!cacheManager->isCached(key, hash)) {
          std::regex re(R"(\brequire\s*\(\s*["']([^"']+)["']\))");
          std::string result;
          auto searchStart = content.cbegin();
          std::smatch match;

          while (std::regex_search(searchStart, content.cend(), match, re)) {
            result.append(searchStart, match[0].first);

            fs::path resolved = resolveRequirePath(match[1].str(), filePath);
            std::string relativeModule = utils::normalizeRawModuleName(
                fs::relative(resolved, baseDir).generic_string());

            result.append("require(\"" + relativeModule + "\")");
            process(resolved, relativeModule);
            searchStart = match.suffix().first;
          }
          result.append(searchStart, content.cend());
          modules[key] = result;
          cacheManager->store(key, result);
        } else {
          modules[key] = content;
        }
      };

  entryModuleName = utils::normalizeRawModuleName(
      fs::relative(entryPath, baseDir).generic_string());
  process(entryPath, entryModuleName);
  cacheManager->save();
}
