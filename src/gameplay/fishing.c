#include "gameplay/fishing.h"

#include "engine/animation.h"
#include "engine/assertions.h"
#include "engine/inputs.h"
#include "engine/log.h"
#include "engine/resources.h"
#include "engine/shader.h"
#include "engine/types.h"
#include "engine/window.h"

#include <GLFW/glfw3.h> // HACK see where a glfw is used here (remove if not used anymore)
#include <stdlib.h>

// Statics vars
// Since there are few elements, we use static instantiations.
// It is easier and OK for our purpose (instead of malloc / free).
static Anchor s_anchor;
static Explosion s_explosionsPool[GAME_FISH_COUNT]; // Static pool of explosions

static void drawGameUI(GameApp* _gameapp, const FishingTime* _game, const uint32 _shaderID)
{
    // UI position is in World units (as beeing seen by camera).
    // Positions are hard coded (simply based on the visual results on screen)

    // Score (max supported 999)
    vecf2 score_position = { -14.0f, GAME_CAMERA_RECT_HEIGHT / 2.1f };
    vecf2 scale = { 1.0f, 1.0f };

    const int score = _game->score;
    int digit_hundred = (score / 100) % 10; // 100 -> gives 1
    int digit_ten = (score / 10) % 10;      // 92 -> gives 9
    int digit_unit = score % 10;            // 92 -> gives 2

    drawSprite(_gameapp->resources.fontsBitmap[digit_hundred], score_position, scale, _shaderID);
    score_position.x += _gameapp->resources.fontsBitmap[digit_hundred]->size.x;
    drawSprite(_gameapp->resources.fontsBitmap[digit_ten], score_position, scale, _shaderID);
    score_position.x += _gameapp->resources.fontsBitmap[digit_ten]->size.x;
    drawSprite(_gameapp->resources.fontsBitmap[digit_unit], score_position, scale, _shaderID);

    // Remaining time (max supported 99 secs)
    vecf2 time_position = { 10.0f, GAME_CAMERA_RECT_HEIGHT / 2.1f };

    const int time = _game->remainingTime;
    digit_ten = (time / 10) % 10; // 92 -> gives 9
    digit_unit = time % 10;       // 92 -> gives 2

    drawSprite(_gameapp->resources.fontsBitmap[digit_ten], time_position, scale, _shaderID);
    time_position.x += _gameapp->resources.fontsBitmap[digit_ten]->size.x;
    drawSprite(_gameapp->resources.fontsBitmap[digit_unit], time_position, scale, _shaderID);
}

// Spawn a fish on game. The given index is the fish position in the array.
// This assume the index is not a living fish.
static void spwanFishInGame(FishingTime* _game, int _index)
{
    // Since 0:0 is at the center, this returns a position X between left to righ coords
    int randomX = (rand() % (int)_game->cameraRect.x) - (_game->cameraRect.x / 2);

    // This is super arbitraty, the only goal is to place the fish outside the screen.
    // at a random distance so that he will be back in a random x seconds.
    int randomY = (rand() % (int)_game->cameraRect.y) + _game->cameraRect.y;

    Fish* fish = _game->fishes[_index];
    fish->position.x = randomX;
    fish->position.y = randomY;
}

