#pragma once

#include "engine/libmath.h"
#include "engine/sprite.h"


// -----------------------------------------------------------------------------
// Data
// -----------------------------------------------------------------------------

typedef struct
{
    vecf2 position;
    float velocity; // Boat can move only on x axis
    Sprite * sprite;
} Boat;

typedef struct
{
    vecf2 position;
    Sprite * sprite;
} Shark;

typedef struct
{
    Boat boat;
    Shark shark;
    Sprite * background;
} Game;


// -----------------------------------------------------------------------------
// Methods
// -----------------------------------------------------------------------------

void gameInit();
void gameDestroy();
void gameRunLoop();

