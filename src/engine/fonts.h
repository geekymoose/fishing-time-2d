#pragma once

#include "engine/texture.h"

// TODO Note: finally not used in fishing-time-2d

typedef struct
{
    int width;
    int height;
    float uvX0;
    float uvX1;
    float uvY0;
    float uvY1;
} FontGlyph;

typedef struct
{
    int sizeInPx;
    int charStart;
    int charEnd;
    Texture texture;
    FontGlyph* glyphs;
} Font;

/// Initializes the font library.
/// @return 0 if success, otherwise, a positive number.
int initFontLibrary();

/// Terminates the font library (free all memory etc).
/// @return 0 if success, otherwise, a positive number.
int terminateFontLibrary();

/// Creates a font from a file (must be destroyed upon usage).
/// Creates a bitmap with the glyphes for the set of requested char (e.g., 'A','Z').
/// Use the ASCII representation and order (e.g., 'A','z' to load all uppercases and lowercases).
/// @param path Font file.
/// @param fontSizeInPx Size of the font (heigh in pixels).
/// @param charStart First character to load from ASCII (e.g., 'a').
/// @param charEnd Last character to load from ASCII (e.g., 'z').
/// @return The loaded font or NULL if error.
Font* createFontFromFile(const char* _path, int _fontSizeInPx, int _charStart, int _charEnd);

/// Frees the font memory.
/// @param font Pointer to the font to free.
void destroyFont(Font* _font);
