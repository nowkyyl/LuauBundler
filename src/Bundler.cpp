#include "Bundler.h"
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
}
