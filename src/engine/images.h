#pragma once


typedef struct
{
    unsigned char * buffer;
    int width;
    int height;
    int channels;
} Image;


Image * loadImageFromFile(const char * _path);
void destroyImage(Image * image);

