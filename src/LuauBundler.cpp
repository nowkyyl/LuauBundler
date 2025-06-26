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
        "local oldRequire = require\n\n"
        "function require(mod)\n"
        "  if type(mod) == 'function' then return mod() end\n"
        "  if type(mod) ~= 'string' then return oldRequire(mod) end\n"
        "  local src = __modules[mod]\n"
        "  if not src then error(\"module '\"..mod..\"' not found\", 2) end\n"
        "  local fn, err = loadstring(src, '@'..mod)\n"
        "  if not fn then error(err, 2) end\n"
        "  local env = setmetatable({require = require}, {__index = "
        "getfenv()})\n"
        "  setfenv(fn, env)\n"
        "  return fn()\n"
        "end\n\n";

  for (const auto &[key, content] : modules)
    ss << "__modules[\"" << key << "\"] = [=[" << content << "]=]\n";

  ss << "\nrequire(\"" << entryModule << "\")\n";
  return ss.str();
}
