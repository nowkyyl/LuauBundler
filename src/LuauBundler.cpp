#include "LuauBundler.h"
#include <sstream>
#include <iostream>
#include <string>

LuauBundler::LuauBundler(const std::unordered_map<std::string, std::string> &modules_, const std::string &entry)
    : modules(modules_), entryModule(entry) {}

std::string LuauBundler::generate() const
{
    std::stringstream ss;
    ss << R"(
local __modules = {}
function require(module)
    if type(module) == "string" then
        local f = __modules[module]
        if not f then
            error("module '" .. module .. "' not found")
        end
        return f()
    else
        return require(module)
    end
end

)";

    for (const auto &[name, content] : modules)
    {
        ss << "__modules[\"" << name << "\"] = function()\n";

        std::istringstream stream(content);
        std::string line;
        bool hasReturn = false;
        bool hasNonEmptyLine = false;

        std::stringstream body;

        while (std::getline(stream, line))
        {
            std::string trimmed = line;
            trimmed.erase(0, trimmed.find_first_not_of(" \t"));

            if (trimmed.rfind("return", 0) == 0)
            {
                hasReturn = true;
            }

            if (!trimmed.empty())
            {
                hasNonEmptyLine = true;
            }

            body << "    " << line << "\n";
        }

        if (!hasReturn && hasNonEmptyLine)
        {
            body << "    return true\n";
        }

        ss << body.str();
        ss << "end\n\n";
    }

    ss << "require(\"" << entryModule << "\")\n";
    return ss.str();
}
