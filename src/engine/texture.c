#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "engine/shader.h"
#include "engine/log.h"


Texture makeTexture(const char* path)
{
    LOG_DBG("Loading texture from %s\n", path);
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char * data = stbi_load(path, &width, &height, &channels, 0);
    if(data == NULL)
    {
        LOG_ERR("Unable to load texture %s\n", path);
        ASSERT_MSG(0==1, "Unable to load texture");
        Texture t = {-1}; // Returns invalid texture. Kinda ugly but ok for now
        return t;
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
            ASSERT_MSG(0==1, "Invalid channels value");
            break;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                 format, GL_UNSIGNED_BYTE, data);
    if(glGetError() != GL_NO_ERROR)
    {
        LOG_ERR("Metho glGetError failed with GLenum to %d\n", glGetError());
        ASSERT_MSG(0==1, "Error from glTexImage2D");
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    Texture tex = {textureID};
    return tex;
}


