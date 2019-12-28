#include "texture.h"

#include <GL/glew.h>

#include "engine/types.h"
#include "engine/log.h"
#include "engine/assertions.h"
#include "engine/images.h"


Texture makeTexture(const char* _path)
{
    LOG_INFO("[Texture] Loading texture %s", _path);

    Texture texture;
    Image * image = loadImageFromFile(_path);
    if(image == NULL)
    {
        texture.id = 0; // Zero is not a valid ID
        return texture;
    }

    texture.width = image->width;
    texture.height = image->height;

    uint32 format;
    switch(image->channels)
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

    uint32 textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, image->buffer);
    if(glGetError() != GL_NO_ERROR)
    {
        LOG_ERR("[Texture] Method glGetError failed with GLenum = %d", glGetError());
        ASSERT_MSG(FALSE, "[Texture] Error from glTexImage2D");
    }

    destroyImage(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    texture.id = textureID;
    return texture;
}

