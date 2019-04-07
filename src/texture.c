#include "texture.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "log.h"


Texture makeTexture(const char* path)
{
    stbi_set_flip_vertically_on_load(0);
    int x, y, n;
    unsigned char* data = stbi_load(path, &x, &y, &n, 0);
    if(data != NULL)
    {
        LOG_ERR("Unable to load texture %s\n", path);
        Texture t = {-1}; // Returns invalid texture. Kinda ugly but ok for now
        return t;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    Texture tex = {textureID};
    return tex;
}
