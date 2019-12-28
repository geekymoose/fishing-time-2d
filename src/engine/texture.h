#pragma once

#include "engine/images.h"


/// 2D Texture data.
typedef struct
{
    unsigned int id;
    int width;
    int height;
} Texture;


/// Creates a texture from an image.
/// If the method fails, a structure with an invalid id is returned (id = 0).
/// @param _image Image to use to create this texture.
/// @return Returns the texture structure.
Texture makeTexture(const Image * _image);

