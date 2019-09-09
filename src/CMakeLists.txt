message(STATUS "Building the sources")

# Build config
set(GAME_VERSION_MAJOR 0)
set(GAME_VERSION_MINOR 1)
set(GAME_RESOURCES_DIR "${PROJECT_SOURCE_DIR}/resources/")
set(GAME_SHADERS_DIR "${PROJECT_SOURCE_DIR}/shaders/")

configure_file(
    "build_config.h.in"
    "build_config.h"
    )
include_directories("${PROJECT_BINARY_DIR}/src") # add this specific header

# Game executable
include_directories("${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE src_files *.c)
add_executable(fishing-time-2d ${src_files})
add_custom_target(rungame fishing-time-2d)

# GLFW
include_directories("${PROJECT_SOURCE_DIR}/extern/glfw/include/")
target_link_libraries(fishing-time-2d glfw)

# GLEW
include_directories("${PROJECT_SOURCE_DIR}/extern/glew/include/")
target_link_libraries(fishing-time-2d glew)

# STB
include_directories("${PROJECT_SOURCE_DIR}/extern/stb/include/")
