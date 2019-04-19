#include "game.h"

#include "engine/log.h"
#include "engine/shader.h"
#include "engine/sprite.h"
#include "engine/texture.h"
#include "engine/window.h"
#include "gameplay/config.h"
#include "gameplay/shark.h"
#include "gameplay/boat.h"
#include "engine/resources.h"


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

static void drawBackground(Sprite const* _sprite, const GLuint _shaderID)
{
    const float x = -SHARK_CAMERA_RECT_WIDTH / 2.0f;
    const float y = -SHARK_CAMERA_RECT_HEIGHT / 2.0f;
    setShaderProgramUniform(_shaderID, "position", x, y);
    drawSprite(_sprite, _shaderID);
}

void gameRunLoop()
{
    // TMP UV coords
    vecf2 uv1 = {0.0f, 0.0f};
    vecf2 uv2 = {1.0f, 1.0f};

    unsigned int id = 0; // tmp id

    // TMP Background
    id = resourceLoadTexture("./resources/tmp/background.png");
    id = resourceLoadSprite(id, 640.0f, 480.0f, uv1, uv2);
    Sprite * spriteBackground = resourceGetSprite(id);

    // TMP shark
    id = resourceLoadTexture("./resources/tmp/shark_a.png");
    id = resourceLoadSprite(id, 78, 16, uv1, uv2);
    Sprite * spriteShark = resourceGetSprite(id);
    Shark shark = {{100.0f, 0.0f}, spriteShark};

    // TMP shark
    id = resourceLoadTexture("./resources/tmp/boat.png");
    id = resourceLoadSprite(id, 180, 38, uv1, uv2);
    Sprite * spriteBoat = resourceGetSprite(id);
    Boat boat = {{0.0f, -230.0f}, spriteBoat};

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

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        drawBackground(spriteBackground, s_shaderID);
        drawShark(&shark, s_shaderID);
        drawBoat(&boat, s_shaderID);

        glfwSwapBuffers(s_window);
        glfwPollEvents();

        timeEndInSec = glfwGetTime();
        dt = timeEndInSec - timeBeginInSec;
        timeBeginInSec = timeEndInSec;
    }
}


