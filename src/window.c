#include "window.h"

#include "log.h"

static void glfwWindowSizeCallback(GLFWwindow * _window, int _width, int _height)
{
    LOG_DBG("Resizing: w = %d, h = %d\n", _width, _height);
    glViewport(0, 0, _width, _height);
}

GLFWwindow * createWindowGLFW(const int width, const int height, const char* title)
{
    // Init GLFW
    if(!glfwInit())
    {
        LOG_ERR("Unable to initialize glfw. Ahah you suck!\n");
        return NULL;
    }
    LOG_INFO("GLFW successfully initialized. Not that bad!\n");
    LOG_INFO("GLFW info: %s\n", glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create GLFW window
    GLFWwindow * window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(window == NULL)
    {
        LOG_ERR("Unable to create GLFW window. How are you gonna play now?\n");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, glfwWindowSizeCallback);

    // Init GLEW
    GLenum glewinit = glewInit();
    if(glewinit != GLEW_OK)
    {
        LOG_ERR("Unable to init GLEW. He was not in the mood\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;
    }
    LOG_INFO("GLEW successfully initialize! GG!\n");

    return window;
}


