#include "Utils.h"
#include <fstream>
#include <sstream>
<<<<<<< HEAD
#include <stdexcept>
=======
#include <iostream>
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c

namespace utils
{
    std::string readFile(const fs::path &path)
    {
        std::ifstream file(path, std::ios::in);
        if (!file.is_open())
<<<<<<< HEAD
            throw std::runtime_error("Failed to open file: " + path.string());
=======
        {
            throw std::runtime_error("Failed to open file: " + path.string());
        }
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

<<<<<<< HEAD
    std::string normalizeRawModuleName(const std::string &raw)
    {
        if (raw.rfind("./", 0) == 0)
            return raw.substr(2);
        return raw;
    }
} // namespace utils
=======
    std::string normalizeModuleName(const fs::path &basePath, const fs::path &modulePath)
    {
        fs::path relative = fs::relative(modulePath, basePath);
        return relative.generic_string();
    }
} // namespace utils
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c
