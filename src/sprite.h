#pragma once

#include "libmath.h"
#include "shader.h"


typedef struct
{
    vecf2 position;
    vecf2 uv;
} Vertex1P1UV;

typedef struct
{
    vecf2 size;
    Vertex1P1UV vertices[4];
    GLuint vertex_vao;
} Sprite;

void initSprite(Sprite * _sprite);
void drawSprite(Sprite const* _sprite, const GLuint _shaderID);
void destroySprite(Sprite * _sprite);


