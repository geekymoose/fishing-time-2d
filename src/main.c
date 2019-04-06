#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.h"

int main(int argc, char** argv)
{
    LOG_INFO("--- Start your engines ---\n");

    if(!glfwInit())
    {
    }

    LOG_DBG("--- See you soon big baboune ---\n");
    return EXIT_SUCCESS;
}


