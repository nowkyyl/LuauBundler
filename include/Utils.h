#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace utils
{
    std::string readFile(const fs::path &path);
    std::string normalizeModuleName(const fs::path &basePath, const fs::path &modulePath);
} // namespace utils
