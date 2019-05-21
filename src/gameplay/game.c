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
static Explosion s_explosionsPool[GAME_NB_MAX_SHARKS]; // Static pool of explosions


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
// Static game methods
// -----------------------------------------------------------------------------

// Spawn a shark on game. The given index is the shark position in the array.
// This assume the index is not a living shark.
static void spwanSharkInGame(Game * _game, int _index)
{
    // Since 0:0 is at the center, this returns a position X between left to righ coords
    int randomX = (rand() % (int)_game->cameraRect.x) - (_game->cameraRect.x / 2);

    // This is super arbitraty, the only goal is to place the shark outside the screen.
    // at a random distance so that he will be back in a random x seconds.
    int randomY = (rand() % (int)_game->cameraRect.y) + _game->cameraRect.y;

    Shark * shark = _game->sharksArray[_index];
    shark->position.x = randomX;
    shark->position.y = randomY;
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
    static int wasPressed = -1; // Maintain key pressed count for one shot
    if(glfwGetKey(s_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if(_game->anchor == NULL && wasPressed != 1)
        {
            wasPressed = 1;
            s_anchor.position.x = _game->boat.position.x;
            s_anchor.position.y = _game->boat.position.y;
            s_anchor.collider.center.x = s_anchor.position.x;
            s_anchor.collider.center.y = s_anchor.position.y;
            _game->anchor = &s_anchor;
        }
    }
    else
    {
        wasPressed = -1;
    }

    // Boat anim
    updateAnimation(&(_game->boat.anim), _dt);

    // Explosion anim
    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        if(_game->explosionsArray[i] != NULL)
        {
            Animation * anim = &(_game->explosionsArray[i]->anim);
            updateAnimation(anim, _dt);
            if(anim->currentFrameIndex >= anim->nbFrames - 1)
            {
                // DevNote: note that if _dt is high (game freeze), we may bypass
                // the last frame because anim 'plays' several frame instead of one
                _game->explosionsArray[i] = NULL;
            }
        }
    }
}

static void gameFixedUpdate(Game * _game, float _dt)
{
    // Boat position
    const int limit = _game->cameraRect.x / 2; // Boat cannot go outside camera
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

        if(shark->position.y <= -(_game->cameraRect.y / 2))
        {
            s_game.explosionsArray[i] = &(s_explosionsPool[i]);
            s_game.explosionsArray[i]->position.x = shark->position.x;
            s_game.explosionsArray[i]->position.y = shark->position.y;
            s_game.explosionsArray[i]->anim.currentFrameIndex = 0;
            s_game.explosionsArray[i]->anim.currentFrameDurationInSec = 0.0f;
            spwanSharkInGame(_game, i);
        }

        // Check collision if anchor
        if(_game->anchor != NULL)
        {
            if(checkIfCollide(&(shark->collider), &(_game->anchor->collider)) != -1)
            {
                s_game.explosionsArray[i] = &(s_explosionsPool[i]);
                s_game.explosionsArray[i]->position.x = shark->position.x;
                s_game.explosionsArray[i]->position.y = shark->position.y;
                s_game.explosionsArray[i]->anim.currentFrameIndex = 0;
                s_game.explosionsArray[i]->anim.currentFrameDurationInSec = 0.0f;
                _game->anchor = NULL;
                spwanSharkInGame(_game, i);
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
        if(_game->anchor->position.y >= (_game->cameraRect.y / 2))
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

    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        if(_game->explosionsArray[i] != NULL)
        {
            Explosion * explosion = _game->explosionsArray[i];
            Sprite * sprite = explosion->spritesArray[explosion->anim.currentFrameIndex];
            drawSprite(sprite, explosion->position, s_shaderID);
        }
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

    s_game.cameraRect.x = GAME_CAMERA_RECT_WIDTH;
    s_game.cameraRect.y = GAME_CAMERA_RECT_HEIGHT;

    // Camera is hardcoded with a default rect of vision
    setShaderProgramUniform(s_shaderID, "cameraRect",
                            s_game.cameraRect.x,
                            s_game.cameraRect.y);

    // Variables temporary used for loading
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
    tex_id = resourceLoadTexture("./resources/tmp/shark.png");
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
            spwanSharkInGame(&s_game, i);
            s_game.sharksArray[i]->velocity = GAME_SHARK_SPEED;
            s_game.sharksArray[i]->sprite = resourceGetSprite(sprite_id);
            s_game.sharksArray[i]->collider.width = 39.0f;
            s_game.sharksArray[i]->collider.height= 16.0f;
        }
    }

    // Resource explosion
    tex_id = resourceLoadTexture("./resources/tmp/explosion.png");
    unsigned int sprite1_id = 0;
    unsigned int sprite2_id = 0;
    unsigned int sprite3_id = 0;
    origin.x = 0;
    sprite1_id = resourceLoadSprite(resourceGetTexture(tex_id), 13, 14, origin);
    origin.x = 13;
    sprite2_id = resourceLoadSprite(resourceGetTexture(tex_id), 13, 14, origin);
    origin.x = 2 * 13;
    sprite3_id = resourceLoadSprite(resourceGetTexture(tex_id), 13, 14, origin);
    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        s_explosionsPool[i].spritesArray[0] = resourceGetSprite(sprite1_id);
        s_explosionsPool[i].spritesArray[1] = resourceGetSprite(sprite2_id);
        s_explosionsPool[i].spritesArray[2] = resourceGetSprite(sprite3_id);
        s_explosionsPool[i].anim.nbFrames = GAME_EXPLOSION_ANIM_NB_FRAMES;
        s_explosionsPool[i].anim.currentFrameIndex = 0;
        s_explosionsPool[i].anim.frameDurationInSec = GAME_EXPLOSION_ANIM_FRAME_DURATION_IN_SEC;
        s_explosionsPool[i].anim.currentFrameDurationInSec = 0.0f;
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


