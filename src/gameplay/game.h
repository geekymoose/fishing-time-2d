#pragma once

#include "gameplay/fishing.h"

// Game entry point (with menu flow scenes, ingame scene etc)

typedef enum
{
    GAME_SCREEN_WELCOME,
    GAME_SCREEN_FISHING
} GameScreen;

typedef struct
{
    FishingGame fishingGame;
    GameScreen currentScreen;
} Game;


// -----------------------------------------------------------------------------

void gameInit(void * _gamePtr);
void gameDestroy(void * _gamePtr);
void gameUpdate(void * _gamePtr, float _dt);
void gameFixedUpdate(void * _gamePtr, float _dt);
void gameRender(void * _gamePtr);
