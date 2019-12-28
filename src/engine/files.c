#include "files.h"

#include "engine/log.h"
#include "engine/assertions.h"

#include <stdio.h>
#include <stdlib.h>


uint8 * readFileBuffer(const char * _path)
{
    ASSERT_MSG(_path != NULL, "[File] Invalid values: parameter should not be NULL");

    FILE * file;
    file = fopen(_path, "r");
    if(file == NULL)
    {
        LOG_ERR("[File] Unable to open the file %s", _path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);

    char * filebuffer = NULL;
    filebuffer = malloc((file_size + 1) * sizeof(*filebuffer)); // +1 for '\0'
    if(filebuffer == NULL)
    {
        LOG_ERR("[File] Unable to read %s (malloc %d bytes failed)", _path, filebuffer);
        return NULL;
    }

    size_t elementsRead = fread(filebuffer, sizeof(char), file_size, file);
    if(elementsRead != file_size)
    {
        LOG_ERR("[File] Unable to read %s", _path);
        free(filebuffer);
        return NULL;
    }

    filebuffer[file_size] = '\0';

    fclose(file);
    return filebuffer;
}

