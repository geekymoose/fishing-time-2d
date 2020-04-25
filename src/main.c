#include <stdlib.h>

#include "engine/engine.h"
#include "engine/fonts.h"
#include "engine/log.h"
#include "gameplay/game_app.h"

int main(int argc, char** argv)
{
    LOG_INFO("--- Start your engines ---");

    Engine engine = { 0 };

    engine.gameInit = gameInit;
    engine.gameTerminate = gameTerminate;
    engine.gameUpdate = gameUpdate;
    engine.gameFixedUpdate = gameFixedUpdate;
    engine.gameRender = gameRender;

    GameApp game = { 0 };
    game.engine = &engine;

    game.resourcesPath = (argc >= 2) ? argv[1] : "./resources";
    game.shadersPath = (argc >= 3) ? argv[2] : "./shaders";

    LOG_INFO("Resources path: %s", game.resourcesPath);
    LOG_INFO("Shaders path: %s", game.shadersPath);

    int success = engineRun(&engine, &game);
    if (success != 0) {
        LOG_ERR("[Main] Engine run failed with error code %d", success);
        return EXIT_FAILURE;
    }

    LOG_INFO("--- See you soon big baboune ---");
    return EXIT_SUCCESS;
}
