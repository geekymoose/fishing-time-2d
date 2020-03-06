#include <stdlib.h>

#include "build_config.h"
#include "engine/engine.h"
#include "engine/fonts.h"
#include "engine/log.h"
#include "gameplay/game_app.h"

int main(int argc, char** argv)
{
    LOG_INFO("--- Start your engines ---");
    LOG_INFO("[Main] Game version %d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR);

#if 0

    // TODO Temporary debug and test for fonts
    initFontLibrary();
    Font* font = loadFontFromFile("../../resources/Pixeled.ttf", 22, 'a', 'z');
    free(font);
    destroyFontLibrary();

#else

    Engine engine = { 0 };

    engine.gameInit = gameInit;
    engine.gameDestroy = gameDestroy;
    engine.gameUpdate = gameUpdate;
    engine.gameFixedUpdate = gameFixedUpdate;
    engine.gameRender = gameRender;

    GameApp game = { 0 };
    game.engine = &engine;

    int success = engineRun(&engine, &game);
    if (success != 0) {
        LOG_ERR("[Main] Engine run failed with error code %d", success);
        return EXIT_FAILURE;
    }

#endif

    LOG_INFO("--- See you soon big baboune ---");
    return EXIT_SUCCESS;
}
