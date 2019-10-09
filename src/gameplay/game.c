#include "engine/log.h"
#include "gameplay/game.h"
#include "gameplay/welcome.h"


void gameInit(void * _gamePtr)
{
    Game * game = (Game*)_gamePtr;
    //game->currentScreen = GAME_SCREEN_WELCOME;
    game->currentScreen = GAME_SCREEN_FISHING;
    fishingGameInit(&game->fishingGame);
}

void gameDestroy(void * _gamePtr)
{
    // Nothing yet
    Game * game = (Game*)_gamePtr;
    fishingGameDestroy(&game->fishingGame);
}

void gameUpdate(void * _gamePtr, float _dt)
{
    Game * game = (Game*)_gamePtr;

    switch (game->currentScreen)
    {
        case GAME_SCREEN_WELCOME:
            welcomeUpdate(game);
            break;

        case GAME_SCREEN_FISHING:
            fishingGameUpdate(&game->fishingGame, _dt);
            break;

        default:
            ASSERT_MSG(1==2, "Not implemented switch-case value");
            break;
    }
}

void gameFixedUpdate(void * _gamePtr, float _dt)
{
    Game * game = (Game*)_gamePtr;

    switch (game->currentScreen)
    {
        case GAME_SCREEN_WELCOME:
            welcomeRender(game);
            break;
        case GAME_SCREEN_FISHING:
            fishingGameFixedUpdate(&game->fishingGame, _dt);
            break;
        default:
            ASSERT_MSG(1==2, "Not implemented switch-case value");
            break;
    }
}

void gameRender(void * _gamePtr)
{
    Game * game = (Game*)_gamePtr;

    switch (game->currentScreen)
    {
        case GAME_SCREEN_WELCOME:
            welcomeRender(game);
            break;
        case GAME_SCREEN_FISHING:
            fishingGameRender(&game->fishingGame);
            break;
        default:
            ASSERT_MSG(1==2, "Not implemented switch-case value");
            break;
    }
}
