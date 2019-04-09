#include "engine/entity.h"

#include "engine/log.h"


void drawEntity(Entity const* _entity, const GLuint _shaderID)
{
    ASSERT_MSG(_entity != NULL, "Entity should not be NULL");
    ASSERT_MSG(_entity->sprite != NULL, "Entity sprite should not be NULL");

    setShaderProgramUniform(_shaderID, "position", _entity->position.x, _entity->position.y);
    drawSprite(_entity->sprite, _shaderID);
}

