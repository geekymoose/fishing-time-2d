#include "sprite.h"

#include <stddef.h> // offsetof
#include <GL/glew.h> // OpenGL lib

#include "engine/log.h"
#include "engine/shader.h"


// -----------------------------------------------------------------------------
// Static Data
// -----------------------------------------------------------------------------

static unsigned int s_indices[] =
{
    0, 1, 2,
    0, 2, 3
};


// -----------------------------------------------------------------------------
// Methods
// -----------------------------------------------------------------------------

Sprite makeSprite(Texture * _texture, int _width, int _height, vecf2 _origin)
{
    ASSERT_MSG(_texture != NULL, "[Sprite] No texture provided! What are you doing?");
    ASSERT_MSG(_width >= 0, "[Sprite] Invalid width (width < 0)");
    ASSERT_MSG(_height >= 0, "[Sprite] Invalid height (width < 0)");

    Sprite sprite;
    sprite.texture = _texture;
    sprite.size.x = _width;
    sprite.size.y = _height;

    const float x_max = (float)_width / 2.0f;
    const float x_min = -x_max;
    const float y_max = (float)_height / 2.0f;
    const float y_min = -y_max;

    // Top left vertex
    sprite.vertices[0].position.x = x_min;
    sprite.vertices[0].position.y = y_max;
    sprite.vertices[0].uv.x = _origin.x / _texture->width;
    sprite.vertices[0].uv.y = (_origin.y + _height) / _texture->height;

    // Top right vertex
    sprite.vertices[1].position.x = x_max;
    sprite.vertices[1].position.y = y_max;
    sprite.vertices[1].uv.x = (_origin.x + _width) / _texture->width;
    sprite.vertices[1].uv.y = (_origin.y + _height) / _texture->height;

    // Bottom right vertex
    sprite.vertices[2].position.x = x_max;
    sprite.vertices[2].position.y = y_min;
    sprite.vertices[2].uv.x = (_origin.x + _width) / _texture->width;
    sprite.vertices[2].uv.y = _origin.y / _texture->height;

    // Bottom left vertex
    sprite.vertices[3].position.x = x_min;
    sprite.vertices[3].position.y = y_min;
    sprite.vertices[3].uv.x = _origin.x / _texture->width;
    sprite.vertices[3].uv.y = _origin.y / _texture->height;


    // VAO
    glGenVertexArrays(1, &sprite.vertex_vao);
    glBindVertexArray(sprite.vertex_vao);

    // VBO
    glGenBuffers(1, &sprite.vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sprite.vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sprite.vertices), sprite.vertices, GL_STATIC_DRAW);

    const int stride = sizeof(float) * 4;
    const void* offset_0 = (void*)(offsetof(Vertex1P1UV, position));
    const void* offset_1 = (void*)(offsetof(Vertex1P1UV, uv));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, offset_0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, offset_1);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // EBO
    glGenBuffers(1, &sprite.vertex_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.vertex_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_indices), s_indices, GL_STATIC_DRAW);

    // Unbind all
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return sprite;
}

void drawSprite(Sprite const* _sprite, vecf2 _center, vecf2 _scale, const GLuint _shaderID)
{
    setShaderProgramUniform(_shaderID, "position", _center.x, _center.y);
    setShaderProgramUniform(_shaderID, "scale", _scale.x, _scale.y);

    glUseProgram(_shaderID);

    glActiveTexture(GL_TEXTURE0); // Done by default actually
    glBindTexture(GL_TEXTURE_2D, _sprite->texture->id);
    glBindVertexArray(_sprite->vertex_vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void destroySprite(Sprite * _sprite)
{
    ASSERT_MSG(_sprite != NULL, "[Sprite] Sprite should not be NULL");

    glDeleteVertexArrays(1, &_sprite->vertex_vao);

    glDeleteBuffers(1, &_sprite->vertex_ebo);
    glDeleteBuffers(1, &_sprite->vertex_vbo);
}


