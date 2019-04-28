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

/// Creates a sprite from a texture.
/// A sprite may render only a part of the texture (e.g., element of a spritesheet)
/// @param _texture The original texture for this sprite.
/// @param _width The sprite's width in the original texture.
/// @param _height The sprite's height in the original texture.
/// @param _origin Location of the sprite in the original texture.
Sprite makeSprite(Texture * _texture, int _width, int _height, vecf2 _origin);

/// Free all resources used by a sprite.
/// The sprite is no longer usable.
/// @param _sprite Pointer to the sprite to destroy.
void destroySprite(Sprite * _sprite);

/// Draw a sprite using the specified shader.
void drawSprite(Sprite const* _sprite, const GLuint _shaderID);

