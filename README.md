
# How To Use

### Project Structure
```
project/
├── init.lua
├── utils/
│   ├── init.lua
│   └── math.lua
├── services/
│   ├── init.lua
│   └── database.lua
```

### Importing Modules with `require`

- Use **relative** paths in `require` with `./` and `../`
- To import a folder, use a path pointing to the folder (ending with `/` or just the folder name), which must contain an `init.lua` file
- To import files, you can specify the full relative path, with or without the `.lua` or `.luau` extension
- You can also use the alias `@self/` to require modules relative to the current script’s folder
- The bundler automatically resolves the require path trying, in this order:
  1. Exact path as given
  2. Path with `.lua` extension
  3. Path with `.luau` extension
  4. Folder path with `init.lua`
  5. Folder path with `init.luau`

### Using `.luaurc` and Aliases

You can create a `.luaurc` JSON configuration file in your project root (or any parent folder of your entry script) to define custom aliases for your modules.

Example `.luaurc`:

```json
{
  "aliases": {
    "utils": "utils",
    "pkgs": "libs/packages"
  }
}
```

- Aliases let you import modules using `@alias/` prefix.  
- For example, `require("@utils/math")` resolves to the `math.lua` module inside the `utils` folder defined in the alias.  
- The bundler loads `.luaurc` automatically from the closest parent directory of your entry file.

### Examples

```lua
local utils = require("./utils")                -- imports utils/init.lua
local math = require("@utils/math")              -- imports utils/math.lua via alias
local services = require("./services")          -- imports services/init.lua
local db = require("./services/database.lua")   -- imports services/database.lua
local nums = require("@self/nums")               -- imports nums/init.lua or nums.lua relative to current script
```

### Example Project Files

`init.lua`:

```lua
local utils = require("@utils")
print(utils.math.add(2, 3))
```

`utils/init.lua`:

```lua
return {
    math = require("./math.lua")
}
```

`utils/math.lua`:

```lua
local Math = {}

function Math.add(a, b)
    return a + b
end

return Math
```

### Bundling Your Project

Run this in terminal:

```bash
./LuauBundler.exe project/init.lua output.lua 4
```

- `project/init.lua`: your main entry Lua file (input)
- `output.lua`: output file where the bundled code will be saved
- `4`: level of threading or optimization (number of threads)

### Summary

- Use `require` with relative paths (`./folder/`, `../file`, etc)
- Use `@self/` as alias for "module relative to current script"
- Use aliases defined in `.luaurc` with `@alias/` syntax
- Folders must have `init.lua` or `init.luau` to be required as a module
- The bundler automatically adds `.lua` or `.luau` extensions if missing, or looks for `init` files inside folders
