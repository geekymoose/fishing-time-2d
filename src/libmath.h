#pragma once

// The best lib math ever!

typedef struct
{
    float x;
    float y;
} vecf2;

typedef struct
{
    float x;
    float y;
    float z;
} vecf3;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vecf4;

typedef struct
{
    vecf4 ix;
    vecf4 iy;
    vecf4 iz;
    vecf4 iw;
} matf4;


