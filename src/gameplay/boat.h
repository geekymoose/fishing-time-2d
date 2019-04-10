#pragma once

#include "engine/libmath.h"
#include "engine/sprite.h"
#include "engine/shader.h"

typedef struct
{
    vecf2 position;
    Sprite * sprite;
} Boat;

void drawBoat(Boat const* _boat, const GLuint _shaderID);


