#include "gameplay/game_resources.h"

#include "engine/assertions.h"
#include "engine/files.h"
#include "engine/libmath.h"
#include "engine/log.h"
#include "engine/resources.h"
#include "engine/str.h"
#include "engine/types.h"

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int internalLoadTexture(const char* _resourceDirPath, const char* _resourceName)
{
    const size_t size = strlen(_resourceDirPath) + strlen(_resourceName) + 2; // +2 for the '/' and '\0'
    char fullpath[size];
    concatStrings(fullpath, size, _resourceDirPath, "/");
    concatStrings(fullpath, size, fullpath, _resourceName);

    return resourceLoadTexture(fullpath);
}

static unsigned int internalLoadSpriteFromJSON(const cJSON* _json, const char* _name, unsigned int _texID)
{
    // TODO Add validity checks

    const cJSON* json_sprite = cJSON_GetObjectItemCaseSensitive(_json, _name);

    const cJSON* json_position = cJSON_GetObjectItemCaseSensitive(json_sprite, "position");
    cJSON* x = cJSON_GetObjectItemCaseSensitive(json_position, "x");
    cJSON* y = cJSON_GetObjectItemCaseSensitive(json_position, "y");

    const cJSON* json_size = cJSON_GetObjectItemCaseSensitive(json_sprite, "size");
    cJSON* w = cJSON_GetObjectItemCaseSensitive(json_size, "w");
    cJSON* h = cJSON_GetObjectItemCaseSensitive(json_size, "h");

    vecf2 origin = { x->valueint, y->valueint };

    return resourceLoadSprite(resourceGetTexture(_texID), w->valueint, h->valueint, origin);
}

static void internalLoadSpritesheet(GameResources* _resources, const char* _resourcesDirPath)
{
    LOG_INFO("[GameResources] Loading spritesheet resource");

    const char* _resourceName = "spritesheet.json";

    const size_t size = strlen(_resourcesDirPath) + strlen(_resourceName) + 2; // +2 for the '/' and '\0'
    char fullpath[size];
    concatStrings(fullpath, size, _resourcesDirPath, "/");
    concatStrings(fullpath, size, fullpath, _resourceName);

    char* json_buffer = (char*)newReadFileContent(fullpath);
    cJSON* json = cJSON_Parse(json_buffer);

    if (json == NULL) {
        LOG_ERR("[GameResources] Unable to load JSON resources (%s)", fullpath);
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            LOG_ERR("[GameResources] JSON error: '%s'", error_ptr);
        }
        return;
    }

    unsigned int texID = internalLoadTexture(_resourcesDirPath, "spritesheet.png");

    // Anchor
    _resources->anchor = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-anchor", texID));
    ASSERT_MSG(_resources->anchor != NULL, "Invalid NULL resource");

    // Fish
    _resources->fish[0] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-fish-01", texID));
    _resources->fish[1] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-fish-02", texID));
    _resources->fish[2] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-fish-03", texID));
    ASSERT_MSG(_resources->fish[0] != NULL, "Invalid NULL resource");
    ASSERT_MSG(_resources->fish[1] != NULL, "Invalid NULL resource");
    ASSERT_MSG(_resources->fish[2] != NULL, "Invalid NULL resource");

    // Explosion
    _resources->explosion[0] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-explosion-01", texID));
    _resources->explosion[1] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-explosion-02", texID));
    _resources->explosion[2] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-explosion-03", texID));
    ASSERT_MSG(_resources->explosion[0] != NULL, "Invalid NULL resource");
    ASSERT_MSG(_resources->explosion[1] != NULL, "Invalid NULL resource");
    ASSERT_MSG(_resources->explosion[2] != NULL, "Invalid NULL resource");

    // Boat
    _resources->boat[0] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-boat-01", texID));
    _resources->boat[1] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-boat-02", texID));
    _resources->boat[2] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-boat-03", texID));
    _resources->boat[3] = resourceGetSprite(internalLoadSpriteFromJSON(json, "sprite-boat-04", texID));
    ASSERT_MSG(_resources->boat[0] != NULL, "Invalid NULL resource");
    ASSERT_MSG(_resources->boat[1] != NULL, "Invalid NULL resource");
    ASSERT_MSG(_resources->boat[2] != NULL, "Invalid NULL resource");
    ASSERT_MSG(_resources->boat[3] != NULL, "Invalid NULL resource");

    free(json_buffer);
    cJSON_Delete(json);
}

int gameResourcesLoadAll(GameResources* _resources, const char* _resourcesDirPath)
{
    ASSERT_MSG(_resources != NULL, "Internal error: unexpected NULL parameter");
    ASSERT_MSG(_resourcesDirPath != NULL, "Internal error: unexpected NULL parameter");

    LOG_INFO("[GameResources] Loading all the game resources");

    internalLoadSpritesheet(_resources, _resourcesDirPath);

    // TMP variables
    vecf2 origin = { 0.0f, 0.0f };
    unsigned int texID = 0;
    unsigned int spriteID;

    // Background
    texID = internalLoadTexture(_resourcesDirPath, "background.png");
    spriteID = resourceLoadSprite(resourceGetTexture(texID), 200, 200, origin);
    _resources->background = resourceGetSprite(spriteID);
    ASSERT_MSG(_resources->background != NULL, "Invalid NULL resource");

    // Foreground
    texID = internalLoadTexture(_resourcesDirPath, "foreground.png");
    spriteID = resourceLoadSprite(resourceGetTexture(texID), 200, 35, origin);
    _resources->foreground = resourceGetSprite(spriteID);
    ASSERT_MSG(_resources->foreground != NULL, "Invalid NULL resource");

    // Welcome
    texID = internalLoadTexture(_resourcesDirPath, "welcome.png");
    spriteID = resourceLoadSprite(resourceGetTexture(texID), 200, 200, origin);
    _resources->welcome = resourceGetSprite(spriteID);
    ASSERT_MSG(_resources->welcome != NULL, "Invalid NULL resource");

    // Gameover
    texID = internalLoadTexture(_resourcesDirPath, "gameover.png");
    spriteID = resourceLoadSprite(resourceGetTexture(texID), 200, 200, origin);
    _resources->gameover = resourceGetSprite(spriteID);
    ASSERT_MSG(_resources->gameover != NULL, "Invalid NULL resource");

    // Fonts
    texID = internalLoadTexture(_resourcesDirPath, "fonts.png");
    for (int k = 0; k < 10; ++k) {
        spriteID = resourceLoadSprite(resourceGetTexture(texID), 6, 8, origin);
        _resources->fontsBitmap[k] = resourceGetSprite(spriteID);
        ASSERT_MSG(_resources->fontsBitmap[k] != NULL, "Invalid NULL resource");
        origin.x += 6;
    }

    LOG_INFO("[GameResources] Game resources successfully loaded");
    return 0;
}

void gameResourceUnloadAll(GameResources* _resources)
{
    ASSERT_MSG(_resources != NULL, "Invalid NULL resources");

    resourceUnloadAllSprites();
    resourceUnloadAllTextures();
}
