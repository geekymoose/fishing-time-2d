#pragma once

#include "engine/types.h"


typedef struct
{
    uint8 * buffer;
    int width;
    int height;
    int channels;
} Image;


Image * loadImageFromFile(const char * _path);
void destroyImage(Image * image);

