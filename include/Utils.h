#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace utils
{
    std::string readFile(const fs::path &path);
<<<<<<< HEAD
    std::string normalizeRawModuleName(const std::string &raw);
=======
    std::string normalizeModuleName(const fs::path &basePath, const fs::path &modulePath);
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c
} // namespace utils
