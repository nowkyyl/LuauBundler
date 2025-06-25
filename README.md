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

Importing Modules with `require`

### LuauBundler uses a system similar to JavaScript:

- Use **relative** paths in `require`, with `./` and `/`
- To import a folder, use a path ending with `/`, which must contain an `init.lua` file
- To import files, use the full relative path, with or without the `.lua` extension

Examples:
```lua
local utils = require("./utils")              -- imports utils/init.lua
local math = require("./utils/math.lua")      -- imports utils/math.lua
local services = require("./services")        -- imports services/init.lua
local db = require("./services/database.lua") -- imports services/database.lua
```

### Example

`init.lua`:

```lua
local utils = require("./utils/")
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

In the terminal, run:
```bash
./LuauBundler.exe project/init.lua output.lua 4
```

* `project/init.lua`: main file of your project (input).
* `output.lua`: file that will receive the bundled code (output).
* `4`: processing or optimization level (how much the bundler will "bundle").

### Summary

* Use `require` with relative paths like JS (`./folder/`, `./file.lua`)
* Folders must have `init.lua` to be imported with `/`
