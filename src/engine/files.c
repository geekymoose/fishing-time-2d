#include "files.h"

#include "engine/assertions.h"
#include "engine/log.h"

#include <stdio.h>
#include <stdlib.h>

void* readFileContent(const char* _path)
{
    ASSERT_MSG(_path != NULL, "[File] Invalid values: parameter should not be NULL");

    FILE* file;
    file = fopen(_path, "r");
    if (file == NULL) {
        LOG_ERR("[File] Unable to open the file %s", _path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    const size_t file_size = ftell(file);
    rewind(file);

    char* filebuffer = NULL;
    filebuffer = calloc(file_size + 1, sizeof(*filebuffer)); // +1 for '\0'
    if (filebuffer == NULL) {
        LOG_ERR("[File] Unable to read %s (malloc %d bytes failed)", _path, filebuffer);
        return NULL;
    }

    size_t elementsRead = fread(filebuffer, sizeof(*filebuffer), file_size, file);
    if (elementsRead != file_size) {
        LOG_ERR("[File] Unable to read %s", _path);
        free(filebuffer);
        return NULL;
    }

    filebuffer[file_size] = '\0'; // In case of (should already be '\0')

    fclose(file);
    return (void*)filebuffer;
}
