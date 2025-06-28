#include "LuauBundler.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

std::string removeComments(const std::string &content) {
  std::string result;
  size_t len = content.size();
  size_t i = 0;

  bool inLongCommet = false;
  int inLongCommetEqCount = 0;

  while (i < len) {
    if (!inLongCommet && i + 1 < len && content[i] == '-' &&
        content[i + 1] == '-') {

      size_t j = i + 2;
      int eqCount = 0;
      if (j < len && content[j] == '[') {
        j++;
        while (j < len && content[j] == '=') {
          eqCount++;
          j++;
        }
        if (j < len && content[j] == '[') {
          inLongCommet = true;
          inLongCommetEqCount = eqCount;
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

    if (inLongCommet) {
      if (content[i] == ']') {
        size_t j = i + 1;
        int eqCount = 0;
        while (j < len && content[j] == '=')
          eqCount++, j++;
        if (j < len && content[j] == ']' && eqCount == inLongCommetEqCount) {
          i = j + 1;
          inLongCommet = false;
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


std::string minify(const std::string &content) {
    std::string result;
    bool inString = false;
    char stringDelimiter = '\0';

    for (size_t i = 0; i < content.size(); ++i) {
        char c = content[i];

        if (!inString && (c == '\'' || c == '"')) {
            inString = true;
            stringDelimiter = c;
            result += c;
            continue;
        } else if (inString && c == stringDelimiter) {
            size_t backslashes = 0;
            size_t j = i;
            while (j > 0 && content[--j] == '\\') backslashes++;
            if (backslashes % 2 == 0) {
                inString = false;
            }
            result += c;
            continue;
        }

        if (inString) {
            result += c;
            continue;
        }

        if (c == '\n' || c == '\r' || c == '\t') {
            if (!result.empty() && result.back() != ' ')
                result += ' ';
        } else if (std::isspace(static_cast<unsigned char>(c))) {
            if (!result.empty() && result.back() != ' ')
                result += ' ';
        } else {
            result += c;
        }
    }

    while (!result.empty() && result.back() == ' ')
        result.pop_back();

    return result;
}

LuauBundler::LuauBundler(
    const std::unordered_map<std::string, std::string> &modules_,
    const std::string &entry)
    : modules(modules_), entryModule(entry) {}

std::string LuauBundler::generate(int mode) const {
    std::stringstream out;

    std::cout << mode << std::endl;

    out << "local modules = {\n";

    for (const auto& [name, src] : modules) {
        std::string chunk = "@" + name;
        for (char& c : chunk)
            if (c == '\\') c = '/';

        out << "    [\"" << name << "\"] = [=[" << removeComments(src) << "]=],\n";
    }

    out << "}\n\n";

    out <<
        "for i, v in modules do\n"
        "    local f, e = loadstring(v, '@' .. i)\n"
        "    if not f then\n"
        "        warn(\"Failed to compile module '\" .. i .. \"': \" .. tostring(e))\n"
        "    else\n"
        "        modules[i] = f\n"
        "    end\n"
        "end\n\n";

    out <<
        "local cache = {}\n"
        "local oldr = require\n"
        "local function require(m)\n"
        "    if type(m) == 'function' then return m() end\n"
        "    if type(m) ~= 'string' then return oldr(m) end\n"
        "    if cache[m] then return cache[m] end\n\n"
        "    local f = modules[m]\n"
        "    if not f then error(\"module '\" .. m .. \"' not found\", 2) end\n"
        "    setfenv(f, setmetatable({ require = require }, { __index = getfenv(f) }))\n"
        "    local r = f()\n"
        "    cache[m] = r\n"
        "    return r\n"
        "end\n\n";

    out << "require(\"" << entryModule << "\")";

    if (mode == 1) {
        return minify(out.str());
    }
    return out.str();
}
