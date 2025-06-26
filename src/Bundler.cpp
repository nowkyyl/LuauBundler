#include "Bundler.h"

#include <future>
#include <iostream>
#include <regex>

#include "CacheManager.h"
#include "ThreadPool.h"
#include "Utils.h"

Bundler::Bundler(const fs::path &entryFile, size_t threadCount)
    : entryPath(entryFile), baseDir(entryFile.parent_path()) {
  cacheManager = std::make_unique<CacheManager>(baseDir);
  cacheManager->load();
  threadPool = std::make_unique<ThreadPool>(threadCount);
}

static fs::path resolveRequirePath(const std::string &raw,
                                   const fs::path &filePath,
                                   const fs::path &baseDir) {
  fs::path root = (raw.compare(0, 6, "@self/") == 0)
                      ? filePath.parent_path() / raw.substr(6)
                  : (raw[0] == '.' || raw.compare(0, 2, "..") == 0)
                      ? filePath.parent_path() / raw
                      : baseDir / raw;

  root = fs::weakly_canonical(root);

  const std::vector<fs::path> candidates{root, root.string() + ".lua",
                                         root.string() + ".luau",
                                         root / "init.lua", root / "init.luau"};

  for (const auto &candidate : candidates)
    if (fs::exists(candidate) && fs::is_regular_file(candidate))
      return fs::weakly_canonical(candidate);

  std::cerr << "Failed to resolve require path:\n"
            << "  raw:      " << raw << "\n"
            << "  from:     " << filePath << "\n"
            << "  attempted:\n";

  for (const auto &candidate : candidates)
    std::cerr << "    - " << candidate << "\n";

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

          while (std::smatch match;
                 std::regex_search(searchStart, content.cend(), match, re);) {
            result.append(searchStart, match[0].first);

            fs::path resolved =
                resolveRequirePath(match[1].str(), filePath, baseDir);
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
