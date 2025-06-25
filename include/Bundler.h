#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

class Bundler
{
private:
    void processModule(const fs::path &filePath, const std::string &moduleName);
    void processModule(const fs::path &filePath);

    fs::path entryPath;
    fs::path baseDir;

    std::unordered_map<std::string, std::string> modules;

    bool _moduleProcessed(const std::string &moduleName) const;

public:
    Bundler(const fs::path &entryFile);

    void bundle();

    const std::unordered_map<std::string, std::string> &getModules() const;
};
