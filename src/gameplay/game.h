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
    vecf2 position;
    float velocity; // Moves only on Y axis
    Sprite * sprite;
} Anchor;

typedef struct
{
    Boat boat;
    Shark shark;
    Anchor * anchor;
    Sprite * background;
} Game;


// -----------------------------------------------------------------------------
// Methods
// -----------------------------------------------------------------------------

void gameInit();
void gameDestroy();
void gameRunLoop();

