#include "LuauBundler.h"

#include <iostream>
#include <sstream>
#include <string>

std::string removeComments(const std::string &content) {
  std::string result;
  size_t len = content.size();
  size_t i = 0;

  bool in_long_comment = false;
  int long_comment_eq_count = 0;

  while (i < len) {
    if (!in_long_comment && i + 1 < len && content[i] == '-' &&
        content[i + 1] == '-') {

      size_t j = i + 2;
      int eq_count = 0;
      if (j < len && content[j] == '[') {
        j++;
        while (j < len && content[j] == '=') {
          eq_count++;
          j++;
        }
        if (j < len && content[j] == '[') {
          in_long_comment = true;
          long_comment_eq_count = eq_count;
          i = j + 1;
          continue;
        }
      }

      while (i < len && content[i] != '\n') {
        if (content[i] == '\r')
          result += '\r';
        else
          result += ' ';
        i++;
      }

      if (i < len && content[i] == '\n') {
        result += '\n';
        i++;
      }
      continue;
    }

    if (in_long_comment) {
      if (content[i] == ']') {
        size_t j = i + 1;
        int eq_count = 0;
        while (j < len && content[j] == '=')
          eq_count++, j++;
        if (j < len && content[j] == ']' && eq_count == long_comment_eq_count) {
          i = j + 1;
          in_long_comment = false;
          continue;
        }
      }

      if (content[i] == '\n')
        result += '\n';
      else if (content[i] == '\r')
        result += '\r';
      else
        result += ' ';
      i++;
      continue;
    }

    result += content[i];
    i++;
  }

  return result;
}

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
        "  local fn, err = __modules[mod]\n"
        "  if not fn then error(\"module '\"..mod..\"' not found\", 2) end\n"
        "  local env = setmetatable({require = require}, {__index = "
        "getfenv()})\n"
        "  setfenv(fn, env)\n"
        "  return fn()\n"
        "end\n\n";

  ss << "local l = loadstring\n";
  for (const auto &[key, content] : modules) {
    std::string chunkName = "@" + key;
    for (auto &c : chunkName)
      if (c == '\\')
        c = '/';

    ss << "__modules[\"" << key << "\"] = l([=[" << removeComments(content) << "]=], \""
       << chunkName << "\")\n";
  }

  ss << "\nrequire(\"" << entryModule << "\")\n";
  return ss.str();
}
