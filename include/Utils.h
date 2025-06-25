#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace utils {
std::string readFile(const fs::path &path);
std::string normalizeRawModuleName(const std::string &raw);
} // namespace utils
