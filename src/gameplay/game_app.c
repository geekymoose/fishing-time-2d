#include <stdlib.h>

#include "build_config.h"

#include "engine/log.h"
#include "engine/types.h"
#include "engine/shader.h"
#include "engine/inputs.h"

#include "gameplay/fishing.h"
#include "gameplay/game_app.h"
#include "gameplay/game_menus.h"


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
        return 42;
    }

    int success = gameResourcesLoadAll(&game->resources, GAME_RESOURCES_DIR);
    if(success != 0)
    {
        LOG_ERR("[Game] Failed to load all resources");
        ASSERT_MSG(FALSE, "[Game] Failed to load all resources");
        return 42;
    }

    fishingTimeInit(game->engine, game, s_fishingTime);
    fishingTimeRestart(s_fishingTime);

    return 0;
}

int gameDestroy(void * _gamePtr)
{
    GameApp * game = (GameApp *)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

    fishingTimeDestroy(game->engine, game, s_fishingTime);

    free(s_fishingTime);

    return 0;
}

void gameUpdate(void * _gamePtr, float _dt)
{
    GameApp * game = (GameApp *)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

    switch (game->currentScreen)
    {
        case GAME_SCREEN_FISHING:
            fishingTimeUpdate(game->engine, game, s_fishingTime, _dt);
            break;

        case GAME_SCREEN_WELCOME:
            welcomeUpdate(game);
            break;

        case GAME_SCREEN_CREDITS:
            creditsUpdate(game);
            break;

        case GAME_SCREEN_GAMEOVER:
            gameoverUpdate(game);
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
        case GAME_SCREEN_FISHING:
            fishingTimeFixedUpdate(game->engine, game, s_fishingTime, _dt);
            break;

        case GAME_SCREEN_WELCOME:
        case GAME_SCREEN_CREDITS:
        case GAME_SCREEN_GAMEOVER:
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
        case GAME_SCREEN_FISHING:
            fishingTimeRender(game->engine, game, s_fishingTime);
            break;

        case GAME_SCREEN_WELCOME:
            welcomeRender(game);
            break;

        case GAME_SCREEN_CREDITS:
            creditsRender(game);
            break;

        case GAME_SCREEN_GAMEOVER:
            gameoverRender(game);
            break;

        default:
            ASSERT_MSG(FALSE, "Not implemented switch-case value");
            break;
    }
}

