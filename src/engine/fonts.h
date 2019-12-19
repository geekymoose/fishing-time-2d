#pragma once


#include "engine/types.h"

typedef struct
{
    int uvX;
    int uvY;
    uint32 textureID;
} FontGlyph;

typedef struct
{
    FontGlyph * glyphs;
} Font;


/// Load the font library.
/// @return 0 if success, otherwise, a positive number.
int initFontLibrary();

/// Unload and destroy the font library.
/// @return 0 if success, otherwise, a positive number.
int destroyFontLibrary();

/// TODO
Font * loadFontFromFile(const char * path, int fontSixeInPx, int charStart, int charEnd);

