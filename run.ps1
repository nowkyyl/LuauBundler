cmake -S . -B build
cmake --build build --config Release

.\build\Release\LuauBundler.exe .\test\main.lua output.lua 4