#pragma once

#include "libmath.h"


// TODO Yes! we love todo things! GameJam style!
typedef struct
{
    vecf2 position; // Position 0:0 is the top-left corner
    vecf2 size;
    float vertices[4];
} Sprite;
