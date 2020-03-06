#include "assertions.h"

#include <stdarg.h>
#include <stdio.h>

void _printAssert(const char* _format, ...)
{
    va_list ap;
    va_start(ap, _format);

#if defined(_WIN32) || defined(_WIN64)
    fprintf(stderr, "[ASSERT][%s:%d]: ", __FILE__, __LINE__);
    vfprintf(stdout, _format, ap);
    fprintf(stdout, "\n");
#else
    fprintf(stderr, "\e[%dm[ASSERT][%s:%d]: ", 35, __FILE__, __LINE__); // 35 is xterm color red
    vfprintf(stdout, _format, ap);
    fprintf(stdout, "\e[0m\n");
#endif

    va_end(ap);
}
