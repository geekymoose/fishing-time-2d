#include "gameplay/boat.h"

#include "engine/log.h"

void drawBoat(Boat const* _boat, const GLuint _shaderID)
{
    ASSERT_MSG(_boat != NULL, "Boat NULL\n");
    ASSERT_MSG(_boat->sprite != NULL, "Boat has no sprite\n");

    setShaderProgramUniform(_shaderID, "position", _boat->position.x, _boat->position.y);
    drawSprite(_boat->sprite, _shaderID);
}


