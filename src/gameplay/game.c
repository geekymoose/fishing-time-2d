#include "game.h"

#include "engine/log.h"
#include "engine/shader.h"
#include "engine/window.h"
#include "gameplay/config.h"
#include "engine/resources.h"


// -----------------------------------------------------------------------------
// Static data
// -----------------------------------------------------------------------------

static GLFWwindow * s_window  = NULL; // Yeah, ugly static var. GameJam style!
static GLuint s_shaderID = 0;
static Game s_game;


// -----------------------------------------------------------------------------
// Static draw methods
// -----------------------------------------------------------------------------

static void drawBackground(Sprite const * _sprite, const GLuint _shaderID)
{
    const float x = -SHARK_CAMERA_RECT_WIDTH / 2.0f;
    const float y = -SHARK_CAMERA_RECT_HEIGHT / 2.0f;

    setShaderProgramUniform(_shaderID, "position", x, y);
    drawSprite(_sprite, _shaderID);
}

static void drawShark(Shark const * _shark, const GLuint _shaderID)
{
    ASSERT_MSG(_shark != NULL, "[Draw] Cannot draw if shark ptr is NULL");
    ASSERT_MSG(_shark->sprite != NULL, "[Draw] Shark has no sprite");

    setShaderProgramUniform(_shaderID, "position", _shark->position.x, _shark->position.y);
    drawSprite(_shark->sprite, _shaderID);
}

static void drawBoat(Boat const * _boat, const GLuint _shaderID)
{
    ASSERT_MSG(_boat != NULL, "[Draw] Boat NULL");
    ASSERT_MSG(_boat->sprite != NULL, "[Draw] Boat has no sprite");

    setShaderProgramUniform(_shaderID, "position", _boat->position.x, _boat->position.y);
    drawSprite(_boat->sprite, _shaderID);
}

// -----------------------------------------------------------------------------
// Static update methods
// -----------------------------------------------------------------------------

static void gameUpdate(Game * _game, float _dt)
{
    // TODO
}

static void gameFixedUpdate(Game * _game, float _dt)
{
    // TODO
}

static void gameRender(Game * _game)
{
    drawBackground(_game->background, s_shaderID);
    drawShark(&_game->shark, s_shaderID);
    drawBoat(&_game->boat, s_shaderID);
}



// -----------------------------------------------------------------------------
// Game Methods
// -----------------------------------------------------------------------------

void gameInit()
{
    s_window = createWindowGLFW(
            SHARK_WINDOW_WIDTH,
            SHARK_WINDOW_HEIGHT,
            SHARK_WINDOW_TITLE);

    s_shaderID = createShaderProgramFromFile(
            "./shaders/vertex_shader.glsl",
            "./shaders/fragment_shader.glsl");

    // Tmp vars to create game elements
    unsigned int tex_id = 0;
    unsigned int sprite_id = 0;
    vecf2 origin = {0.0f, 0.0f};

    // Game background
    tex_id = resourceLoadTexture("./resources/tmp/background.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 191, 199, origin);
    s_game.background = resourceGetSprite(sprite_id);

    // Game shark
    tex_id = resourceLoadTexture("./resources/tmp/shark_a.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 39, 16, origin);
    Shark shark = {{0.0f, 0.0f}, resourceGetSprite(sprite_id)};
    s_game.shark = shark;

    // Boat
    tex_id = resourceLoadTexture("./resources/tmp/boat.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 45, 38, origin);
    Boat boat = {{0.0f, -100.0f}, resourceGetSprite(sprite_id)};
    s_game.boat = boat;

    // Camera is hardcoded with a default rect of vision
    setShaderProgramUniform(s_shaderID, "cameraRect",
            SHARK_CAMERA_RECT_WIDTH,
            SHARK_CAMERA_RECT_HEIGHT);
}

void gameDestroy()
{
    destroyWindowGLFW(s_window);
}

void gameRunLoop()
{
    glfwSwapInterval(1);

    float timeBeginInSec = glfwGetTime();
    float timeEndInSec = 0.0f;
    float dt = 1.0f / 60.0f;

    float elapsedFixedDeltaTime = 0.0f;
    const float fixedDeltaTime = 1.0f / 45.0f; // Physic capped at 45 fps

    while(!isWindowClosed(s_window))
    {
        int fps = (int)(1.0f / dt);
        LOG_DBG("dt = %f, fps = %d (fixed dt = %f)", dt, fps, fixedDeltaTime);

        clearWindow(s_window);

        elapsedFixedDeltaTime += dt;
        if(elapsedFixedDeltaTime >= fixedDeltaTime)
        {
            // DevNotes: in case of long render loop (high dt), we don't try to
            // recover the elapsed physics steps and only play one step instead.
            elapsedFixedDeltaTime = 0.0f;
            gameFixedUpdate(&s_game, fixedDeltaTime);
        }

        gameUpdate(&s_game, dt);
        gameRender(&s_game);
        swapWindow(s_window);

        timeEndInSec = glfwGetTime();
        dt = timeEndInSec - timeBeginInSec;
        timeBeginInSec = timeEndInSec;
    }
}

