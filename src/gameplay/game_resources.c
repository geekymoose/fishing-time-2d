#include "gameplay/game_resources.h"

#include <string.h>

#include "engine/libmath.h"
#include "engine/log.h"
#include "engine/types.h"
#include "engine/resources.h"


static int internalLoadTexture(const char * _resourceDirPath, const char * _resourceName)
{
    const size_t dirPathSize = strlen(_resourceDirPath);
    const size_t nameSize = strlen(_resourceName);

    const char fullPathSize = (dirPathSize + nameSize + 1); // +1 for NUL char
    char fullpath[fullPathSize];
    memset(fullpath, 0, fullPathSize);
    strncat(fullpath, _resourceDirPath, dirPathSize);
    strncat(fullpath, _resourceName, nameSize);

    return resourceLoadTexture(fullpath);
}

int gameResourcesLoadAll(GameResources * _resources, const char * _resourcesDirPath)
{
    ASSERT_MSG(_resources != NULL, "Internal error: unexpected NULL parameter");
    ASSERT_MSG(_resourcesDirPath != NULL, "Internal error: unexpected NULL parameter");

    LOG_INFO("[GameResources] Loading all the game resources");


    // TMP variables
    vecf2 origin = {0.0f, 0.0f};
    unsigned int texID = 0;
    unsigned int spriteID;

    // Background
    origin.x = 0.0f;
    origin.y = 0.0f;
    texID = internalLoadTexture(_resourcesDirPath, "/background.png");
    spriteID = resourceLoadSprite(resourceGetTexture(texID), 200, 200, origin);
    _resources->background = resourceGetSprite(spriteID);
    ASSERT_MSG(_resources->background != NULL, "Invalid NULL resource");

    // Foreground
    origin.x = 0.0f;
    origin.y = 0.0f;
    texID = internalLoadTexture(_resourcesDirPath, "/foreground.png");
    spriteID = resourceLoadSprite(resourceGetTexture(texID), 200, 35, origin);
    _resources->foreground = resourceGetSprite(spriteID);
    ASSERT_MSG(_resources->foreground!= NULL, "Invalid NULL resource");

    // Fonts
    origin.x = 0.0f;
    origin.y = 0.0f;
    texID = internalLoadTexture(_resourcesDirPath, "/fonts.png");
    for(int k = 0; k < 10; ++k)
    {
        spriteID = resourceLoadSprite(resourceGetTexture(texID), 6, 8, origin);
        _resources->fontsBitmap[k] = resourceGetSprite(spriteID);
        ASSERT_MSG(_resources->fontsBitmap[k] != NULL, "Invalid NULL resource");
        origin.x += 6;
    }

    // Anchor
    origin.x = 153.0f;
    origin.y = 5.0f;
    texID = internalLoadTexture(_resourcesDirPath, "/spritesheet.png");
    spriteID = resourceLoadSprite(resourceGetTexture(texID), 10, 11, origin);
    _resources->anchor = resourceGetSprite(spriteID);
    ASSERT_MSG(_resources->anchor != NULL, "Invalid NULL resource");

    // Fish
    origin.x = 0.0f;
    origin.y = 0.0f;
    for(int k = 0; k < GAME_FISH_ANIM_NB_FRAMES; ++k)
    {
        spriteID = resourceLoadSprite(resourceGetTexture(texID), 14, 12, origin);
        _resources->fish[k] = resourceGetSprite(spriteID);
        ASSERT_MSG(_resources->fish[k] != NULL, "Invalid NULL resource");
        origin.x += 14.0f;
    }

    // Explosion
    origin.x = 43.0f;
    origin.y = 3.0f;
    for(int k = 0; k < GAME_EXPLOSION_ANIM_NB_FRAMES; ++k)
    {
        spriteID = resourceLoadSprite(resourceGetTexture(texID), 14, 14, origin);
        _resources->explosion[k] = resourceGetSprite(spriteID);
        ASSERT_MSG(_resources->explosion[k] != NULL, "Invalid NULL resource");
        origin.x += 14.0f;
    }

    // Boat
    origin.x = 0.0f;
    origin.y = 20.0f;
    for(int k = 0; k < GAME_BOAT_ANIM_NB_FRAMES; ++k)
    {
        origin.x = k * 45.0f;
        spriteID = resourceLoadSprite(resourceGetTexture(texID), 45, 35, origin);
        _resources->boat[k] = resourceGetSprite(spriteID);
        ASSERT_MSG(_resources->boat[k] != NULL, "Invalid NULL resource");
    }


    LOG_INFO("[GameResources] Game resources successfully loaded");
    return 0;
}
