#include "LuauBundler.h"

#include <iostream>
#include <sstream>
#include <string>

LuauBundler::LuauBundler(
    const std::unordered_map<std::string, std::string> &modules_,
    const std::string &entry)
    : modules(modules_), entryModule(entry) {}

std::string LuauBundler::generate() const {
  std::stringstream ss;

  ss << "local __modules = {}\n"
        "function require(module)\n"
        "  if type(module) == 'string' then\n"
        "     local f = __modules[module]\n"
        "     if not f then error(\"module '\"..module..\"' not found\") end\n"
        "  else\n"
        "     return require(module)"
        "  end\n"
        "  return f()\n"
        "end\n\n";

  for (const auto &[key, content] : modules) {
    ss << "__modules[\"" << key << "\"] = function()\n";

    std::istringstream in(content);
    std::string line;
    while (std::getline(in, line))
      ss << "  " << line << "\n";

    ss << "end\n\n";
  }

  ss << "require(\"" << entryModule << "\")\n";
  return ss.str();
}
