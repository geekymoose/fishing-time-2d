#pragma once

#include "engine/libmath.h"
#include "engine/sprite.h"
#include "engine/shader.h"

typedef struct
{
    vecf2 position;
    Sprite * sprite;
} Shark;

void drawShark(Shark const* _shark, const GLuint _shaderID);