void fishingTimeUpdate(Engine* _engine, GameApp* _gameapp, FishingTime* _game, float _dt)
{
    // Quit game
    if (isKeyDown(KEY_ESCAPE) == TRUE) {
        enterWelcomeScreen(_gameapp);
        return;
    }
    if (_game->remainingTime <= 0.0f) {
        enterGameoverScreen(_gameapp);
        _gameapp->currentScore = _game->score;
        if (_gameapp->bestScore < _game->score) {
            _gameapp->bestScore = _game->score;
        }
        return;
    }

    // Restart game
    if (isKeyDown(KEY_R) == TRUE) {
        fishingTimeRestart(_game);
        return;
    }

    // End game
    _game->remainingTime -= _dt;
    if (_game->remainingTime <= 0) {
        _game->isPaused = TRUE;
    }

    // Pause
    if (_game->isPaused == TRUE) {
        _engine->timescale = 0.0f;
    } else {
        _engine->timescale = 1.0f;
    }

    static int wasPausePressed = -1;
    if (isKeyDown(KEY_P) == TRUE) {
        if (wasPausePressed == -1) {
            wasPausePressed = 1;
            _game->isPaused = (_game->isPaused == FALSE) ? TRUE : FALSE;
        }
    } else {
        wasPausePressed = -1;
    }

    // Boat movement
    /*
    // TODO this should be the righ code to place
    _game->boat.velocity = 0.0f;
    if (isKeyPressed(KEY_LEFT)) {
        _game->boat.direction = -1;
        _game->boat.velocity = -GAME_BOAT_SPEED;
    } else if (isKeyPressed(KEY_RIGHT)) {
        _game->boat.direction = 1;
        _game->boat.velocity = GAME_BOAT_SPEED;
    }
    */
    // TODO: Update with new input system (not working yet)
    // HACK
    _game->boat.velocity = 0.0f;
    if (glfwGetKey((GLFWwindow*)_engine->window.context, GLFW_KEY_LEFT) == GLFW_PRESS) {
        _game->boat.direction = -1;
        _game->boat.velocity = -GAME_BOAT_SPEED;
    } else if (glfwGetKey((GLFWwindow*)_engine->window.context, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        _game->boat.direction = 1;
        _game->boat.velocity = GAME_BOAT_SPEED;
    }

    // Boat shoot anchor
    static int wasPressed = -1; // Maintain key pressed count for one shot
    if (isKeyDown(KEY_SPACE) == TRUE) {
        if (_game->anchor == NULL && wasPressed != 1) {
            wasPressed = 1;
            s_anchor.position.x = _game->boat.position.x;
            s_anchor.position.y = _game->boat.position.y;
            s_anchor.collider.center.x = s_anchor.position.x;
            s_anchor.collider.center.y = s_anchor.position.y;
            _game->anchor = &s_anchor;
        }
    } else {
        wasPressed = -1;
    }

    // Boat anim
    updateAnimation(&(_game->boat.anim), _dt);

    // Explosion anim
    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        if (_game->explosions[i] != NULL) {
            Animation* anim = &(_game->explosions[i]->anim);
            updateAnimation(anim, _dt);
            if (anim->currentFrameIndex >= anim->nbFrames - 1) {
                // DevNote: note that if _dt is high (game freeze), we may bypass
                // the last frame because anim 'plays' several frame instead of one
                _game->explosions[i] = NULL;
            }
        }
    }

    // Fishes
    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        updateAnimation(&(_game->fishes[i]->anim), _dt);
    }
}

void fishingTimeFixedUpdate(Engine* _engine, GameApp* _gameapp, FishingTime* _game, float _dt)
{
    // Boat position
    const int limit = _game->cameraRect.x / 2; // Boat cannot go outside camera
    const float nextpos = _game->boat.position.x + (_game->boat.velocity * _dt);
    if (nextpos > -limit && nextpos < limit) {
        _game->boat.position.x = nextpos;
    }

    // Fish positions and collision
    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        ASSERT_MSG(_game->fishes != NULL, "Unexpected NULL element in array");

        Fish* fish = _game->fishes[i];

        fish->position.y -= (fish->velocity * _dt);
        fish->collider.center.x = fish->position.x;
        fish->collider.center.y = fish->position.y;

        if (fish->position.y <= -(_game->cameraRect.y / 2)) {
            _game->explosions[i] = &(s_explosionsPool[i]);
            _game->explosions[i]->position.x = fish->position.x;
            _game->explosions[i]->position.y = fish->position.y;
            _game->explosions[i]->anim.currentFrameIndex = 0;
            _game->explosions[i]->anim.currentFrameDurationInSec = 0.0f;
            spwanFishInGame(_game, i);
        }

        // Check collision if anchor
        if (_game->anchor != NULL) {
            if (checkIfCollide(&(fish->collider), &(_game->anchor->collider)) != -1) {
                _game->explosions[i] = &(s_explosionsPool[i]);
                _game->explosions[i]->position.x = fish->position.x;
                _game->explosions[i]->position.y = fish->position.y;
                _game->explosions[i]->anim.currentFrameIndex = 0;
                _game->explosions[i]->anim.currentFrameDurationInSec = 0.0f;
                _game->anchor = NULL;
                _game->score++;
                spwanFishInGame(_game, i);
            }
        }
    }

    // Anchor position
    if (_game->anchor != NULL) {
        _game->anchor->position.y += (_game->anchor->velocity * _dt);
        _game->anchor->collider.center.x = _game->anchor->position.x;
        _game->anchor->collider.center.y = _game->anchor->position.y;

        // The world doesn't move, 0:0 is the center of the screen, therefore,
        // the top border is the camera height / 2
        // since camera is exactly the world size
        if (_game->anchor->position.y >= (_game->cameraRect.y / 2)) {
            _game->anchor = NULL;
        }
    }
}

