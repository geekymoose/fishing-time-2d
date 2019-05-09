#pragma once

#include "engine/sprite.h"


// -----------------------------------------------------------------------------
// Data
// -----------------------------------------------------------------------------

typedef struct
{
    int nbFrames;                       // Number of frames in this anim
    int currentFrameIndex;              // Index in array of the current played frame
    float frameDurationInSec;           // Duration of each frame (same for all)
    float currentFrameDurationInSec;    // Duration of the current frame
} Animation;


// -----------------------------------------------------------------------------
// Methods
// -----------------------------------------------------------------------------

/// Update the given animation according to the elapsed time.
/// @param _anim The animation to update.
/// @param _dt Elapsed time since the last animation update (in sec)
void updateAnimation(Animation * _anim, float _dt);


