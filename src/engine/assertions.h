#pragma once

#if defined(ENGINE_ENABLE_ASSERT)

#include <assert.h>


void _printAssert(const char * _format, ...);

/// Evaluate the given expression and assert (+ msg) if false.
#define ASSERT_MSG(_exp, _msg, ...) \
    if(!(_exp)) { _printAssert(_msg, ##__VA_ARGS__); } \
    assert(_exp)


// -----------------------------------------------------------------------------

#else // Assertions disabled

#define ASSERT_MSG(_exp, _msg, ...)

#endif

