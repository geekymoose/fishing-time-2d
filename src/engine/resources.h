#pragma once

#include "engine/texture.h"
#include "engine/sprite.h"


// -----------------------------------------------------------------------------
// Texture resources
// -----------------------------------------------------------------------------

/// Loads a texture resource and adds it in the LSR resources registery.
/// @param _path Path to the texture image.
/// @return The resource ID (common for the whole engine).
unsigned int resourceLoadTexture(const char * _path);

/// Looks up for a Texture resource, given its ID.
/// @param _resourceID ID of the resource.
/// @return Pointer to the Texture resource or NULL.
Texture * resourceGetTexture(unsigned int _resourceID);


// -----------------------------------------------------------------------------
// Sprite resources
// -----------------------------------------------------------------------------

/// Loads a sprite resource and adds it in the LSR resources registery.
/// @param _tex Pointer to the texture to use for this Sprite.
/// @param _width The sprite's width in the original texture.
/// @param _height The sprite's height in the original texture.
/// @param _origin Location of the sprite in the original texture.
unsigned int resourceLoadSprite(Texture * _tex, int _width, int _height, vecf2 _origin);

/// Looks up for a Sprite resource, given its ID.
/// @param _resourceID ID of the resource.
/// @return Pointer to the Sprite resource or NULL.
Sprite * resourceGetSprite(unsigned int _resourceID);

