#pragma once

/// Window data.
// Cast the context to the actual implementation (e.g., (GLFWwindow*)context)
typedef struct
{
    void* context;
} Window;

/// Creates a GLFW window and GL context.
/// This method also init GLFW and GLEW and must be called once at start.
/// @param _width Width of the window.
/// @param _height Heigh of the window.
/// @param _title Title of the window.
/// @return Returns the created window.
Window createWindow(const int _width, const int _height, const char* _title);

/// Destroys a GLFW window, the GLcontext and terminates GLFW.
/// This is the last method to call.
/// @param _window The GLFW window to destroy.
void destroyWindow(Window* _window);

/// Clears the whole window content.
/// @param _window the window to clear.
void clearWindow(Window* _window);

/// Swaps window buffer.
/// @param _window the window where to swap.
void swapWindow(Window* _window);

/// Requests closing the given window.
/// Note that this does not destroy the window (and OpenGL context).
/// @param _window The window to close.
void closeWindow(Window* _window);

/// Checks whether the window is closed.
/// @param _window The window to check if closed.
/// @return Returns 1 if closed.
int isWindowClosed(Window* _window);

/// Resize the window.
/// @param _window The window to resize.
/// @param _width The new window's width to apply.
/// @param _height The new Window's height to apply.
void resizeWindow(Window* _window, int _width, int _height);
