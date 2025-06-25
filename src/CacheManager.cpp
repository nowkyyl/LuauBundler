#include "CacheManager.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "picosha2.h"

CacheManager::CacheManager(const fs::path &base)
{
    cachePath = base / ".luacache";
}

std::string CacheManager::sha256(const std::string &content) const
{
    return picosha2::hash256_hex_string(content);
}

bool CacheManager::load()
{
    if (!fs::exists(cachePath))
        return false;

    std::ifstream in(cachePath);
    if (!in.is_open())
        return false;

    std::string name, hash;
    while (std::getline(in, name) && std::getline(in, hash))
    {
        cacheContent[name] = hash;
    }

    return true;
}

void CacheManager::save()
{
    std::ofstream out(cachePath);
    for (const auto &[name, hash] : cacheContent)
    {
        out << name << "\n"
            << hash << "\n";
    }
}

bool CacheManager::isCached(const std::string &moduleName,
                            const std::string &contentHash)
{
    auto it = cacheContent.find(moduleName);
    return it != cacheContent.end() && it->second == contentHash;
}

void CacheManager::store(const std::string &moduleName,
                         const std::string &content)
{
    cacheContent[moduleName] = sha256(content);
}

const std::unordered_map<std::string, std::string> &CacheManager::getCache() const
{
    return cacheContent;
}
