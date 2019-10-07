#include "gameplay/game.h"

void gameInit(void * _gamePtr)
{
    Game * game = (Game*)_gamePtr;
    fishingGameInit(&game->fishingGame);
}

void gameDestroy(void * _gamePtr)
{
    Game * game = (Game*)_gamePtr;
    fishingGameDestroy(&game->fishingGame);
}

void gameUpdate(void * _gamePtr, float _dt)
{
    Game * game = (Game*)_gamePtr;
    fishingGameUpdate(&game->fishingGame, _dt);
}

void gameFixedUpdate(void * _gamePtr, float _dt)
{
    Game * game = (Game*)_gamePtr;
    fishingGameFixedUpdate(&game->fishingGame, _dt);
}

void gameRender(void * _gamePtr)
{
    Game * game = (Game*)_gamePtr;
    fishingGameRender(&game->fishingGame);
}
