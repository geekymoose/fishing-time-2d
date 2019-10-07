#pragma once

#include "gameplay/fishing.h"

// Game entry point (with menu flow scenes, ingame scene etc)

typedef struct
{
    FishingGame fishingGame;
} Game;

// TODO: Not used yet
typedef enum
{
    GAMESTATE_WELCOME,
    GAMESTATE_INGAME
} GameState;


// -----------------------------------------------------------------------------

void gameInit(void * _gamePtr);
void gameDestroy(void * _gamePtr);
void gameUpdate(void * _gamePtr, float _dt);
void gameFixedUpdate(void * _gamePtr, float _dt);
void gameRender(void * _gamePtr);
