message(STATUS "Building the sources")

# Game executable
file(GLOB_RECURSE src_files *.c)
include_directories("${PROJECT_SOURCE_DIR}/src")
add_executable(fishing-time-2d ${src_files})

set(GAME_RESOURCES_DIR "${PROJECT_SOURCE_DIR}/resources")
set(GAME_SHADERS_DIR "${PROJECT_SOURCE_DIR}/shaders")
add_custom_target(rungame fishing-time-2d ${GAME_RESOURCES_DIR} ${GAME_SHADERS_DIR})

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
include_directories("${PROJECT_SOURCE_DIR}/extern/glfw/include/")
target_link_libraries(fishing-time-2d glfw)

# GLEW
add_definitions(-DGLEW_NO_GLU)
include_directories("${PROJECT_SOURCE_DIR}/extern/glew/include/")
target_link_libraries(fishing-time-2d glew)

# STB
include_directories("${PROJECT_SOURCE_DIR}/extern/stb/include/")

# Freetype
include_directories("${PROJECT_SOURCE_DIR}/extern/freetype/include/")
target_link_libraries(fishing-time-2d freetype)

# cJSON
include_directories("${PROJECT_SOURCE_DIR}/extern/cjson/")
target_link_libraries(fishing-time-2d cjson)

