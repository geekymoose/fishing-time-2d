#pragma once

#include "engine/engine.h"

// Game entry point (with menu flow scenes, ingame scene etc)

typedef enum
{
    GAME_SCREEN_WELCOME,
    GAME_SCREEN_FISHING,
    GAME_SCREEN_GAMEOVER,
    GAME_SCREEN_CREDITS
} GameScreen;

typedef struct
{
    Engine * engine;
    GameScreen currentScreen;
} GameApp;


// -----------------------------------------------------------------------------

int gameInit(void * _gamePtr);
int gameDestroy(void * _gamePtr);
void gameUpdate(void * _gamePtr, float _dt);
void gameFixedUpdate(void * _gamePtr, float _dt);
void gameRender(void * _gamePtr);
