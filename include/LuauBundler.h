#pragma once

#include <unordered_map>
#include <string>

class LuauBundler
{
private:
    const std::unordered_map<std::string, std::string> &modules;
    std::string entryModule;

public:
    LuauBundler(const std::unordered_map<std::string, std::string> &modules, const std::string &entry);

    std::string generate() const;
}; // namespace LuauBundler