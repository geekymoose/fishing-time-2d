# Fishing time 2D

> The game is work in progress.

## Description

This is a little game in C for the learning purpose (made with OpenGL and GLFW).
It focuses on the engine side.

## Build and run with CMake (Linux only)

- Requires C++11.
- Requires `pragma once` support.
- Requires CMake 2.8.2 or more (tested with 3.10).
- Requires Jam build system to compile FreeType (<https://www.freetype.org/jam/index.html>)
- Use `-DENGINE_ENABLE_LOG` to activate logs (by default in Debug and Release)
- Use `-DENGINE_ENABLE_ASSERT` to activate asserts (by default in Debug)

```bash
mkdir build
cd build
cmake ..
make
make rungame

# Or use the build script
./build.sh
```
