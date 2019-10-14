#pragma once

#include "engine/types.h"
#include "engine/window.h"

typedef void (*gameInitFct)(void* _game);
typedef void (*gameDestroyFct)(void* _game);
typedef void (*gameUpdateFct)(void* _game, float _dt);
typedef void (*gameFixedUpdateFct)(void* _game, float _dt);
typedef void (*gameRenderFct)(void* _game);


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

void engineRun(Engine * _engine, void * _game);
