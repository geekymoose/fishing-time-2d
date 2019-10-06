#include <stdlib.h>

#include "build_config.h"
#include "engine/log.h"
#include "engine/engine.h"
#include "gameplay/game.h"


// -----------------------------------------------------------------------------

// DevNote:
// Engine does not know about the concrete game implementation and uses void* instead.
// This allows the engine to call game methods (e.g., update, fixedUpdate) without the concrete game type.
// This is a little bit ugly but ok for now.

void callGameInit(void * _gamePtr) {
    Game * game = (Game*)_gamePtr;
    gameInit(game);
}

void callGameDestroy(void * _gamePtr)
{
    Game * game = (Game*)_gamePtr;
    gameDestroy(game);
}

void callGameUpdate(void * _gamePtr, float _dt)
{
    Game * game = (Game*)_gamePtr;
    gameUpdate(game, _dt);
}

void callGameFixedUpdate(void * _gamePtr, float _dt)
{
    Game * game = (Game*)_gamePtr;
    gameFixedUpdate(game, _dt);
}

void callGameRender(void * _gamePtr)
{
    Game * game = (Game*)_gamePtr;
    gameRender(game);
}


// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
    LOG_INFO("Game version %d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR);
    LOG_INFO("--- Start your engines ---");

    Engine engine = {};

    engine.gameInit = callGameInit;
    engine.gameDestroy = callGameDestroy;
    engine.gameUpdate = callGameUpdate;
    engine.gameFixedUpdate = callGameFixedUpdate;
    engine.gameRender = callGameRender;

    Game game = {};
    game.engine = &engine;

    engineRun(&engine, &game);

    LOG_INFO("--- See you soon big baboune ---");
    return EXIT_SUCCESS;
}
