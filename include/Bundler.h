#pragma once

#include "CacheManager.h"
#include "ThreadPool.h"
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

class Bundler {
private:
  fs::path entryPath;
  fs::path baseDir;

  std::unordered_map<std::string, std::string> modules;

  std::unique_ptr<CacheManager> cacheManager;
  std::unique_ptr<ThreadPool> threadPool;

  std::string entryModuleName;

  bool _moduleProcessed(const std::string &moduleName) const {
    return modules.count(moduleName);
  }

public:
  Bundler(const fs::path &entryFile, size_t threadCount = 4);

  void bundle();

  const std::unordered_map<std::string, std::string> &getModules() const {
    return modules;
  }

  const std::string &getEntryModuleName() const { return entryModuleName; }
};
