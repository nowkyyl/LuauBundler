#include "Utils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
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

    std::string normalizeRawModuleName(const std::string &raw)
    {
        if (raw.rfind("./", 0) == 0)
            return raw.substr(2);
        return raw;
    }
} // namespace utils
