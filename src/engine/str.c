#include "str.h"

#include "engine/assertions.h"
#include "engine/log.h"

#include <string.h>

void concatStrings(char* _dest, const size_t _destBufferSize, const char* _str1, const char* _str2)
{
    ASSERT_MSG(_dest != NULL, "Invalid parameter: destination string should not be null");
    ASSERT_MSG(_str1 != NULL, "Invalid parameter: source string should not be null");
    ASSERT_MSG(_str2 != NULL, "Invalid parameter: source string should not be null");
    ASSERT_MSG(_destBufferSize > 0, "Invalid parameter: destination string size is not valid");

    if (_dest == NULL || _str1 == NULL || _str2 == NULL) {
        LOG_ERR("Unable to concat string (NULL given)");
        return;
    } else if (_destBufferSize <= 0) {
        LOG_ERR("Unable to concat string (invalid size given)");
        return;
    }

    strncpy(_dest, _str1, _destBufferSize - 1); // Size includes '\0'

    int sizeRemains = _destBufferSize - strlen(_dest) - 1; // -1 to account the '\0'
    if (sizeRemains > 0) {
        strncat(_dest, _str2, sizeRemains);
    }
}
