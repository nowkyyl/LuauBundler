#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
<<<<<<< HEAD
#include <memory>
#include "CacheManager.h"
#include "ThreadPool.h"
=======
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c

namespace fs = std::filesystem;

class Bundler
{
private:
<<<<<<< HEAD
=======
    void processModule(const fs::path &filePath, const std::string &moduleName);
    void processModule(const fs::path &filePath);

>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c
    fs::path entryPath;
    fs::path baseDir;

    std::unordered_map<std::string, std::string> modules;

<<<<<<< HEAD
    std::unique_ptr<CacheManager> cacheManager;
    std::unique_ptr<ThreadPool> threadPool;

    std::string entryModuleName;

    bool _moduleProcessed(const std::string &moduleName) const
    {
        return modules.count(moduleName);
    };

public:
    Bundler(const fs::path &entryFile, size_t threadCount = 4);

    void bundle();

    const std::unordered_map<std::string, std::string> &getModules() const
    {
        return modules;
    }

    const std::string &getEntryModuleName() const
    {
        return entryModuleName;
    };
=======
    bool _moduleProcessed(const std::string &moduleName) const;

public:
    Bundler(const fs::path &entryFile);

    void bundle();

    const std::unordered_map<std::string, std::string> &getModules() const;
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c
};
