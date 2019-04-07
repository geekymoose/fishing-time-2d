#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.h"
#include "config.h"
#include "render.h"


static void glfwErrorCallback(int error, const char* description)
{
    LOG_ERR("GLFW error %d: %s\n", error, description);
}

static void glfwKeyCallback(GLFWwindow * window, int key, int scancode, int action, int modes)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

static void glfwWindowSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
    LOG_INFO("--- Start your engines ---\n");


    // Init GLFW
    if(!glfwInit())
    {
        LOG_ERR("Unable to initialize glfw. Ahah you suck!\n");
        return -1;
    }
    LOG_INFO("GLFW successfully initialized. Not that bad!\n");
    LOG_INFO("GLFW info: %s\n", glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


    // Create GLFW window
    GLFWwindow * window = glfwCreateWindow(
            SHARK_WINDOW_WIDTH,
            SHARK_WINDOW_HEIGHT,
            SHARK_WINDOW_TITLE,
            NULL,
            NULL);
    if(window == NULL)
    {
        LOG_ERR("Unable to create GLFW window. How are you gonna play now?\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // Set callbacks
    glfwSetErrorCallback(glfwErrorCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetFramebufferSizeCallback(window, glfwWindowSizeCallback);


    // Init GLEW
    GLenum glewinit = glewInit();
    if(glewinit != GLEW_OK)
    {
        LOG_ERR("Unable to init GLEW. He was not in the mood\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    LOG_INFO("GLEW successfully initialize! GG!\n");


    // OpenGL Shader
    createShaderProgramFromFile(
            "./shaders/vertex_shader.glsl",
            "./shaders/fragment_shader.glsl");


    // Main loop
    glfwSwapInterval(1);
    float timeBeginInSec = glfwGetTime();
    float timeEndInSec = 0.0f;
    float dt = 1.0f / 60.0f;
    while(!glfwWindowShouldClose(window))
    {
        int fps = (int)(1.0f / dt);
        LOG_DBG("dt = %f, fps = %d\n", dt, fps);

        glClearColor(0.76f, 0.78f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);

        timeEndInSec = glfwGetTime();
        dt = timeEndInSec - timeBeginInSec;
        timeBeginInSec = timeEndInSec;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    LOG_INFO("--- See you soon big baboune ---\n");
    return EXIT_SUCCESS;
}


