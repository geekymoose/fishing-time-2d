#!/bin/bash


TARGET_EXE=raining-sharks.out

rm $TARGET_EXE >> /dev/null 2>&1

gcc -g -Wall \
    -I./extern/glfw/include \
    -I./extern/glew/include \
    -lX11 -lXrandr -lXinerama -lXxf86vm -lXcursor \
    -lrt -lm -ldl -lGL \
    -lpthread \
    ./src/main.c \
    ./src/window.c \
    ./src/shader.c \
    ./src/sprite.c \
    ./extern/glfw/bin/libglfw3.a \
    ./extern/glew/bin/libGLEW.a \
    -o $TARGET_EXE


