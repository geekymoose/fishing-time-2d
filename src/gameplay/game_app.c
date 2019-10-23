#include <stdlib.h>

#include "build_config.h"

#include "engine/log.h"
#include "engine/types.h"
#include "engine/shader.h"

#include "gameplay/credits.h"
#include "gameplay/fishing.h"
#include "gameplay/game_app.h"
#include "gameplay/gameover.h"
#include "gameplay/welcome.h"


static FishingTime * s_fishingTime = NULL;


void gameInit(void * _gamePtr)
{
    GameApp * game = (GameApp *)_gamePtr;
    ASSERT_MSG(game->engine != NULL, "game->engine should not be NULL");

    game->engine->shaderID = createShaderProgramFromFile(
            GAME_SHADERS_DIR "/vertex_shader.glsl",
            GAME_SHADERS_DIR "/fragment_shader.glsl");

    game->currentScreen = GAME_SCREEN_WELCOME;

    s_fishingTime = malloc(sizeof(FishingTime));
    ASSERT_MSG(s_fishingTime != NULL, "Failed to malloc fishingtime");

    welcomeInit(game);
    creditsInit(game);
    gameoverInit(game);
    fishingTimeInit(game->engine, s_fishingTime);
}

void gameDestroy(void * _gamePtr)
{
    GameApp * game = (GameApp *)_gamePtr;

    welcomeDestroy(game);
    creditsDestroy(game);
    gameoverDestroy(game);
    fishingTimeDestroy(game->engine, s_fishingTime);

    free(s_fishingTime);
}

void gameUpdate(void * _gamePtr, float _dt)
{
    GameApp * game = (GameApp *)_gamePtr;

    switch (game->currentScreen)
    {
        case GAME_SCREEN_WELCOME:
            welcomeUpdate(game);
            break;

        case GAME_SCREEN_CREDITS:
            creditsUpdate(game);
            break;

        case GAME_SCREEN_GAMEOVER:
            gameoverUpdate(game);
            break;

        case GAME_SCREEN_FISHING:
            fishingTimeUpdate(game->engine, s_fishingTime, _dt);
            break;

        default:
            ASSERT_MSG(FALSE, "Not implemented switch-case value");
            break;
    }
}

void gameFixedUpdate(void * _gamePtr, float _dt)
{
    GameApp * game = (GameApp *)_gamePtr;

    switch (game->currentScreen)
    {
        case GAME_SCREEN_WELCOME:
            welcomeRender(game);
            break;

        case GAME_SCREEN_CREDITS:
            creditsFixedUpdate(game);
            break;

        case GAME_SCREEN_GAMEOVER:
            gameoverFixedUpdate(game);
            break;

        case GAME_SCREEN_FISHING:
            fishingTimeFixedUpdate(game->engine, s_fishingTime, _dt);
            break;

        default:
            ASSERT_MSG(FALSE, "Not implemented switch-case value");
            break;
    }
}

void gameRender(void * _gamePtr)
{
    GameApp * game = (GameApp *)_gamePtr;

    switch (game->currentScreen)
    {
        case GAME_SCREEN_WELCOME:
            welcomeRender(game);
            break;

        case GAME_SCREEN_CREDITS:
            creditsRender(game);
            break;

        case GAME_SCREEN_GAMEOVER:
            gameoverRender(game);
            break;

        case GAME_SCREEN_FISHING:
            fishingTimeRender(game->engine, s_fishingTime);
            break;

        default:
            ASSERT_MSG(FALSE, "Not implemented switch-case value");
            break;
    }
}
