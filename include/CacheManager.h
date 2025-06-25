#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

class CacheManager {
private:
  fs::path cachePath;
  std::unordered_map<std::string, std::string> cacheContent;

public:
  explicit CacheManager(const fs::path &base);

  bool load();
  void save();

  bool isCached(const std::string &moduleName, const std::string &contentHash);
  void store(const std::string &moduleName, const std::string &content);

  const std::unordered_map<std::string, std::string> &getCache() const;
  std::string sha256(const std::string &content) const;
};
