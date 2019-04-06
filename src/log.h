#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#define SHARK_LOG(format, ...) _log(format, ##__VA_ARGS__)
#define SHARK_ASSERT(exp) assert(exp)

void _log(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stdout, format, ap);
    va_end(ap);
}


