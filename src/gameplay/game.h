#pragma once

#include "engine/libmath.h"
#include "engine/sprite.h"
#include "config.h"


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
    float velocity; // Moves only on Y axis
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
    Shark * sharksArray[GAME_NB_MAX_SHARKS];
    Anchor * anchor;
    Sprite * background;
} Game;


// -----------------------------------------------------------------------------
// Methods
// -----------------------------------------------------------------------------

void gameInit();
void gameDestroy();
void gameRunLoop();

