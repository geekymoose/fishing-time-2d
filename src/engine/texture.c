#include "texture.h"

#include "engine/types.h"
#include "engine/log.h"
#include "engine/assertions.h"

#include <GL/glew.h>


Texture makeTexture(const Image * _image)
{
    uint32 format;
    switch(_image->channels)
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
            format = GL_RGBA;
            ASSERT_MSG(FALSE, "[Texture] Invalid channels value %d", _image->channels);
            break;
    }

    uint32 textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, format, _image->width, _image->height, 0, format, GL_UNSIGNED_BYTE, _image->buffer);
    if(glGetError() != GL_NO_ERROR)
    {
        LOG_ERR("[Texture] Method glGetError failed with GLenum = %d", glGetError());
        ASSERT_MSG(FALSE, "[Texture] Method glGetError failed with GLenum = %d", glGetError());
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    Texture texture;
    texture.id = textureID;
    texture.width = _image->width;
    texture.height = _image->height;

    return texture;
}

