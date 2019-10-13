#include "window.h"

#include "engine/log.h"


// -----------------------------------------------------------------------------

static void glfwWindowSizeCallback(GLFWwindow * _window, int _width, int _height)
{
    glViewport(0, 0, _width, _height);
}

static void glfwErrorCallback(int _error, const char* _description)
{
    LOG_ERR("[Window] GLFW error %d: %s", _error, _description);
}


// -----------------------------------------------------------------------------

GLFWwindow * createWindow(const int width, const int height, const char* title)
{
    // Init GLFW
    if(!glfwInit())
    {
        LOG_ERR("[Window] Unable to initialize glfw. Ahah you suck!");
        return NULL;
    }
    LOG_INFO("[Window] GLFW successfully initialized. Not that bad!");
    LOG_INFO("[Window] GLFW info: %s", glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create GLFW window
    GLFWwindow * window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(window == NULL)
    {
        LOG_ERR("[Window] Unable to create GLFW window. How are you gonna play now?");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    // Callbacks
    glfwSetFramebufferSizeCallback(window, glfwWindowSizeCallback);
    glfwSetErrorCallback(glfwErrorCallback);

    // Init GLEW
    GLenum glewinit = glewInit();
    if(glewinit != GLEW_OK)
    {
        LOG_ERR("[Window] Unable to init GLEW. He was not in the mood");
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;
    }
    LOG_INFO("[Window] GLEW successfully initialize! GG!");

    // Specific config (used for alpha color)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

void destroyWindow(GLFWwindow * _window)
{
    LOG_INFO("[Window] Destroy GLFW window and terminate GLFW");
    ASSERT_MSG(_window != NULL, "[Window] Cannot destroy NULL window");

    glfwDestroyWindow(_window);
    glfwTerminate();
}

void clearWindow(GLFWwindow * _window)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void swapWindow(GLFWwindow * _window)
{
    glfwSwapBuffers(_window);
}

void closeWindow(GLFWwindow * _window)
{
    glfwSetWindowShouldClose(_window, 1);
}

int isWindowClosed(GLFWwindow * _window)
{
    return glfwWindowShouldClose(_window);
}

void resizeWindow(GLFWwindow * _window, int _width, int _height)
{
    glfwSetWindowSize(_window, _width, _height);
}
