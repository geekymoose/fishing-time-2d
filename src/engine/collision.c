#include "engine/collision.h"

int checkIfCollide(const RectCollider* _c1, const RectCollider* _c2)
{
    // Tmp data
    const float demiWidthCollider1 = _c1->width / 2.0f;
    const float demiWidthCollider2 = _c2->width / 2.0f;

    const float demiHeightCollider1 = _c1->height / 2.0f;
    const float demiHeightCollider2 = _c2->height / 2.0f;

    const float xLeftCollider1 = _c1->center.x - demiWidthCollider1;
    const float xLeftCollider2 = _c2->center.x - demiWidthCollider2;

    const float xRightCollider1 = _c1->center.x + demiWidthCollider1;
    const float xRightCollider2 = _c2->center.x + demiWidthCollider2;

    const float yTopCollider1 = _c1->center.y + demiHeightCollider1;
    const float yTopCollider2 = _c2->center.y + demiHeightCollider2;

    const float yBottomCollider1 = _c1->center.y - demiHeightCollider1;
    const float yBottomCollider2 = _c2->center.y - demiHeightCollider2;

    // Collision (we check if they don't collide)
    if (xRightCollider1 < xLeftCollider2) {
        return -1;
    }
    if (xLeftCollider1 > xRightCollider2) {
        return -1;
    }
    if (yTopCollider1 < yBottomCollider2) {
        return -1;
    }
    if (yBottomCollider1 > yTopCollider2) {
        return -1;
    }

    // At this point, we collide my friend!
    return 1;
}
