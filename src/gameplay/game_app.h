#pragma once

#include "engine/engine.h"
#include "gameplay/game_resources.h"

// Game entry point (with menu flow scenes, ingame scene etc)

typedef enum
{
    GAME_SCREEN_WELCOME,
    GAME_SCREEN_FISHING,
    GAME_SCREEN_GAMEOVER
} GameScreen;

typedef struct
{
    Engine* engine;
    GameScreen currentScreen;
    GameResources resources;
    char* resourcesPath;
    char* shadersPath;
    int bestScore;
    int currentScore;
} GameApp;

int gameInit(void* _gamePtr);
int gameDestroy(void* _gamePtr);
void gameUpdate(void* _gamePtr, float _dt);
void gameFixedUpdate(void* _gamePtr, float _dt);
void gameRender(void* _gamePtr);

void leaveApp(GameApp* _gameApp);
void enterFishingScreen(GameApp* _gameApp);
void enterWelcomeScreen(GameApp* _gameApp);
void enterGameoverScreen(GameApp* _gameApp);
