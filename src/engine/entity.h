#pragma once

#include "engine/libmath.h"
#include "engine/sprite.h"
#include "engine/shader.h"


typedef struct
{
    vecf2 position;
    Sprite * sprite;
} Entity;


void drawEntity(Entity const* _entity, const GLuint _shaderID);


