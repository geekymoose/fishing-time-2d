#include "game.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.h"
#include "config.h"
#include "sprite.h"
#include "shader.h"
#include "window.h"

static GLFWwindow * s_window  = NULL; // Yeah, ugly static var. GameJam style!


static void glfwErrorCallback(int _error, const char* _description)
{
    LOG_ERR("GLFW error %d: %s\n", _error, _description);
}

static void glfwKeyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _modes)
{
    if(_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_window, 1);
    }
}

static void glfwWindowSizeCallback(GLFWwindow * _window, int _width, int _height)
{
    glViewport(0, 0, _width, _height);
}

void gameInit()
{
    s_window = createWindowGLFW(
            SHARK_WINDOW_WIDTH,
            SHARK_WINDOW_HEIGHT,
            SHARK_WINDOW_TITLE);

    // Set callbacks
    glfwSetErrorCallback(glfwErrorCallback);
    glfwSetKeyCallback(s_window, glfwKeyCallback);
    glfwSetFramebufferSizeCallback(s_window, glfwWindowSizeCallback);

    // OpenGL Shader
    createShaderProgramFromFile(
            "./shaders/vertex_shader.glsl",
            "./shaders/fragment_shader.glsl");
}

void gameDestroy()
{
    glfwDestroyWindow(s_window);
    glfwTerminate();
}


void gameRunLoop()
{
    // Main loop
    glfwSwapInterval(1);
    float timeBeginInSec = glfwGetTime();
    float timeEndInSec = 0.0f;
    float dt = 1.0f / 60.0f;
    while(!glfwWindowShouldClose(s_window))
    {
        int fps = (int)(1.0f / dt);
        LOG_DBG("dt = %f, fps = %d\n", dt, fps);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(s_window);

        timeEndInSec = glfwGetTime();
        dt = timeEndInSec - timeBeginInSec;
        timeBeginInSec = timeEndInSec;
    }
}


