#pragma once

#include "engine/types.h"
#include "engine/libmath.h"
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
    uint32 vertex_vao;
    uint32 vertex_vbo;
    uint32 vertex_ebo;
    Vertex1P1UV vertices[4];
} Sprite;


// -----------------------------------------------------------------------------
// Methods
// -----------------------------------------------------------------------------

/// Creates a sprite from a texture.
/// A sprite may render only a part of the texture to be used with spritesheets.
/// The origin position 0:0 is the bottom left corner of the spritesheet.
/// @param _texture The original texture for this sprite.
/// @param _width The sprite's width in the original texture.
/// @param _height The sprite's height in the original texture.
/// @param _origin Location of the sprite in the original texture.
Sprite makeSprite(Texture * _texture, int _width, int _height, vecf2 _origin);

/// Frees all resources used by a sprite.
/// The sprite is no longer usable.
/// @param _sprite Pointer to the sprite to destroy.
void destroySprite(Sprite * _sprite);

/// Draws a sprite using the specified shader.
/// @param _sprite Pointer to the sprite to draw.
/// @param _center Position of the sprite's center in world coordinates.
/// @param _scale Sprite scaling.
/// @param _shaderID ID of the OpenGL shader to use.
void drawSprite(Sprite const* _sprite, vecf2 _center, vecf2 _scale, const uint32 _shaderID);


