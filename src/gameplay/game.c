#include "game.h"

#include "engine/log.h"
#include "engine/entity.h"
#include "engine/shader.h"
#include "engine/sprite.h"
#include "engine/texture.h"
#include "engine/window.h"
#include "gameplay/config.h"


static GLFWwindow * s_window  = NULL; // Yeah, ugly static var. GameJam style!
static GLuint s_shaderID = 0;

void gameInit()
{
    s_window = createWindowGLFW(
            SHARK_WINDOW_WIDTH,
            SHARK_WINDOW_HEIGHT,
            SHARK_WINDOW_TITLE);

    s_shaderID = createShaderProgramFromFile(
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
    // TODO a tmp entity
    Texture tex = makeTexture("./resources/tmp/shark_background.png");
    Sprite s = makeSprite(&tex, 640.0f, 480.0f);
    Entity e;
    e.sprite = &s;
    e.position.x = -320.0f;
    e.position.y = -240.0f;

    // Camera is hardcoded with a default rect of vision
    setShaderProgramUniform(s_shaderID, "cameraRect",
            SHARK_CAMERA_RECT_WIDTH,
            SHARK_CAMERA_RECT_HEIGHT);

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

        drawEntity(&e, s_shaderID);

        glfwSwapBuffers(s_window);
        glfwPollEvents();

        timeEndInSec = glfwGetTime();
        dt = timeEndInSec - timeBeginInSec;
        timeBeginInSec = timeEndInSec;
    }
}


