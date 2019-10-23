#pragma once


// -----------------------------------------------------------------------------
// Data
// -----------------------------------------------------------------------------

/// 2D Texture data.
typedef struct
{
    unsigned int id;
    int width;
    int height;
} Texture;


// -----------------------------------------------------------------------------
// Methods
// -----------------------------------------------------------------------------

/// Creates a texture from an image located at the given path.
/// If the method fails, a structure with an invalid id is returned (id = 0).
/// @param _path Path to the image.
/// @return Returns the texture structure.
Texture makeTexture(const char* _path);

