#pragma once

#include "engine/libmath.h"
#include "engine/shader.h"
#include "engine/texture.h"


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
    Texture * texture;
} Sprite;

Sprite makeSprite(Texture * texture, int width, int height);
void drawSprite(Sprite const* _sprite, const GLuint _shaderID);
void destroySprite(Sprite * _sprite);


