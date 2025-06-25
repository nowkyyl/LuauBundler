```markdown
## Estrutura do projeto
```
project/
├── init.lua
├── utils/
│   ├── init.lua
│   └── math.lua
├── services/
│   ├── init.lua
│   └── database.lua
````

## Importando módulos com `require`

O LuauBundler usa um sistema de importação parecido com JavaScript:

- Use caminhos **relativos** no `require`, com `./` e `/`
- Para importar uma pasta, use o caminho com `/` no final, que deve ter `init.lua`
- Para importar arquivos, use o caminho relativo completo, pode ou não incluir `.lua`

Exemplos:

```lua
local utils = require("./utils")             -- importa utils/init.lua
local math = require("./utils/math.lua")      -- importa utils/math.lua
local services = require("./services")       -- importa services/init.lua
local db = require("./services/database.lua") -- importa services/database.lua
````

## Exemplo prático

`init.lua`:

```lua
local utils = require("./utils/")
print(utils.math.add(2, 3))
```

`utils/init.lua`:

```lua
local M = {}

M.math = require("./math.lua")

return M
```

`utils/math.lua`:

```lua
local M = {}

function M.add(a, b)
    return a + b
end

return M
```

## Empacotando seu projeto

No terminal, rode:

```bash
LuauBundler src/init.lua output.lua 4
```

Esse comando cria o arquivo `output.lua` com todo seu projeto empacotado.

## Resumo

* Use `require` com caminhos relativos estilo JS (`./pasta/`, `./arquivo.lua`)
* Pastas devem ter `init.lua` para serem importadas com `/`
