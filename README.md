# Raining Sharks

> The game is work in progress.

## Description

This is a very little game in C made from scratch (execpt OpenGL and GLFW) for learning purpose.

## Build and run with CMake (Linux only)

- Requires C++11.
- Requires `pragma once` support.
- Requires CMake 2.8.2 or more (tested with 3.10).
- Use `-DENGINE_ENABLE_LOG` to activate logs (default in Debug and Release)
- Use `-DENGINE_ENABLE_ASSERT` to activate asserts (default in Debug)

```bash
mkdir build
cd build
cmake ..
make
make rungame

# Or use the build script
./build.sh
```
