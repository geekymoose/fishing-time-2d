#include "gameplay/shark.h"

#include "engine/log.h"

void drawShark(Shark const* _shark, const GLuint _shaderID)
{
    ASSERT_MSG(_shark != NULL, "Cannot draw if shark ptr is NULL");
    ASSERT_MSG(_shark->sprite != NULL, "Shark has no sprite");

    setShaderProgramUniform(_shaderID, "position", _shark->position.x, _shark->position.y);
    drawSprite(_shark->sprite, _shaderID);
}


