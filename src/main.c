#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.h"
#include "config.h"

void glfwErrorCallback(int error, const char* description)
{
    LOG_ERR("GLFW error %d: %s\n", error, description);
}

void glfwKeyCallback(GLFWwindow * window, int key, int scancode, int action, int modes)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

int main(int argc, char** argv)
{
    LOG_DBG("--- Start your engines ---\n");


    // Init GLFW
    if(!glfwInit())
    {
        LOG_ERR("Unable to initialize glfw. Ahah you suck!\n");
        return -1;
    }
    LOG_DBG("GLFW successfully initialized. Not that bad!\n");
    LOG_DBG("GLFW info: %s\n", glfwGetVersionString());


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


    // Init GLEW
    GLenum glewinit = glewInit();
    if(glewinit == GLEW_OK)
    {
        LOG_ERR("Unable to init GLEW. He was not in the mood\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }


    // Set callbacks
    glfwSetErrorCallback(glfwErrorCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    glfwMakeContextCurrent(window);


    // Main loop
    glfwSwapInterval(1);
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    LOG_DBG("--- See you soon big baboune ---\n");
    return EXIT_SUCCESS;
}


