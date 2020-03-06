#pragma once

#include "engine/libmath.h"
#include "engine/sprite.h"
#include "gameplay/config.h"

// This game is pretty simple.
// We can load all the required resources (e.g., Textures, Sprites) at start.

typedef struct
{
    Sprite* background;
    Sprite* foreground;
    Sprite* anchor;
    Sprite* boat[GAME_BOAT_ANIM_NB_FRAMES];
    Sprite* fish[GAME_FISH_ANIM_NB_FRAMES];
    Sprite* explosion[GAME_EXPLOSION_ANIM_NB_FRAMES];
    Sprite* fontsBitmap[10]; // The font has only 10 hard coded digits
    Sprite* welcome;
    Sprite* gameover;
    Sprite* credits;
} GameResources;

int gameResourcesLoadAll(GameResources* _resources, const char* _resourcesDirPath);
