#include "build_config.h"
#include "engine/log.h"
#include "engine/shader.h"
#include "gameplay/game.h"
#include "gameplay/welcome.h"


void gameInit(void * _gamePtr)
{
    Game * game = (Game*)_gamePtr;
    ASSERT_MSG(game->engine != NULL, "game->engine should not be NULL");

    game->currentScreen = GAME_SCREEN_WELCOME;

    game->engine->shaderID = createShaderProgramFromFile(
            GAME_SHADERS_DIR "/vertex_shader.glsl",
            GAME_SHADERS_DIR "/fragment_shader.glsl");

    welcomeInit(game);
    fishingTimeInit(game->engine, &game->fishingTime);
}

void gameDestroy(void * _gamePtr)
{
    // Nothing yet
    Game * game = (Game*)_gamePtr;

    welcomeDestroy(game);
    fishingTimeDestroy(game->engine, &game->fishingTime);
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
            fishingTimeUpdate(game->engine, &game->fishingTime, _dt);
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
            fishingTimeFixedUpdate(game->engine, &game->fishingTime, _dt);
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
            fishingTimeRender(game->engine, &game->fishingTime);
            break;
        default:
            ASSERT_MSG(1==2, "Not implemented switch-case value");
            break;
    }
}
