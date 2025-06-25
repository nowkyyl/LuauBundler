cmake_minimum_required(VERSION 3.10)
project(LuauBundler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(DEFAULT_THREAD_COUNT 4 CACHE STRING "Default number of threads")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Default thread count: ${DEFAULT_THREAD_COUNT}")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
project(LuauBundler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(LuauBundler
    src/main.cpp
    src/Bundler.cpp
    src/LuauBundler.cpp
    src/Utils.cpp
    src/ThreadPool.cpp
    src/CacheManager.cpp
)

add_executable(LuauBundler ${SOURCES})

if(MSVC)
    target_compile_options(LuauBundler PRIVATE /W4 /permissive-)
else()
    target_compile_options(LuauBundler PRIVATE -Wall -Wextra -Wpedantic -Werror)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(LuauBundler PRIVATE -O3)
    endif()
endif()

target_compile_definitions(LuauBundler PRIVATE DEFAULT_THREAD_COUNT=${DEFAULT_THREAD_COUNT})

if(WIN32)
    message(STATUS "Configuring for Windows")
elseif(UNIX)
    message(STATUS "Configuring for Unix/Linux")
endif()
)
