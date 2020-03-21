message(STATUS "Building the external dependencies")

# GLFW (See https://www.glfw.org/docs/latest/build_guide.html#build_link_cmake_source)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("glfw/")

# GLEW
add_definitions(-DGLEW_STATIC)
add_subdirectory("glew/build/cmake/")

# Freetype
execute_process(
    COMMAND jam -s ALL_LOCATE_TARGET=${CMAKE_BINARY_DIR}/freetype
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/extern/freetype"
)
add_library(freetype STATIC IMPORTED)
set_property(TARGET freetype PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/freetype/libfreetype.a")

# cJSON
set(ENABLE_CJSON_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory("cjson/cjson")
