#pragma once


typedef struct
{

} Font;


/// Load the font library.
/// @return 0 if success, otherwise, a positive number
int initFontLibrary();

Font makeFont(const char * path);

