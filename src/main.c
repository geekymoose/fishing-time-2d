#include <stdlib.h>

#include "build_config.h"
#include "engine/log.h"
#include "engine/engine.h"
#include "gameplay/game.h"


// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
    LOG_INFO("--- Start your engines ---");
    LOG_INFO("Game version %d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR);

    Engine engine = {0};

    engine.gameInit = gameInit;
    engine.gameDestroy = gameDestroy;
    engine.gameUpdate = gameUpdate;
    engine.gameFixedUpdate = gameFixedUpdate;
    engine.gameRender = gameRender;

    Game game = {0};
    game.engine = &engine;

    engineRun(&engine, &game);

    LOG_INFO("--- See you soon big baboune ---");
    return EXIT_SUCCESS;
}
