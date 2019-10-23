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


int gameInit(void * _gamePtr)
{
    GameApp * game = (GameApp *)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

    game->engine->shaderID = createShaderProgramFromFile(
            GAME_SHADERS_DIR "/vertex_shader.glsl",
            GAME_SHADERS_DIR "/fragment_shader.glsl");

    game->currentScreen = GAME_SCREEN_WELCOME;

    s_fishingTime = malloc(sizeof(FishingTime));
    if(s_fishingTime == NULL)
    {
        LOG_ERR("[Game] Failed to malloc fishingtime");
        ASSERT_MSG(FALSE, "Failed to malloc fishingtime");
        return 1;
    }

    welcomeInit(game);
    creditsInit(game);
    gameoverInit(game);
    fishingTimeInit(game->engine, s_fishingTime);

    return 0;
}

int gameDestroy(void * _gamePtr)
{
    GameApp * game = (GameApp *)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

    welcomeDestroy(game);
    creditsDestroy(game);
    gameoverDestroy(game);
    fishingTimeDestroy(game->engine, s_fishingTime);

    free(s_fishingTime);

    return 0;
}

void gameUpdate(void * _gamePtr, float _dt)
{
    GameApp * game = (GameApp *)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

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
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

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
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

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
