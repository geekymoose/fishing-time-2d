#include <stdlib.h>

#include "gameplay/game.h"
#include "engine/log.h"


int main(int argc, char** argv)
{
    LOG_INFO("--- Start your engines ---");

    gameInit();
    gameRunLoop();
    gameDestroy();

    LOG_INFO("--- See you soon big baboune ---");
    return EXIT_SUCCESS;
}


