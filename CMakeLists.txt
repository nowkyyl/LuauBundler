cmake_minimum_required(VERSION 3.10)
project(LuauBundler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(LuauBundler
    src/main.cpp
    src/Bundler.cpp
    src/LuauBundler.cpp
    src/Utils.cpp
)