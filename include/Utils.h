#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace utils
{
    std::string readFile(const fs::path &path);
    std::string normalizeRawModuleName(const std::string &raw);
} // namespace utils
