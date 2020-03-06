#pragma once

#include "engine/images.h"
#include "engine/types.h"

typedef struct
{
    unsigned int id;
    int width;
    int height;
} Texture;

/// Creates a texture.
/// If the method fails, a structure with an invalid id is returned (id = 0).
/// @param _buffer Texture's bitmap buffer.
/// @param _width Texture's bitmap width.
/// @param _height Texture's bitmap height.
/// @param _channels Texture's bitmap channels.
/// @return Returns the texture structure.
Texture makeTexture(const uint8* _buffer, int _width, int _height, int _channels);

/// Creates a texture from an image.
/// If the method fails, a structure with an invalid id is returned (id = 0).
/// @param _image Image to use to create this texture.
/// @return Returns the texture structure.
Texture makeTextureFromImage(const Image* _image);
