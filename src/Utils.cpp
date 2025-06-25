#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace utils
{
    std::string readFile(const fs::path &path)
    {
        std::ifstream file(path, std::ios::in);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + path.string());
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::string normalizeModuleName(const fs::path &basePath, const fs::path &modulePath)
    {
        fs::path relative = fs::relative(modulePath, basePath);
        return relative.generic_string();
    }
} // namespace utils