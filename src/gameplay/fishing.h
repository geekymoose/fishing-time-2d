#pragma once

#include "engine/animation.h"
#include "engine/collision.h"
#include "engine/engine.h"
#include "engine/libmath.h"
#include "engine/sprite.h"
#include "gameplay/config.h"
#include "gameplay/game_app.h"

typedef struct
{
    vecf2 position;
    float direction; // Vect director on x axis
    float velocity;  // Boat can move only on x axis
    Animation anim;
} Boat;

typedef struct
{
    vecf2 position;
    float velocity; // Moves only on Y axis
    RectCollider collider;
    Animation anim;
} Fish;

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
    Fish* fishes[GAME_FISH_COUNT];
    Explosion* explosions[GAME_FISH_COUNT];
    Anchor* anchor;
    int isPaused;
    int score;
    float remainingTime;
} FishingTime;

void fishingTimeInit(Engine* _engine, GameApp* _gameapp, FishingTime* _game);
void fishingTimeTerminate(Engine* _engine, GameApp* _gameapp, FishingTime* _game);

void fishingTimeRestart(FishingTime* _game);

void fishingTimeUpdate(Engine* _engine, GameApp* _gameapp, FishingTime* _game, float _dt);
void fishingTimeFixedUpdate(Engine* _engine, GameApp* _gameapp, FishingTime* _game, float _dt);
void fishingTimeRender(Engine* _engine, GameApp* _gameapp, FishingTime* _game);