void fishingTimeRender(Engine* _engine, GameApp* _gameapp, FishingTime* _game)
{
    // Background
    // The world center 0:0 is the center of the screen.
    // We hard coded to place the background in the center.
    vecf2 center = { 0.0f, 0.0f };
    vecf2 scale = { 1.0f, 1.0f };
    drawSprite(_gameapp->resources.background, center, scale, _engine->shaderID);

    // Fishes
    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        ASSERT_MSG(_game->fishes != NULL, "Unexpected NULL element");

        Fish* fish = _game->fishes[i];
        const vecf2 scale = { 1.0f, 1.0f };
        drawSprite(_gameapp->resources.fish[fish->anim.currentFrameIndex], fish->position, scale, _engine->shaderID);
    }

    // Boat
    Boat* boat = &_game->boat;
    scale.x = 1.0f * boat->direction;
    scale.y = 1.0f;
    drawSprite(_gameapp->resources.boat[boat->anim.currentFrameIndex], boat->position, scale, _engine->shaderID);

    // Anchor
    if (_game->anchor != NULL) {
        const vecf2 scale = { 1.0f, 1.0f };
        drawSprite(_gameapp->resources.anchor, _game->anchor->position, scale, _engine->shaderID);
    }

    // Explosions
    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        if (_game->explosions[i] != NULL) {
            Explosion* explosion = _game->explosions[i];
            Sprite* sprite = _gameapp->resources.explosion[explosion->anim.currentFrameIndex];
            const vecf2 scale = { 1.0f, 1.0f };
            drawSprite(sprite, explosion->position, scale, _engine->shaderID);
        }
    }

    // Foreground UI
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

void fishingTimeRestart(FishingTime* _game)
{
    ASSERT_MSG(_game != NULL, "Invalid parameter (should not be NULL");

    LOG_INFO("[Game] Reseting the game data");

    _game->score = 0;
    _game->remainingTime = GAME_TIME_AT_START_IN_SEC;
    _game->isPaused = FALSE;

    // Boat
    _game->boat.position.x = 0.0f;
    _game->boat.position.y = -81.0f;
    _game->boat.velocity = GAME_BOAT_SPEED;
    _game->boat.direction = 1; // Look at the right by default
    _game->boat.anim.currentFrameIndex = 0;
    _game->boat.anim.currentFrameDurationInSec = 0.0f;

    // Anchor
    s_anchor.velocity = GAME_ANCHOR_SPEED;
    s_anchor.collider.width = 10.0f;
    s_anchor.collider.height = 12.0f;
    _game->anchor = NULL;

    // Fishes
    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        _game->fishes[i]->velocity = GAME_FISH_SPEED;

        _game->fishes[i]->anim.currentFrameDurationInSec = 0.0f;
        _game->fishes[i]->anim.currentFrameIndex = 0;

        spwanFishInGame(_game, i);
    }

    // Explosions
    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        // Remove all explosion from the current running game
        _game->explosions[i] = NULL;
    }

    LOG_INFO("[Game] Successfully reset the game data");
}

void fishingTimeInit(Engine* _engine, GameApp* _gameapp, FishingTime* _game)
{
    ASSERT_MSG(_engine != NULL, "Invalid parameter (should not be NULL");
    ASSERT_MSG(_game != NULL, "Invalid parameter (should not be NULL");

    LOG_INFO("[Game] Initializing the game");

    _game->cameraRect.x = GAME_CAMERA_RECT_WIDTH;
    _game->cameraRect.y = GAME_CAMERA_RECT_HEIGHT;
    resizeWindow(&_engine->window, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

    // Camera is hardcoded with a default rect of vision
    setShaderProgramUniform(_engine->shaderID, "cameraRect", _game->cameraRect.x, _game->cameraRect.y);

    // Fishes
    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        _game->fishes[i] = (Fish*)malloc(sizeof(Fish));
        ASSERT_MSG(_game->fishes[i] != NULL, "malloc failed");

        _game->fishes[i]->collider.width = 12.0f;
        _game->fishes[i]->collider.height = 11.0f;
        _game->fishes[i]->anim.frameDurationInSec = GAME_FISH_ANIM_FRAME_DURATION_IN_SEC;
        _game->fishes[i]->anim.nbFrames = GAME_FISH_ANIM_NB_FRAMES;
    }

    // Explosions
    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        s_explosionsPool[i].anim.nbFrames = GAME_EXPLOSION_ANIM_NB_FRAMES;
        s_explosionsPool[i].anim.frameDurationInSec = GAME_EXPLOSION_ANIM_FRAME_DURATION_IN_SEC;
    }

    // Boat
    _game->boat.anim.frameDurationInSec = GAME_BOAT_ANIM_FRAME_DURATION_IN_SEC;
    _game->boat.anim.nbFrames = GAME_BOAT_ANIM_NB_FRAMES;

    LOG_INFO("[Game] Game successfully initialized");
}

void fishingTimeTerminate(Engine* _engine, GameApp* _gameapp, FishingTime* _game)
{
    ASSERT_MSG(_engine != NULL, "Invalid parameter (should not be NULL");
    ASSERT_MSG(_game != NULL, "Invalid parameter (should not be NULL");

    LOG_INFO("[Game] Destroying the game");

    for (int i = 0; i < GAME_FISH_COUNT; ++i) {
        ASSERT_MSG(_game->fishes[i] != NULL, "Shark array has an unexpected NULL value");
        free(_game->fishes[i]);
    }

    LOG_INFO("[Game] Game successfully destroyed");
}
