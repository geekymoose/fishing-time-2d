#include "engine/path.h"

#include "engine/assertions.h"

#include <stdarg.h>
#include <string.h>

Path makePath(int _count, ...)
{
    Path path;
    memset(path.str, '\0', _PATH_MAX_SIXE);

    va_list ap;
    va_start(ap, _count);

    for (int k = 0; k < _count; ++k) {
        appendPath(&path, va_arg(ap, char*));
    }

    va_end(ap);

    return path;
}

void appendPath(Path* _dest, const char* _append)
{
    // ASSERT_MSG(_dest != NULL, "[Path] Invalid parameter");
    // ASSERT_MSG(_append != NULL, "[Path] Invalid parameter");
    ASSERT_MSG(_dest->str != _append, "[Path] Append string should not be the dest string");

    if (_dest == NULL || _append == NULL || _dest->str == _append) {
        return;
    }

    int sizeDest = strlen(_dest->str);

    // Exception case where _dest is empty
    if (sizeDest == 0) {
        int size = strlen(_append);
        size = size < _PATH_MAX_SIXE ? size : _PATH_MAX_SIXE - 1;
        strncat(_dest->str, _append, size);
        return;
    }

    // Exception case where _dest is already full
    if (sizeDest == _PATH_MAX_SIXE - 1) {
        return;
    }

    // Append / to the end of dest if not there
    if (_dest->str[sizeDest - 1] != '/') {
        strncat(_dest->str, "/", 2);
        sizeDest += 1;
    }

    // Bypass the / in _append because we already added it in _dest
    int offset = 0;
    if (_append[0] == '/') {
        offset = 1;
    }

    int sizeAppend = strlen(_append) - offset;
    sizeAppend = ((sizeDest + sizeAppend) < _PATH_MAX_SIXE) ? sizeAppend : (_PATH_MAX_SIXE - 1 - sizeDest);
    int finalSize = sizeDest + sizeAppend;
    sizeAppend = sizeAppend >= 0 ? sizeAppend : 0;

    strncat(_dest->str, _append + offset, sizeAppend);

    _dest->str[finalSize] = '\0';
}
