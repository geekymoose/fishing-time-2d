#include "fishing.h"

#include <stdlib.h>

#include "build_config.h"
#include "engine/types.h"
#include "engine/animation.h"
#include "engine/log.h"
#include "engine/resources.h"
#include "engine/shader.h"
#include "engine/window.h"


// -----------------------------------------------------------------------------

// Statics vars
// Since there are few elements, we use static instantiations.
// It is easier and OK for our purpose (instead of malloc / free).
static Anchor s_anchor;
static Explosion s_explosionsPool[GAME_NB_MAX_SHARKS]; // Static pool of explosions


// -----------------------------------------------------------------------------

static void drawGameUI(GameApp * _gameapp, FishingTime const * _game, const uint32 _shaderID)
{
    // UI position is in World units (as beeing seen by camera).
    // Positions are hard coded (simply based on the visual results on screen)

    // Score (max supported 999)
    vecf2 score_position = {-14.0f, GAME_CAMERA_RECT_HEIGHT / 2.1f};
    vecf2 scale = {1.0f, 1.0f};

    const int score = _game->score;
    int digit_1 = (score / 10) % 10;   // 92 -> gives 9
    int digit_2 = score % 10;          // 92 -> gives 2

    drawSprite(_gameapp->resources.fontsBitmap[digit_1], score_position, scale, _shaderID);
    score_position.x += _gameapp->resources.fontsBitmap[digit_1]->size.x;
    drawSprite(_gameapp->resources.fontsBitmap[digit_2], score_position, scale, _shaderID);

    // Remaining time (max supported 99 secs)
    vecf2 time_position = {7.0f , GAME_CAMERA_RECT_HEIGHT / 2.1f};

    const int time = _game->remainingTime;
    digit_1 = (time / 10) % 10;   // 92 -> gives 9
    digit_2 = time % 10;          // 92 -> gives 2

    drawSprite(_gameapp->resources.fontsBitmap[digit_1], time_position, scale, _shaderID);
    time_position.x += _gameapp->resources.fontsBitmap[digit_1]->size.x;
    drawSprite(_gameapp->resources.fontsBitmap[digit_2], time_position, scale, _shaderID);
}


// -----------------------------------------------------------------------------

// Spawn a shark on game. The given index is the shark position in the array.
// This assume the index is not a living shark.
static void spwanSharkInGame(FishingTime * _game, int _index)
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

