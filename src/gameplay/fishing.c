#include "fishing.h"

#include <stdlib.h>

#include "build_config.h"
#include "engine/animation.h"
#include "engine/log.h"
#include "engine/resources.h"
#include "engine/shader.h"


// -----------------------------------------------------------------------------

// Statics vars
// Since there are few elements, we use static instantiations.
// It is easier and OK for our purpose (instead of malloc / free).
static GLuint s_shaderID = 0;
static Anchor s_anchor;
static Explosion s_explosionsPool[GAME_NB_MAX_SHARKS]; // Static pool of explosions


// -----------------------------------------------------------------------------

static void drawBackground(Sprite const * _sprite, const GLuint _shaderID)
{
    ASSERT_MSG(_sprite != NULL, "[Draw] Cannot draw if foreground ptr is NULL");

    // The world center 0:0 is the center of the screen.
    // We hard coded to place the background in the center.
    const vecf2 center = {0.0f, 0.0f};
    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(_sprite, center, scale, _shaderID);
}

static void drawForeground(Sprite const * _sprite, const GLuint _shaderID)
{
    ASSERT_MSG(_sprite != NULL, "[Draw] Cannot draw if foreground ptr is NULL");

    // Hardcoded position but basically it is (backgroundSize - 35) / 2
    // See the background sprite size
    const vecf2 center = {0.0f, 82.5f};
    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(_sprite, center, scale, _shaderID);
}

static void drawShark(Shark const * _shark, const GLuint _shaderID)
{
    ASSERT_MSG(_shark != NULL, "[Draw] Cannot draw if shark ptr is NULL");
    ASSERT_MSG(_shark->spritesArray[_shark->anim.currentFrameIndex] != NULL, "[Draw] Shark has no sprite");

    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(_shark->spritesArray[_shark->anim.currentFrameIndex], _shark->position, scale, _shaderID);
}

static void drawBoat(Boat const * _boat, const GLuint _shaderID)
{
    ASSERT_MSG(_boat != NULL, "[Draw] Boat pointer NULL");
    ASSERT_MSG(_boat->spritesArray[_boat->anim.currentFrameIndex] != NULL, "[Draw] Boat sprite in spritesArray is NULL");

    vecf2 scale = {1.0f, 1.0f};
    scale.x *= _boat->direction;
    drawSprite(_boat->spritesArray[_boat->anim.currentFrameIndex], _boat->position, scale, _shaderID);
}

static void drawAnchor(Anchor const * _anchor, const GLuint _shaderID)
{
    ASSERT_MSG(_anchor != NULL, "[Draw] Anchor NULL");
    ASSERT_MSG(_anchor->sprite != NULL, "[Draw] Anchor has no sprite");

    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(_anchor->sprite, _anchor->position, scale, _shaderID);
}

static void drawGameUI(FishingGame const * _game, const GLuint _shaderID)
{
    // UI position is in World units (as beeing seen by camera).
    // Positions are hard coded (simply based on the visual results on screen)

    // Score (max supported 999)
    vecf2 score_position = {GAME_CAMERA_RECT_WIDTH / 2.3f, -GAME_CAMERA_RECT_HEIGHT / 2.1f};
    vecf2 scale = {1.0f, 1.0f};

    const int score = _game->score;
    int digit_1 = (score / 10) % 10;   // 92 -> gives 9
    int digit_2 = score % 10;          // 92 -> gives 2

    drawSprite(_game->textBitMap[digit_1], score_position, scale, _shaderID);
    score_position.x += _game->textBitMap[digit_1]->size.x;
    drawSprite(_game->textBitMap[digit_2], score_position, scale, _shaderID);

    // Remaining time (max supported 99 secs)
    vecf2 time_position = {0.0f , GAME_CAMERA_RECT_HEIGHT / 2.1f};

    const int time = _game->remainingTime;
    digit_1 = (time / 10) % 10;   // 92 -> gives 9
    digit_2 = time % 10;          // 92 -> gives 2

    drawSprite(_game->textBitMap[digit_1], time_position, scale, _shaderID);
    time_position.x += _game->textBitMap[digit_1]->size.x;
    drawSprite(_game->textBitMap[digit_2], time_position, scale, _shaderID);
}


