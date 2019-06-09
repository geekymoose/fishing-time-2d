#pragma once

#include "engine/shader.h"


/// Creates a GLFW window and GL context.
/// This method also init GLFW and GLEW and must be called once at start.
/// @param _width Width of the window.
/// @param _height Heigh of the window.
/// @param _title Title of the window.
/// @return Returns a pointer to the created window.
GLFWwindow * createWindowGLFW(const int _width, const int _height, const char* _title);

/// Destroys a GLFW window, the GLcontext and terminates GLFW.
/// This is the last method to call.
/// @param _window The GLFW window to destroy.
void destroyWindowGLFW(GLFWwindow * _window);

/// Clears the whole window content.
/// @param _window the window to clear.
void clearWindow(GLFWwindow * _window);

/// Swaps window buffer.
/// @param _window the window where to swap.
void swapWindow(GLFWwindow * _window);

/// Requests closing the given window.
/// Note that this does not destroy the window (and OpenGL context).
/// @param _window The window to close.
void closeWindow(GLFWwindow * _window);

/// Checks whether the window is closed.
/// @param _window The window to check if closed.
/// @return Returns 1 if closed.
int isWindowClosed(GLFWwindow * _window);

