#pragma once

#include "libmath.h"
#include "sprite.h"
#include "shader.h"


typedef struct
{
    vecf2 position;
    Sprite * sprite;
} Entity;


void drawEntity(Entity const* _entity, const GLuint _shaderID);


