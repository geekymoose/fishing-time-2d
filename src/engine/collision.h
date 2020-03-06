#pragma once

#include "engine/libmath.h"

typedef struct
{
    float width;  // Width of the collider in world space.
    float height; // Height of the collider in world space.
    vecf2 center; // Center of the collider in world space.
} RectCollider;

/// Checks whether two colliders collide.
/// The parameter order doesn't matter.
/// @param _c1 Collider one.
/// @param _c2 Collider two.
/// @return Returns 1 if collide, otherwise, return -1.
int checkIfCollide(RectCollider const* _c1, RectCollider const* _c2);
