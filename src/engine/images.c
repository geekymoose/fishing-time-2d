#include "engine/images.h"

#include "engine/assertions.h"
#include "engine/log.h"
#include "engine/types.h"

#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image* loadImageFromFile(const char* _path)
{
    LOG_INFO("[Image] Loading image %s", _path);
    ASSERT_MSG(_path != NULL, "[Image] Internal error: parameter cannot be NULL");

    Image* image = malloc(sizeof(Image));
    if (image == NULL) {
        LOG_ERR("[Image] Unable to malloc memory for image %s", _path);
        ASSERT_MSG(FALSE, "[Image] Unable to malloc memory for image %s", _path);
        return NULL;
    }

    stbi_set_flip_vertically_on_load(1);

    image->buffer = stbi_load(_path, &(image->width), &(image->height), &(image->channels), 0);
    if (image->buffer == NULL) {
        LOG_ERR("[Image] Unable to load the image %s", _path);
        ASSERT_MSG(FALSE, "[Image] Unable to load the image %s", _path);
        return NULL;
    }

    return image;
}

void destroyImage(Image* _image)
{
    ASSERT_MSG(_image != NULL, "[Image] Internal error: parameter cannot be NULL");
    if (_image != NULL) {
        stbi_image_free(_image->buffer);
        free(_image);
    }
}
