#include "game.h"

#include <stdlib.h>

#include "engine/animation.h"
#include "engine/log.h"
#include "engine/resources.h"
#include "engine/shader.h"
#include "engine/window.h"


// -----------------------------------------------------------------------------
// Static data
// -----------------------------------------------------------------------------

// Statics vars
// Since there are few elements, we use static instantiations.
// It is easier and OK for our purpose (instead of malloc / free).
static GLFWwindow * s_window  = NULL;
static GLuint s_shaderID = 0;
static Game s_game;
static Anchor s_anchor;


// -----------------------------------------------------------------------------
// Static draw methods
// -----------------------------------------------------------------------------

static void drawBackground(Sprite const * _sprite, const GLuint _shaderID)
{
    // The world center 0:0 is the center of the screen.
    // We hard coded to place the background in the center.
    const vecf2 center = {0.0f, 0.0f};
    drawSprite(_sprite, center, _shaderID);
}

static void drawShark(Shark const * _shark, const GLuint _shaderID)
{
    ASSERT_MSG(_shark != NULL, "[Draw] Cannot draw if shark ptr is NULL");
    ASSERT_MSG(_shark->sprite != NULL, "[Draw] Shark has no sprite");

    drawSprite(_shark->sprite, _shark->position, _shaderID);
}

static void drawBoat(Boat const * _boat, const GLuint _shaderID)
{
    ASSERT_MSG(_boat != NULL, "[Draw] Boat pointer NULL");
    ASSERT_MSG(_boat->spritesArray[_boat->anim.currentFrameIndex] != NULL,
               "[Draw] Boat sprite in spritesArray is NULL");

    drawSprite(_boat->spritesArray[_boat->anim.currentFrameIndex], _boat->position, _shaderID);
}

static void drawAnchor(Anchor const * _anchor, const GLuint _shaderID)
{
    ASSERT_MSG(_anchor != NULL, "[Draw] Anchor NULL");
    ASSERT_MSG(_anchor->sprite != NULL, "[Draw] Anchor has no sprite");

    drawSprite(_anchor->sprite, _anchor->position, _shaderID);
}


// -----------------------------------------------------------------------------
// Static update methods
// -----------------------------------------------------------------------------

