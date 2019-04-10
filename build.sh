#!/bin/bash


TARGET_EXE=raining-sharks.out
SRC_FILES=$(find ./src/ -regextype sed -regex ".*\.c")


function exec_and_print() {
    echo -e "\e[34m${1}\e[0m"
    #$("$1")
    `$1`
}

cmd="rm $TARGET_EXE >> /dev/null 2>&1"
exec_and_print "$cmd"

cmd="gcc -g -Wall
    -I./extern/glfw/include
    -I./extern/glew/include
    -I./extern/stb/include
    -I./src
    -lX11 -lXrandr -lXinerama -lXxf86vm -lXcursor
    -lrt -lm -ldl -lGL
    -lpthread
    ${SRC_FILES}
    ./extern/glfw/bin/libglfw3.a
    ./extern/glew/bin/libGLEW.a
    -o $TARGET_EXE
    "
exec_and_print "$cmd"

