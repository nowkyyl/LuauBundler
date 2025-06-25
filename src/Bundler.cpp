#include "Bundler.h"
<<<<<<< HEAD

#include <future>
#include <iostream>
#include <regex>

#include "CacheManager.h"
#include "ThreadPool.h"
#include "Utils.h"

Bundler::Bundler(const fs::path &entryFile, size_t threadCount)
    : entryPath(entryFile),
      baseDir(entryFile.parent_path())
{
    cacheManager = std::make_unique<CacheManager>(baseDir);
    cacheManager->load();

    threadPool = std::make_unique<ThreadPool>(threadCount);
}

void Bundler::bundle()
{
    std::function<void(const fs::path &, const std::string &)> process;

    process = [&](const fs::path &filePath, const std::string &rawName)
    {
        std::string key = utils::normalizeRawModuleName(rawName);
        if (_moduleProcessed(key))
            return;

        std::string content = utils::readFile(filePath);
        std::string hash = cacheManager->sha256(content);

        if (!cacheManager->isCached(key, hash))
        {
            std::regex re(R"(\brequire\s*\(\s*["']([^"']+)["']\))");
            std::smatch match;
            std::string result;
            auto searchStart = content.cbegin();

            while (std::regex_search(searchStart, content.cend(), match, re))
            {
                result.append(searchStart, match[0].first);

                std::string raw = match[1].str();
                if (raw.rfind("./", 0) == 0)
                    raw.erase(0, 2);

                std::string rawSystemPath = raw;
                for (char &c : rawSystemPath)
                    if (c == '/')
                        c = '\\';

                fs::path fullChildPath = filePath.parent_path() / fs::path(rawSystemPath);
                fullChildPath = fullChildPath.lexically_normal();

                if (fs::is_directory(fullChildPath))
                    fullChildPath /= "init.lua";

                if (!fs::exists(fullChildPath))
                    throw std::runtime_error("Can't resolve: " + raw);

                std::string relativeModule = utils::normalizeRawModuleName(
                    fs::relative(fullChildPath, baseDir).generic_string());

                result.append("require(\"" + relativeModule + "\")");

                process(fullChildPath, relativeModule);
                searchStart = match.suffix().first;
            }

            result.append(searchStart, content.cend());

            modules[key] = result;
            cacheManager->store(key, result);
        }
        else
        {
            modules[key] = content;
        }
    };

    entryModuleName = utils::normalizeRawModuleName(
        fs::relative(entryPath, baseDir).generic_string());

    process(entryPath, entryModuleName);
    cacheManager->save();
=======
#include "Utils.h"
#include <regex>
#include <iostream>

Bundler::Bundler(const fs::path &entryFile)
    : entryPath(entryFile), baseDir(entryFile.parent_path()) {}

void Bundler::bundle()
{
    processModule(entryPath, entryPath.filename().string());
}

bool Bundler::_moduleProcessed(const std::string &moduleName) const
{
    return modules.find(moduleName) != modules.end();
}

void Bundler::processModule(const fs::path &filePath)
{
    std::string inferred = filePath.filename().string();
    processModule(filePath, inferred);
}

void Bundler::processModule(const fs::path &filePath, const std::string &moduleName)
{
    if (_moduleProcessed(moduleName))
        return;

    std::string content = utils::readFile(filePath);
    modules[moduleName] = content;

    std::regex require_regex(R"(require\s*\(\s*["']([^"']+)["']\s*\))");
    auto begin = content.cbegin();
    auto end = content.cend();
    std::smatch match;

    while (std::regex_search(begin, end, match, require_regex))
    {
        std::string requiredName = match[1];
        fs::path resolvedPath;

        fs::path modulePath = baseDir / requiredName;
        if (modulePath.has_extension())
        {
            resolvedPath = modulePath;
        }
        else
        {
            fs::path initPath = baseDir / requiredName / "init.lua";
            if (fs::exists(initPath))
            {
                resolvedPath = initPath;
            }
            else
            {
                fs::path direct = baseDir / (requiredName + ".lua");
                if (fs::exists(direct))
                {
                    resolvedPath = direct;
                }
                else
                {
                    throw std::runtime_error("Failed to resolve module: " + requiredName);
                }
            }
        }

        // Recurse using the exact name from require("...") as the module key
        processModule(resolvedPath, requiredName);

        begin = match.suffix().first;
    }
}

const std::unordered_map<std::string, std::string> &Bundler::getModules() const
{
    return modules;
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c
}
