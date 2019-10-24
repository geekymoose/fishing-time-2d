#pragma once

#include "engine/libmath.h"
#include "engine/sprite.h"
#include "engine/animation.h"
#include "engine/collision.h"
#include "engine/engine.h"

#include "gameplay/config.h"
#include "gameplay/game_app.h"


// -----------------------------------------------------------------------------

typedef struct
{
    vecf2 position;
    float direction; // Vect director on x axis
    float velocity; // Boat can move only on x axis
    Animation anim;
} Boat;

typedef struct
{
    vecf2 position;
    float velocity; // Moves only on Y axis
    RectCollider collider;
    Animation anim;
} Shark;

typedef struct
{
    vecf2 position;
    float velocity; // Moves only on Y axis
    RectCollider collider;
} Anchor;

typedef struct
{
    vecf2 position;
    Animation anim;
} Explosion;

typedef struct
{
    vecf2 cameraRect; // Rectangle of camera vision
    Boat boat;
    Shark * sharksArray[GAME_FISH_COUNT];
    Explosion * explosionsArray[GAME_FISH_COUNT];
    Anchor * anchor;
    int isPaused;
    int score;
    float remainingTime;
    float timeAtStartInSec; // Available amount of time at beginning
} FishingTime;


// -----------------------------------------------------------------------------

void fishingTimeInit(Engine * _engine, GameApp * _gameapp, FishingTime * _game);
void fishingTimeDestroy(Engine * _engine, GameApp * _gameapp, FishingTime * _game);
void fishingTimeUpdate(Engine * _engine, GameApp * _gameapp, FishingTime * _game, float _dt);
void fishingTimeFixedUpdate(Engine * _engine, GameApp * _gameapp, FishingTime * _game, float _dt);
void fishingTimeRender(Engine * _engine, GameApp * _gameapp, FishingTime * _game);
