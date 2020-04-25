#include "gameplay/game_app.h"

#include "engine/assertions.h"
#include "engine/inputs.h"
#include "engine/log.h"
#include "engine/shader.h"
#include "engine/str.h"
#include "engine/types.h"
#include "gameplay/fishing.h"
#include "gameplay/game_menus.h"

#include <stdlib.h>

static FishingTime* s_fishingTime = NULL;

int gameInit(void* _gamePtr)
{
    GameApp* game = (GameApp*)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

    char vertex_path[255];
    concatStrings(vertex_path, 255, game->shadersPath, "/");
    concatStrings(vertex_path, 255, vertex_path, "vertex_shader.glsl");

    char fragment_path[255];
    concatStrings(fragment_path, 255, game->shadersPath, "/");
    concatStrings(fragment_path, 255, fragment_path, "fragment_shader.glsl");

    game->engine->shaderID = createShaderProgramFromFile(vertex_path, fragment_path);

    s_fishingTime = malloc(sizeof(FishingTime));
    if (s_fishingTime == NULL) {
        LOG_ERR("[Game] Failed to malloc fishingtime");
        ASSERT_MSG(FALSE, "Failed to malloc fishingtime");
        return 42;
    }

    int success = gameResourcesLoadAll(&game->resources, game->resourcesPath);
    if (success != 0) {
        LOG_ERR("[Game] Failed to load all resources");
        ASSERT_MSG(FALSE, "[Game] Failed to load all resources");
        return 42;
    }

    fishingTimeInit(game->engine, game, s_fishingTime);
    game->currentScreen = GAME_SCREEN_WELCOME;
    game->bestScore = 0;
    game->currentScore = 0;

    return 0;
}

int gameTerminate(void* _gamePtr)
{
    GameApp* game = (GameApp*)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

    destroyShader(game->engine->shaderID);

    fishingTimeTerminate(game->engine, game, s_fishingTime);
    free(s_fishingTime);

    gameResourceUnloadAll(&game->resources);

    return 0;
}

void gameUpdate(void* _gamePtr, float _dt)
{
    GameApp* game = (GameApp*)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

    switch (game->currentScreen) {
        case GAME_SCREEN_FISHING:
            fishingTimeUpdate(game->engine, game, s_fishingTime, _dt);
            break;

        case GAME_SCREEN_WELCOME:
            welcomeUpdate(game);
            break;

        case GAME_SCREEN_GAMEOVER:
            gameoverUpdate(game);
            break;

        default:
            ASSERT_MSG(FALSE, "Not implemented switch-case value");
            break;
    }
}

void gameFixedUpdate(void* _gamePtr, float _dt)
{
    GameApp* game = (GameApp*)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

    switch (game->currentScreen) {
        case GAME_SCREEN_FISHING:
            fishingTimeFixedUpdate(game->engine, game, s_fishingTime, _dt);
            break;

        case GAME_SCREEN_WELCOME:
        case GAME_SCREEN_GAMEOVER:
            break;

        default:
            ASSERT_MSG(FALSE, "Not implemented switch-case value");
            break;
    }
}

void gameRender(void* _gamePtr)
{
    GameApp* game = (GameApp*)_gamePtr;
    ASSERT_MSG(game != NULL, "Internal critical error: NULL parameter from the engine");

    switch (game->currentScreen) {
        case GAME_SCREEN_FISHING:
            fishingTimeRender(game->engine, game, s_fishingTime);
            break;

        case GAME_SCREEN_WELCOME:
            welcomeRender(game);
            break;

        case GAME_SCREEN_GAMEOVER:
            gameoverRender(game);
            break;

        default:
            ASSERT_MSG(FALSE, "Not implemented switch-case value");
            break;
    }
}

void leaveApp(GameApp* _gameApp)
{
    ASSERT_MSG(_gameApp != NULL, "Invalid parameter");

    closeWindow(&_gameApp->engine->window);
}

void enterFishingScreen(GameApp* _gameApp)
{
    ASSERT_MSG(_gameApp != NULL, "Invalid parameter");

    fishingTimeRestart(s_fishingTime);
    _gameApp->currentScreen = GAME_SCREEN_FISHING;
}

void enterWelcomeScreen(GameApp* _gameApp)
{
    ASSERT_MSG(_gameApp != NULL, "Invalid parameter");

    _gameApp->currentScreen = GAME_SCREEN_WELCOME;
}

void enterGameoverScreen(GameApp* _gameApp)
{
    ASSERT_MSG(_gameApp != NULL, "Invalid parameter");

    _gameApp->currentScreen = GAME_SCREEN_GAMEOVER;
}