// -----------------------------------------------------------------------------

// Spawn a shark on game. The given index is the shark position in the array.
// This assume the index is not a living shark.
static void spwanSharkInGame(FishingGame * _game, int _index)
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

void fishingGameUpdate(FishingGame * _game, float _dt)
{
    // End game
    _game->remainingTime -= _dt;
    if(_game->remainingTime <= 0)
    {
        _game->isPaused = 1;
    }

    // Pause
    if(_game->isPaused == 1)
    {
        _game->engine->timescale = 0.0f;
    }
    else
    {
        _game->engine->timescale = 1.0f;
    }

    static int wasPausePressed = -1;
    if(glfwGetKey(_game->engine->window, GLFW_KEY_P) == GLFW_PRESS)
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
    if(glfwGetKey(_game->engine->window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        _game->boat.direction = -1;
        _game->boat.velocity = -GAME_BOAT_SPEED;
    }
    else if(glfwGetKey(_game->engine->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        _game->boat.direction = 1;
        _game->boat.velocity = GAME_BOAT_SPEED;
    }

    // Boat shoot anchor
    static int wasPressed = -1; // Maintain key pressed count for one shot
    if(glfwGetKey(_game->engine->window, GLFW_KEY_SPACE) == GLFW_PRESS)
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

void fishingGameFixedUpdate(FishingGame * _game, float _dt)
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

void fishingGameRender(FishingGame * _game)
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
            const vecf2 scale = {1.0f, 1.0f};
            drawSprite(sprite, explosion->position, scale, s_shaderID);
        }
    }

    drawForeground(_game->foreground, s_shaderID);
    drawGameUI(_game, s_shaderID);
}


// -----------------------------------------------------------------------------

