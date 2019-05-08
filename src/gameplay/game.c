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
static Anchor s_anchor;


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

static void drawAnchor(Anchor const * _anchor, const GLuint _shaderID)
{
    ASSERT_MSG(_anchor != NULL, "[Draw] Anchor NULL");
    ASSERT_MSG(_anchor->sprite != NULL, "[Draw] Anchor has no sprite");

    setShaderProgramUniform(_shaderID, "position", _anchor->position.x, _anchor->position.y);
    drawSprite(_anchor->sprite, _shaderID);
}

// -----------------------------------------------------------------------------
// Static update methods
// -----------------------------------------------------------------------------

static void gameUpdate(Game * _game, float _dt)
{
    _game->boat.velocity = 0.0f;
    if(glfwGetKey(s_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        _game->boat.velocity = -SHARK_BOAT_SPEED;
    }
    else if(glfwGetKey(s_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        _game->boat.velocity = SHARK_BOAT_SPEED;
    }
    else if(glfwGetKey(s_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if(_game->anchor == NULL)
        {
            // TODO Shoot
            s_anchor.position.x = _game->boat.position.x;
            s_anchor.position.y = _game->boat.position.y;
            _game->anchor = &s_anchor;
        }
    }
}

static void gameFixedUpdate(Game * _game, float _dt)
{
    _game->boat.position.x += (_game->boat.velocity * _dt);

    if(_game->anchor != NULL)
    {
        _game->anchor->position.y += (_game->anchor->velocity * _dt);
    }
}

static void gameRender(Game * _game)
{
    drawBackground(_game->background, s_shaderID);
    drawShark(&_game->shark, s_shaderID);
    drawBoat(&_game->boat, s_shaderID);

    if(_game->anchor != NULL)
    {
        drawAnchor(_game->anchor, s_shaderID);
    }
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

    // Camera is hardcoded with a default rect of vision
    setShaderProgramUniform(s_shaderID, "cameraRect",
            SHARK_CAMERA_RECT_WIDTH,
            SHARK_CAMERA_RECT_HEIGHT);

    // Tmp vars to create game elements
    unsigned int tex_id = 0;
    unsigned int sprite_id = 0;
    vecf2 origin = {0.0f, 0.0f};

    // Game background
    tex_id = resourceLoadTexture("./resources/background.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 200, 200, origin);
    s_game.background = resourceGetSprite(sprite_id);

    // Game shark
    tex_id = resourceLoadTexture("./resources/tmp/shark_a.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 39, 16, origin);
    Shark shark = {{0.0f, 0.0f}, resourceGetSprite(sprite_id)};
    s_game.shark = shark;

    // Game boat
    tex_id = resourceLoadTexture("./resources/tmp/boat.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 45, 38, origin);
    Boat boat = {{0.0f, -100.0f}, 0.0f, resourceGetSprite(sprite_id)};
    s_game.boat = boat;

    // Game anchor
    tex_id = resourceLoadTexture("./resources/tmp/anchor.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 12, 13, origin);
    s_anchor.position.x = 0.0f;
    s_anchor.position.y = 0.0f;
    s_anchor.velocity = SHARK_ANCHOR_SPEED;
    s_anchor.sprite = resourceGetSprite(sprite_id);
    s_game.anchor = NULL; // when game has anchor set, means boat is firing
}

void gameDestroy()
{
    destroyWindowGLFW(s_window);
}

void gameRunLoop()
{
    ASSERT_MSG(s_window != NULL, "[Game] You must init the game before running it");
    ASSERT_MSG(s_shaderID != 0, "[Game] You must init the game before running it");

    glfwSwapInterval(1); // vsync

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

