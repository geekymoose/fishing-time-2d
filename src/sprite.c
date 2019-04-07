#include "sprite.h"

#include <string.h> // memcpy
#include <stddef.h> // offsetof

#include "log.h"
#include "shader.h"


// Vertices of sprite describe a simple rect from 0:0 to 1:1
static float s_spriteDefaultVertices[] =
{
    // Coord                UV
    /*
    0.0f, 0.0f,             0.0f, 1.0f,
    1.0f, 0.0f,             1.0f, 1.0f,
    1.0f, 1.0f,             1.0f, 0.0f,
    0.0f, 1.0f,             0.0f, 0.0f,
    */
    0.0f, 0.0f,             0.0f, 1.0f,
    1.0f, 0.0f,             1.0f, 1.0f,
    1.0f, -1.0f,            1.0f, 0.0f,
    0.0f, -1.0f,            0.0f, 0.0f,
};

static unsigned int s_spriteVerticesIndices[] =
{
    0, 1, 2,
    0, 2, 3
};

void initSprite(Sprite * _sprite)
{
    ASSERT_MSG(_sprite->size.x != 0, "Sprite with no X size");
    ASSERT_MSG(_sprite->size.y != 0, "Sprite with no Y size");
    memcpy(_sprite->vertices, s_spriteDefaultVertices, sizeof(s_spriteDefaultVertices));

    // VAO
    glGenVertexArrays(1, &_sprite->vertex_vao);
    glBindVertexArray(_sprite->vertex_vao);

    // VBO
    GLuint vertex_vbo;
    glGenBuffers(1, &vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_sprite->vertices), _sprite->vertices, GL_STATIC_DRAW);

    const int stride = sizeof(float) * 4;
    const void* offset_0 = (void*)(offsetof(Vertex1P1UV, position));
    const void* offset_1 = (void*)(offsetof(Vertex1P1UV, uv));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, offset_0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, offset_1);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // EBO
    GLuint vertex_ebo;
    glGenBuffers(1, &vertex_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_spriteVerticesIndices), s_spriteVerticesIndices, GL_STATIC_DRAW);

    // Unbind all
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawSprite(Sprite const* _sprite, const GLuint _shaderID)
{
    glBindVertexArray(_sprite->vertex_vao);
    setShaderProgramUniform(_shaderID, "spriteSize", _sprite->size.x, _sprite->size.y);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void destroySprite(Sprite * _sprite)
{
    glDeleteVertexArrays(1, &_sprite->vertex_vao);
}


