#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "engine/types.h"
#include "engine/shader.h"
#include "engine/log.h"


Texture makeTexture(const char* _path)
{
    Texture tex;
    int channels;

    LOG_INFO("[Texture] Loading texture %s", _path);

    stbi_set_flip_vertically_on_load(1);
    unsigned char * data = stbi_load(_path, &(tex.width), &(tex.height), &channels, 0);
    if(data == NULL)
    {
        LOG_ERR("[Texture] Unable to load texture %s", _path);
        ASSERT_MSG(FALSE, "[Texture] Unable to load texture");
        tex.id = (unsigned int)-1; // Invalid tex. Kinda ugly but ok for now
        return tex;
    }

    GLenum format;
    switch(channels)
    {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            ASSERT_MSG(FALSE, "Invalid channels value");
            format = GL_RGBA;
            break;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, data);
    if(glGetError() != GL_NO_ERROR)
    {
        LOG_ERR("[Texture] Method glGetError failed with GLenum = %d", glGetError());
        ASSERT_MSG(FALSE, "[Texture] Error from glTexImage2D");
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    tex.id = textureID;
    return tex;
}


