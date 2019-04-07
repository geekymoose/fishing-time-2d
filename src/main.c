#include <stdlib.h>

#include "game.h"
#include "log.h"


int main(int argc, char** argv)
{
    LOG_INFO("--- Start your engines ---\n");

    gameInit();
    gameRunLoop();
    gameDestroy();

    LOG_INFO("--- See you soon big baboune ---\n");
    return EXIT_SUCCESS;
}


