#pragma once

#include "engine/libmath.h"
#include "engine/shader.h"
#include "engine/texture.h"


// -----------------------------------------------------------------------------
// Data
// -----------------------------------------------------------------------------

typedef struct
{
    vecf2 position;
    vecf2 uv;
} Vertex1P1UV;

typedef struct
{
    vecf2 size;
    Texture * texture;
    GLuint vertex_vao;
    GLuint vertex_vbo;
    GLuint vertex_ebo;
    Vertex1P1UV vertices[4];
} Sprite;


// -----------------------------------------------------------------------------
// Methods
// -----------------------------------------------------------------------------

Sprite makeSprite(Texture * _texture, int _width, int _height,
                  vecf2 _uvBottomLeft, vecf2 _uvTopRight);

void drawSprite(Sprite const* _sprite, const GLuint _shaderID);
void destroySprite(Sprite * _sprite);


