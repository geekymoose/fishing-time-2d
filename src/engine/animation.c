#include "engine/animation.h"

#include "engine/log.h"


void updateAnimation(Animation * _anim, float _dt)
{
    ASSERT_MSG(_dt >= 0.0f, "Parameter _dt can't be negative");
    ASSERT_MSG(_anim != NULL, "Anim should not be NULL");

    _anim->currentFrameDurationInSec += _dt;

    if(_anim->currentFrameDurationInSec <= _anim->frameDurationInSec)
    {
        // We don't need to change the current frame, we can leave now.
        // This is actually the most common case!!
        return;
    }

    const int nbElapsedFrames = (int)(_anim->currentFrameDurationInSec / _anim->frameDurationInSec);
    _anim->currentFrameDurationInSec -= nbElapsedFrames * _anim->frameDurationInSec;
    _anim->currentFrameIndex = (_anim->currentFrameIndex + nbElapsedFrames) % _anim->nbFrames;

    // DevNote:
    // This takes into account the cases where _dt is huge (e.g., big game freeze)
    // In our game, this is kinda overkill.
    // We could have used Unity3D otherwise but well, we like stupid things.
}


