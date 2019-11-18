message(STATUS "Building the external dependencies")

# GLFW (See https://www.glfw.org/docs/latest/build_guide.html#build_link_cmake_source)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("glfw/")

# GLEW
add_subdirectory("glew/build/cmake/")
add_definitions(-DGLEW_STATIC)
