cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)
project(fishing-time-2d)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "in-source build not allowed. Use another directory.")
endif()

# Compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
message(STATUS "Detected Operating System: ${CMAKE_SYSTEM_NAME}")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type set. Use Release")
    message(STATUS "CMAKE_BUILD_TYPE: Debug / Release")
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(extern)
add_subdirectory(src)
