#include "engine/resources.h"

#include "engine/log.h"


// -----------------------------------------------------------------------------
// Internal data
// -----------------------------------------------------------------------------

// Textures
#define S_ARRAY_TEXTURES_SIZE 5
static Texture s_arrayTextures[S_ARRAY_TEXTURES_SIZE];
static unsigned int s_arrayTexturesHead = 0;

// Sprites
#define S_ARRAY_SPRITES_SIZE 10
static Sprite s_arraySprites[S_ARRAY_SPRITES_SIZE];
static unsigned int s_arraySpritesHead = 0;


// -----------------------------------------------------------------------------
// Texture methods
// -----------------------------------------------------------------------------

unsigned int resourceLoadTexture(const char * _path)
{
    LOG_INFO("Loading texture %s", _path);
    ASSERT_MSG(s_arrayTexturesHead < S_ARRAY_TEXTURES_SIZE, "You must increase the texture array size");

    const int pos = s_arrayTexturesHead;
    s_arrayTextures[pos] = makeTexture(_path);
    s_arrayTexturesHead++;

    return pos;
}

Texture * resourceGetTexture(const unsigned int _resourceID)
{
    ASSERT_MSG(_resourceID >= 0, "Invalid texture resource ID");
    ASSERT_MSG(_resourceID < S_ARRAY_TEXTURES_SIZE, "Invalid texture resource ID");

    return &s_arrayTextures[_resourceID];
}


// -----------------------------------------------------------------------------
// Sprite methods
// -----------------------------------------------------------------------------
unsigned int resourceLoadSprite(unsigned int _textureID, int _width, int _height,
                                vecf2 _uvBottomLeft, vecf2 _uvTopRight)
{
    LOG_INFO("Loading sprite (tex=%ud, w=%d, h=%d)", _textureID, _width, _height);
    ASSERT_MSG(s_arrayTexturesHead < S_ARRAY_TEXTURES_SIZE, "You must increase the sprite array size");
    ASSERT_MSG(resourceGetTexture(_textureID) != NULL, "Cannot create a sprite with NULL texture");

    const int pos = s_arraySpritesHead;
    s_arraySprites[pos] = makeSprite(resourceGetTexture(_textureID), _width, _height, _uvBottomLeft, _uvTopRight);
    s_arraySpritesHead++;

    return pos;
}

Sprite * resourceGetSprite(unsigned int _resourceID)
{
    ASSERT_MSG(_resourceID >= 0, "Invalid sprite resource ID");
    ASSERT_MSG(_resourceID < S_ARRAY_SPRITES_SIZE, "Invalid sprite resource ID");

    return &s_arraySprites[_resourceID];
}

