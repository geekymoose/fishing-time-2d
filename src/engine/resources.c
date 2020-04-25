#include "engine/resources.h"

#include "engine/assertions.h"
#include "engine/images.h"

#include <stdlib.h>

// DevNote:
// This is a terribly simple and naive implementation of a resource registery.
// All the resources are loaded at start and no 'unload' is implemented.
// These are LSR (Load-and-Stay resident) resources that live for the game duration.

// DevNote2:
// Since this is a simple game, we know the exact number of needed resources
// at start, therefore, the array size is hardcoded with a known value.

// Textures
#define S_ARRAY_TEXTURES_SIZE 7
static Texture s_arrayTextures[S_ARRAY_TEXTURES_SIZE];
static unsigned int s_arrayTexturesHead = 0;

// Sprites
#define S_ARRAY_SPRITES_SIZE 26
static Sprite s_arraySprites[S_ARRAY_SPRITES_SIZE];
static unsigned int s_arraySpritesHead = 0;

unsigned int resourceLoadTexture(const char* _path)
{
    ASSERT_MSG(s_arrayTexturesHead < S_ARRAY_TEXTURES_SIZE, "[Resource] Please increase S_ARRAY_TEXTURES_SIZE");

    Image* image = createImageFromFile(_path);
    if (image == NULL) {
        return 0;
    }

    const int resourceID = s_arrayTexturesHead;

    s_arrayTextures[resourceID] = createTextureFromImage(image);
    s_arrayTexturesHead++;

    destroyImage(image);

    return resourceID;
}

void resourceUnloadAllTextures()
{
    for (int k = 0; k < S_ARRAY_TEXTURES_SIZE; ++k) {
        destroyTexture(&s_arrayTextures[k]);
    }
}

Texture* resourceGetTexture(const unsigned int _resourceID)
{
    ASSERT_MSG(_resourceID >= 0, "[Resource] Invalid texture ID");
    ASSERT_MSG(_resourceID < S_ARRAY_TEXTURES_SIZE, "[Resource] Invalid texture ID");

    return &s_arrayTextures[_resourceID];
}

unsigned int resourceLoadSprite(Texture* _tex, int _w, int _h, vecf2 _origin)
{
    ASSERT_MSG(_tex != NULL, "[Resource] Texture param _tex should not be NULL");
    ASSERT_MSG(s_arraySpritesHead < S_ARRAY_SPRITES_SIZE, "[Resource] Please increase S_ARRAY_SPRITES_SIZE");

    const int resourceID = s_arraySpritesHead;

    s_arraySprites[resourceID] = createSprite(_tex, _w, _h, _origin);
    s_arraySpritesHead++;

    return resourceID;
}

Sprite* resourceGetSprite(unsigned int _resourceID)
{
    ASSERT_MSG(_resourceID >= 0, "[Resource] Invalid sprite ID");
    ASSERT_MSG(_resourceID < S_ARRAY_SPRITES_SIZE, "[Resource] Invalid sprite ID");

    return &s_arraySprites[_resourceID];
}

void resourceUnloadAllSprites()
{
    for (int k = 0; k < S_ARRAY_SPRITES_SIZE; ++k) {
        destroySprite(&s_arraySprites[k]);
    }
}
