#pragma once

#include "engine/types.h"
#include "engine/window.h"

// Engine entry point

typedef int (* gameInitFct)(void * _game);
typedef int (* gameDestroyFct)(void * _game);
typedef void (* gameUpdateFct)(void * _game, float _dt);
typedef void (* gameFixedUpdateFct)(void * _game, float _dt);
typedef void (* gameRenderFct)(void * _game);


// -----------------------------------------------------------------------------

typedef struct
{
    GLFWwindow * window;

    gameInitFct gameInit;
    gameDestroyFct gameDestroy;
    gameUpdateFct gameUpdate;
    gameFixedUpdateFct gameFixedUpdate;
    gameRenderFct gameRender;

    float timescale; // 1 for normal speed (dt = dt * timescale)
    uint32 shaderID; // One shader for now. To update with array if several shaders required.
} Engine;


// -----------------------------------------------------------------------------

/// Start your engines !
/// This blocks until the engine is stopped.
/// @param _engine Engine to use.
/// @param _game Game to run with the given engine.
/// @return 0 if success, otherwise, a positive number.
int engineRun(Engine * _engine, void * _game);
