#include <stdlib.h>

#include "build_config.h"
#include "gameplay/game.h"
#include "engine/log.h"


int main(int argc, char** argv)
{
    LOG_INFO("Game version %d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR);
    LOG_INFO("--- Start your engines ---");

    gameInit();
    gameRunLoop();
    gameDestroy();

    LOG_INFO("--- See you soon big baboune ---");
    return EXIT_SUCCESS;
}


