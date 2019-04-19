#pragma once

#include "engine/texture.h"
#include "engine/sprite.h"


// Disclaimer
// This is a terribly simple and naive implementation of a resource registery.
// All the resources are loaded at start and no 'unload' is implemented.
// These are LSR (Load-and-Stay resident) resources that live for the game duration.
// This gives use the knowledge of the exact resource registery size.
// For this purpose, I use an array, with index as the resource ID.


// -----------------------------------------------------------------------------
// Texture methods
// -----------------------------------------------------------------------------

unsigned int resourceLoadTexture(const char * _path);
Texture * resourceGetTexture(unsigned int _resourceID);


// -----------------------------------------------------------------------------
// Sprite methods
// -----------------------------------------------------------------------------

unsigned int resourceLoadSprite(unsigned int _textureID, int _width, int _height,
                                vecf2 _uvBottomLeft, vecf2 _uvTopRight);
Sprite * resourceGetSprite(unsigned int _resourceID);
