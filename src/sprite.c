#include "sprite.h"

#include <string.h> // memcpy
#include <stddef.h> // offsetof

#include "log.h"
#include "shader.h"


// Vertices of sprite describe a simple rect from 0:0 to 1:1
static float s_vertices[] =
{
    // Coord        UV
    0.0f, 1.0f,     0.0f, 1.0f,
    1.0f, 1.0f,     1.0f, 1.0f,
    1.0f, 0.0f,     1.0f, 0.0f,
    0.0f, 0.0f,     0.0f, 0.0f,
};

static unsigned int s_indices[] =
{
    0, 1, 2,
    0, 2, 3
};

Sprite makeSprite(Texture * texture, int width, int height)
{
    ASSERT_MSG(texture != NULL, "No texture provided! What are you doing?");

    Sprite sprite;
    sprite.texture = texture;
    sprite.size.x = width;
    sprite.size.y = height;
    memcpy(sprite.vertices, s_vertices, sizeof(s_vertices));

    // VAO
    glGenVertexArrays(1, &sprite.vertex_vao);
    glBindVertexArray(sprite.vertex_vao);

    // VBO
    GLuint vertex_vbo;
    glGenBuffers(1, &vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sprite.vertices), sprite.vertices, GL_STATIC_DRAW);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_indices), s_indices, GL_STATIC_DRAW);

    // Unbind all
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return sprite;
}

void drawSprite(Sprite const* _sprite, const GLuint _shaderID)
{
    glActiveTexture(GL_TEXTURE0); // Done by default actually
    glBindTexture(GL_TEXTURE_2D, _sprite->texture->id);

    setShaderProgramUniform(_shaderID, "spriteSize", _sprite->size.x, _sprite->size.y);
    glBindVertexArray(_sprite->vertex_vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void destroySprite(Sprite * _sprite)
{
    glDeleteVertexArrays(1, &_sprite->vertex_vao);
}