static void gameUpdate(Game * _game, float _dt)
{
    // Boat movement
    _game->boat.velocity = 0.0f;
    if(glfwGetKey(s_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        _game->boat.velocity = -GAME_BOAT_SPEED;
    }
    else if(glfwGetKey(s_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        _game->boat.velocity = GAME_BOAT_SPEED;
    }

    // Boat shoot anchor
    if(glfwGetKey(s_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if(_game->anchor == NULL)
        {
            s_anchor.position.x = _game->boat.position.x;
            s_anchor.position.y = _game->boat.position.y;
            s_anchor.collider.center.x = s_anchor.position.x;
            s_anchor.collider.center.y = s_anchor.position.y;
            _game->anchor = &s_anchor;
        }
    }

    // Boat anim
    updateAnimation(&(_game->boat.anim), _dt);
}

static void gameFixedUpdate(Game * _game, float _dt)
{
    // Boat position
    const int limit = GAME_CAMERA_RECT_WIDTH/2; // Boat cannot go outside camera.
    const float nextpos = _game->boat.position.x + (_game->boat.velocity * _dt);
    if(nextpos > -limit && nextpos < limit)
    {
        _game->boat.position.x = nextpos;
    }

    // Sharks positions and collision
    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        ASSERT_MSG(_game->sharksArray != NULL, "Unexpected NULL shark in sharksArray");

        Shark * shark = _game->sharksArray[i];

        shark->position.y -= (shark->velocity * _dt);
        shark->collider.center.x = shark->position.x;
        shark->collider.center.y = shark->position.y;

        // Check collision if anchor
        if(_game->anchor != NULL)
        {
            if(checkIfCollide(&(shark->collider), &(_game->anchor->collider)) != -1)
            {
                LOG_DBG("Collides with shark %d", i);
                shark->position.y = 150; // TODO tmp, just move shark outside screen
                _game->anchor = NULL;
            }
        }
    }

    // Anchor position
    if(_game->anchor != NULL)
    {
        _game->anchor->position.y += (_game->anchor->velocity * _dt);
        _game->anchor->collider.center.x = _game->anchor->position.x;
        _game->anchor->collider.center.y = _game->anchor->position.y;

        // The world doesn't move, 0:0 is the center of the screen, therefore,
        // the top border is the camera height / 2
        // since camera is exactly the world size
        if(_game->anchor->position.y >= (GAME_CAMERA_RECT_HEIGHT / 2))
        {
            _game->anchor = NULL;
        }
    }
}

static void gameRender(Game * _game)
{
    drawBackground(_game->background, s_shaderID);

    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        ASSERT_MSG(_game->sharksArray != NULL, "Unexpected NULL shark in sharksArray");
        drawShark(_game->sharksArray[i], s_shaderID);
    }

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
            GAME_WINDOW_WIDTH,
            GAME_WINDOW_HEIGHT,
            GAME_WINDOW_TITLE);

    s_shaderID = createShaderProgramFromFile(
            "./shaders/vertex_shader.glsl",
            "./shaders/fragment_shader.glsl");

    // Camera is hardcoded with a default rect of vision
    setShaderProgramUniform(s_shaderID, "cameraRect",
            GAME_CAMERA_RECT_WIDTH,
            GAME_CAMERA_RECT_HEIGHT);

    // Variables used to load resources
    unsigned int tex_id = 0;
    unsigned int sprite_id = 0;
    vecf2 origin = {0.0f, 0.0f};

    // Resource Background
    tex_id = resourceLoadTexture("./resources/tmp/background.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 200, 200, origin);
    s_game.background = resourceGetSprite(sprite_id);

    // Resource anchor
    tex_id = resourceLoadTexture("./resources/tmp/anchor.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 12, 13, origin);
    s_anchor.position.x = 0.0f;
    s_anchor.position.y = 0.0f;
    s_anchor.velocity = GAME_ANCHOR_SPEED;
    s_anchor.sprite = resourceGetSprite(sprite_id);
    s_anchor.collider.width = 12.0f;
    s_anchor.collider.height= 13.0f;
    s_game.anchor = NULL; // When game has anchor set, means boat is firing

    // Resource shark
    tex_id = resourceLoadTexture("./resources/tmp/shark_a.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 39, 16, origin);

    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        s_game.sharksArray[i] = (Shark*)malloc(sizeof(Shark));

        ASSERT_MSG(s_game.sharksArray[i] != NULL, "malloc(Shark) failed");
        if(s_game.sharksArray[i] == NULL)
        {
            LOG_ERR("Unable to malloc the shark size at index %d", i);
        }
        else
        {
            s_game.sharksArray[i]->position.x = (i * 40.0f) - 100; // TODO TMP positions
            s_game.sharksArray[i]->position.y = 100.0f; // Top of the screen
            s_game.sharksArray[i]->velocity = GAME_SHARK_SPEED;
            s_game.sharksArray[i]->sprite = resourceGetSprite(sprite_id);
            s_game.sharksArray[i]->collider.width = 39.0f;
            s_game.sharksArray[i]->collider.height= 16.0f;
        }
    }

    // Resource boat
    s_game.boat.position.x = 0.0f;
    s_game.boat.position.y = -81.0f;
    s_game.boat.velocity = GAME_BOAT_SPEED;
    s_game.boat.anim.nbFrames = GAME_BOAT_ANIM_NB_FRAMES;
    s_game.boat.anim.currentFrameIndex = 0;
    s_game.boat.anim.frameDurationInSec = GAME_BOAT_ANIM_FRAME_DURATION_IN_SEC;
    s_game.boat.anim.currentFrameDurationInSec = 0.0f;

    tex_id = resourceLoadTexture("./resources/tmp/boat.png");
    for(int i = 0; i < s_game.boat.anim.nbFrames; ++i)
    {
        origin.x = i * 45;
        sprite_id = resourceLoadSprite(resourceGetTexture(tex_id), 45, 38, origin);
        s_game.boat.spritesArray[i] = resourceGetSprite(sprite_id);
    }
}

void gameDestroy()
{
    destroyWindowGLFW(s_window);

    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        ASSERT_MSG(s_game.sharksArray[i] != NULL, "Shark array has an unexpected NULL value");
        free(s_game.sharksArray[i]);
    }
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
        // const int fps = (int)(1.0f / dt);
        // LOG_DBG("dt = %f, fps = %d (fixed dt = %f)", dt, fps, fixedDeltaTime);

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