void fishingTimeUpdate(Engine * _engine, GameApp * _gameapp, FishingTime * _game, float _dt)
{
    // Quit game
    if(glfwGetKey(_engine->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        _gameapp->currentScreen = GAME_SCREEN_WELCOME;
        return;
    }
    if(_game->remainingTime <= 100.0f)
    {
        if(glfwGetKey(_engine->window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            _gameapp->currentScreen = GAME_SCREEN_CREDITS;
            return;
        }
    }

    // End game
    _game->remainingTime -= _dt;
    if(_game->remainingTime <= 0)
    {
        _game->isPaused = 1;
    }

    // Pause
    if(_game->isPaused == 1)
    {
        _engine->timescale = 0.0f;
    }
    else
    {
        _engine->timescale = 1.0f;
    }

    static int wasPausePressed = -1;
    if(glfwGetKey(_engine->window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if(wasPausePressed == -1)
        {
            wasPausePressed = 1;
            _game->isPaused = (_game->isPaused == -1) ? 1 : -1;
        }
    }
    else
    {
        wasPausePressed = -1;
    }

    // Boat movement
    _game->boat.velocity = 0.0f;
    if(glfwGetKey(_engine->window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        _game->boat.direction = -1;
        _game->boat.velocity = -GAME_BOAT_SPEED;
    }
    else if(glfwGetKey(_engine->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        _game->boat.direction = 1;
        _game->boat.velocity = GAME_BOAT_SPEED;
    }

    // Boat shoot anchor
    static int wasPressed = -1; // Maintain key pressed count for one shot
    if(glfwGetKey(_engine->window, GLFW_KEY_SPACE) == GLFW_PRESS)
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

    // Shark
    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        updateAnimation(&(_game->sharksArray[i]->anim), _dt);
    }
}

void fishingTimeFixedUpdate(Engine * _engine, GameApp * _gameapp, FishingTime * _game, float _dt)
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
            _game->explosionsArray[i] = &(s_explosionsPool[i]);
            _game->explosionsArray[i]->position.x = shark->position.x;
            _game->explosionsArray[i]->position.y = shark->position.y;
            _game->explosionsArray[i]->anim.currentFrameIndex = 0;
            _game->explosionsArray[i]->anim.currentFrameDurationInSec = 0.0f;
            spwanSharkInGame(_game, i);
        }

        // Check collision if anchor
        if(_game->anchor != NULL)
        {
            if(checkIfCollide(&(shark->collider), &(_game->anchor->collider)) != -1)
            {
                _game->explosionsArray[i] = &(s_explosionsPool[i]);
                _game->explosionsArray[i]->position.x = shark->position.x;
                _game->explosionsArray[i]->position.y = shark->position.y;
                _game->explosionsArray[i]->anim.currentFrameIndex = 0;
                _game->explosionsArray[i]->anim.currentFrameDurationInSec = 0.0f;
                _game->anchor = NULL;
                _game->score++;
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

void fishingTimeRender(Engine * _engine, GameApp * _gameapp, FishingTime * _game)
{
    // Background
    // The world center 0:0 is the center of the screen.
    // We hard coded to place the background in the center.
    vecf2 center = {0.0f, 0.0f};
    vecf2 scale = {1.0f, 1.0f};
    drawSprite(_gameapp->resources.background, center, scale, _engine->shaderID);

    // Fishes
    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        ASSERT_MSG(_game->sharksArray != NULL, "Unexpected NULL shark in sharksArray");

        Shark * shark = _game->sharksArray[i];
        const vecf2 scale = {1.0f, 1.0f};
        drawSprite(_gameapp->resources.fish[shark->anim.currentFrameIndex], shark->position, scale, _engine->shaderID);
    }

    // Boat
    /*
    Boat * boat;
    scale.x = 1.0f * boat->direction;
    scale.y = 1.0f;
    drawSprite(_gameapp->resources.boat[boat->anim.currentFrameIndex], boat->position, scale, _engine->shaderID);
    */

    // Anchor
    if(_game->anchor != NULL)
    {
        const vecf2 scale = {1.0f, 1.0f};
        drawSprite(_gameapp->resources.anchor, _game->anchor->position, scale, _engine->shaderID);
    }

    // Explosions
    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        if(_game->explosionsArray[i] != NULL)
        {
            Explosion * explosion = _game->explosionsArray[i];
            Sprite * sprite = _gameapp->resources.explosion[explosion->anim.currentFrameIndex];
            const vecf2 scale = {1.0f, 1.0f};
            drawSprite(sprite, explosion->position, scale, _engine->shaderID);
        }
    }

    // Foreground
    // Hardcoded position but basically it is (backgroundSize - 35) / 2
    // See the background sprite size
    center.x = 0.0f;
    center.y = 82.5f;
    scale.x = 1.0f;
    scale.y = 1.0f;
    drawSprite(_gameapp->resources.foreground, center, scale, _engine->shaderID);

    // UI
    drawGameUI(_gameapp, _game, _engine->shaderID);
}


// -----------------------------------------------------------------------------

void fishingTimeInit(Engine * _engine, GameApp * _gameapp, FishingTime * _game)
{
    ASSERT_MSG(_engine != NULL, "Invalid parameter (should not be NULL");
    ASSERT_MSG(_game != NULL, "Invalid parameter (should not be NULL");

    LOG_INFO("[Game] Initializing the game");

    _game->isPaused = -1;
    _game->score = 0;
    _game->timeAtStartInSec = GAME_TIME_AT_START_IN_SEC;
    _game->remainingTime = _game->timeAtStartInSec;
    _game->cameraRect.x = GAME_CAMERA_RECT_WIDTH;
    _game->cameraRect.y = GAME_CAMERA_RECT_HEIGHT;

    resizeWindow(_engine->window, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

    // Camera is hardcoded with a default rect of vision
    setShaderProgramUniform(_engine->shaderID, "cameraRect", _game->cameraRect.x, _game->cameraRect.y);

    // Resource anchor
    s_anchor.position.x = 0.0f;
    s_anchor.position.y = 0.0f;
    s_anchor.velocity = GAME_ANCHOR_SPEED;
    s_anchor.collider.width = 10.0f;
    s_anchor.collider.height= 12.0f;
    _game->anchor = NULL;

    // Resource shark
    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        _game->sharksArray[i] = (Shark*)malloc(sizeof(Shark));
        ASSERT_MSG(_game->sharksArray[i] != NULL, "malloc(Shark) failed");

        _game->sharksArray[i]->velocity = GAME_SHARK_SPEED;
        _game->sharksArray[i]->collider.width = 12.0f;
        _game->sharksArray[i]->collider.height= 11.0f;

        _game->sharksArray[i]->anim.currentFrameDurationInSec = 0.0f;
        _game->sharksArray[i]->anim.currentFrameIndex = 0;
        _game->sharksArray[i]->anim.frameDurationInSec = GAME_FISH_ANIM_FRAME_DURATION_IN_SEC;
        _game->sharksArray[i]->anim.nbFrames = GAME_FISH_ANIM_NB_FRAMES;

        spwanSharkInGame(_game, i);
    }

    // Resource explosion
    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        s_explosionsPool[i].anim.nbFrames = GAME_EXPLOSION_ANIM_NB_FRAMES;
        s_explosionsPool[i].anim.currentFrameIndex = 0;
        s_explosionsPool[i].anim.frameDurationInSec = GAME_EXPLOSION_ANIM_FRAME_DURATION_IN_SEC;
        s_explosionsPool[i].anim.currentFrameDurationInSec = 0.0f;
    }

    // Resource boat
    _game->boat.position.x = 0.0f;
    _game->boat.position.y = -81.0f;
    _game->boat.velocity = GAME_BOAT_SPEED;
    _game->boat.direction = 1; // Look at the right by default
    _game->boat.anim.nbFrames = GAME_BOAT_ANIM_NB_FRAMES;
    _game->boat.anim.currentFrameIndex = 0;
    _game->boat.anim.frameDurationInSec = GAME_BOAT_ANIM_FRAME_DURATION_IN_SEC;
    _game->boat.anim.currentFrameDurationInSec = 0.0f;

    LOG_INFO("[Game] Game successfully initialized");
}

void fishingTimeDestroy(Engine * _engine, GameApp * _gameapp, FishingTime * _game)
{
    ASSERT_MSG(_engine != NULL, "Invalid parameter (should not be NULL");
    ASSERT_MSG(_game != NULL, "Invalid parameter (should not be NULL");

    LOG_INFO("[Game] Destroying the game");

    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        ASSERT_MSG(_game->sharksArray[i] != NULL, "Shark array has an unexpected NULL value");
        free(_game->sharksArray[i]);
    }

    LOG_INFO("[Game] Game successfully destroyed");
}
