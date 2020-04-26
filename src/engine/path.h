#pragma once

#define _PATH_MAX_SIXE 256 // Includes '\0'

/// Path is a fixed size string with / between each words.
typedef struct
{
    char str[_PATH_MAX_SIXE];
} Path;

Path makePath(int _count, ...);
void appendPath(Path* _dest, const char* _append);