void fishingGameInit(FishingGame * _game)
{
    LOG_INFO("[Game] Initializing the game");

    s_shaderID = createShaderProgramFromFile(
            GAME_SHADERS_DIR "/vertex_shader.glsl",
            GAME_SHADERS_DIR "/fragment_shader.glsl");

    _game->isPaused = -1;
    _game->score = 0;
    _game->timeAtStartInSec = GAME_TIME_AT_START_IN_SEC;
    _game->remainingTime = _game->timeAtStartInSec;
    _game->cameraRect.x = GAME_CAMERA_RECT_WIDTH;
    _game->cameraRect.y = GAME_CAMERA_RECT_HEIGHT;

    glfwSetWindowSize(_game->engine->window, GAME_WINDOW_HEIGHT, GAME_WINDOW_WIDTH);

    // Camera is hardcoded with a default rect of vision
    setShaderProgramUniform(s_shaderID, "cameraRect", _game->cameraRect.x, _game->cameraRect.y);

    // Variables temporary used for loading
    vecf2 origin = {0.0f, 0.0f};
    unsigned int texID = 0;
    unsigned int sprite_id;
    unsigned int sprite1_id = 0;
    unsigned int sprite2_id = 0;
    unsigned int sprite3_id = 0;

    // Resource background
    texID = resourceLoadTexture(GAME_RESOURCES_DIR"/background.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(texID), 200, 200, origin);
    _game->background = resourceGetSprite(sprite_id);

    // Resource foreground
    texID = resourceLoadTexture(GAME_RESOURCES_DIR"/foreground.png");
    sprite_id = resourceLoadSprite(resourceGetTexture(texID), 200, 35, origin);
    _game->foreground = resourceGetSprite(sprite_id);

    // Resource anchor
    texID = resourceLoadTexture(GAME_RESOURCES_DIR"/spritesheet.png");
    origin.x = 153.0f;
    origin.y = 5.0f;
    sprite_id = resourceLoadSprite(resourceGetTexture(texID), 10, 11, origin);
    s_anchor.position.x = 0.0f;
    s_anchor.position.y = 0.0f;
    s_anchor.velocity = GAME_ANCHOR_SPEED;
    s_anchor.sprite = resourceGetSprite(sprite_id);
    s_anchor.collider.width = 10.0f;
    s_anchor.collider.height= 12.0f;
    _game->anchor = NULL;

    // Resource shark
    sprite1_id = 0;
    sprite2_id = 0;
    sprite3_id = 0;
    origin.x = 0;
    origin.y = 0;
    sprite1_id = resourceLoadSprite(resourceGetTexture(texID), 14, 12, origin);
    origin.x += 14.0f;
    sprite2_id = resourceLoadSprite(resourceGetTexture(texID), 14, 12, origin);
    origin.x += 14.0f;
    sprite3_id = resourceLoadSprite(resourceGetTexture(texID), 14, 12, origin);

    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        _game->sharksArray[i] = (Shark*)malloc(sizeof(Shark));
        ASSERT_MSG(_game->sharksArray[i] != NULL, "malloc(Shark) failed");

        _game->sharksArray[i]->velocity = GAME_SHARK_SPEED;
        _game->sharksArray[i]->collider.width = 12.0f;
        _game->sharksArray[i]->collider.height= 11.0f;

        _game->sharksArray[i]->spritesArray[0] = resourceGetSprite(sprite1_id);
        _game->sharksArray[i]->spritesArray[1] = resourceGetSprite(sprite2_id);
        _game->sharksArray[i]->spritesArray[2] = resourceGetSprite(sprite3_id);

        _game->sharksArray[i]->anim.currentFrameDurationInSec = 0.0f;
        _game->sharksArray[i]->anim.currentFrameIndex = 0;
        _game->sharksArray[i]->anim.frameDurationInSec = GAME_SHARK_ANIM_FRAME_DURATION_IN_SEC;
        _game->sharksArray[i]->anim.nbFrames = GAME_SHARK_ANIM_NB_FRAMES;

        spwanSharkInGame(_game, i);
    }

    // Resource explosion
    origin.x = 43.0f;
    origin.y = 3.0f;
    sprite1_id = resourceLoadSprite(resourceGetTexture(texID), 14, 14, origin);
    origin.x += 14.0f;
    sprite2_id = resourceLoadSprite(resourceGetTexture(texID), 14, 14, origin);
    origin.x += 14.0f;
    sprite3_id = resourceLoadSprite(resourceGetTexture(texID), 14, 14, origin);
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
    _game->boat.position.x = 0.0f;
    _game->boat.position.y = -81.0f;
    _game->boat.velocity = GAME_BOAT_SPEED;
    _game->boat.direction = 1; // Look at the right by default
    _game->boat.anim.nbFrames = GAME_BOAT_ANIM_NB_FRAMES;
    _game->boat.anim.currentFrameIndex = 0;
    _game->boat.anim.frameDurationInSec = GAME_BOAT_ANIM_FRAME_DURATION_IN_SEC;
    _game->boat.anim.currentFrameDurationInSec = 0.0f;

    origin.x = 0.0f;
    origin.y = 20.0f;
    for(int i = 0; i < _game->boat.anim.nbFrames; ++i)
    {
        origin.x = i * 45.0f;
        sprite_id = resourceLoadSprite(resourceGetTexture(texID), 45, 35, origin);
        _game->boat.spritesArray[i] = resourceGetSprite(sprite_id);
    }

    // Resource fonts
    texID = resourceLoadTexture(GAME_RESOURCES_DIR"/fonts.png");
    origin.x = 0.0f;
    origin.y = 0.0f;
    for(int k = 0; k < 10; ++k)
    {
        sprite_id = resourceLoadSprite(resourceGetTexture(texID), 6, 8, origin);
        _game->textBitMap[k] = resourceGetSprite(sprite_id);
        origin.x += 6;
    }

    LOG_INFO("[Game] Game successfully initialized");
}

void fishingGameDestroy(FishingGame * _game)
{
    LOG_INFO("[Game] Destroying the game");

    for(int i = 0; i < GAME_NB_MAX_SHARKS; ++i)
    {
        ASSERT_MSG(_game->sharksArray[i] != NULL, "Shark array has an unexpected NULL value");
        free(_game->sharksArray[i]);
    }

    LOG_INFO("[Game] Game successfully destroyed");
}
