#include "engine/window.h"

#include "engine/assertions.h"
#include "engine/inputs.h"
#include "engine/log.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void glfwWindowSizeCallback(GLFWwindow* _window, int _width, int _height)
{
    glViewport(0, 0, _width, _height);
}

static void glfwErrorCallback(int _error, const char* _description)
{
    LOG_ERR("[Window] GLFW error %d: %s", _error, _description);
}

static void glfwHandleKeyEvents(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
    handleKeyEvents(_key, _action);
}

Window createWindow(const int width, const int height, const char* title)
{
    // Init GLFW
    if (!glfwInit()) {
        LOG_ERR("[Window] Unable to initialize glfw. Ahah you suck!");
        Window w = { NULL };
        return w;
    }
    LOG_INFO("[Window] GLFW successfully initialized. Not that bad!");
    LOG_INFO("[Window] GLFW info: %s", glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create GLFW window
    GLFWwindow* glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (glfw_window == NULL) {
        LOG_ERR("[Window] Unable to create GLFW window. How are you gonna play now?");
        glfwTerminate();
        Window w = { NULL };
        return w;
    }
    glfwMakeContextCurrent(glfw_window);

    // Callbacks
    glfwSetFramebufferSizeCallback(glfw_window, glfwWindowSizeCallback);
    glfwSetErrorCallback(glfwErrorCallback);
    glfwSetKeyCallback(glfw_window, glfwHandleKeyEvents);

    // Init GLEW
    GLenum glewinit = glewInit();
    if (glewinit != GLEW_OK) {
        LOG_ERR("[Window] Unable to init GLEW. He was not in the mood");
        glfwDestroyWindow(glfw_window);
        glfwTerminate();
        Window w = { NULL };
        return w;
    }
    LOG_INFO("[Window] GLEW successfully initialize! GG!");

    // Specific config (used for alpha color)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Window w;
    w.context = (void*)glfw_window;
    return w;
}

void destroyWindow(Window* _window)
{
    LOG_INFO("[Window] Destroy GLFW window and terminate GLFW");
    ASSERT_MSG(_window != NULL, "[Window] Cannot destroy NULL window");

    glfwDestroyWindow((GLFWwindow*)_window->context);
    glfwTerminate();
}

void clearWindow(Window* _window)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void swapWindow(Window* _window)
{
    glfwSwapBuffers((GLFWwindow*)_window->context);
}

void closeWindow(Window* _window)
{
    glfwSetWindowShouldClose((GLFWwindow*)_window->context, 1);
}

int isWindowClosed(Window* _window)
{
    return glfwWindowShouldClose((GLFWwindow*)_window->context);
}

void resizeWindow(Window* _window, int _width, int _height)
{
    glfwSetWindowSize((GLFWwindow*)_window->context, _width, _height);
}
