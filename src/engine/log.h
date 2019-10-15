#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>


// -----------------------------------------------------------------------------

enum
{
    _LOG_LEVEL_DBG,
    _LOG_LEVEL_INFO,
    _LOG_LEVEL_WARN,
    _LOG_LEVEL_ERR,
    _LOG_LEVEL_OMG,
};

enum
{
    _XTERM_COLOR_DEFAULT    = 0,
    _XTERM_COLOR_ORANGE     = 31,
    _XTERM_COLOR_GREEN      = 32,
    _XTERM_COLOR_YELLOW     = 33,
    _XTERM_COLOR_BLUE       = 34,
    _XTERM_COLOR_RED        = 35,
};


// -----------------------------------------------------------------------------
// Internal methods
// -----------------------------------------------------------------------------

// Returns a string representation of a log level
static inline const char* _logLevelHeader(const int level)
{
    switch(level)
    {
        case _LOG_LEVEL_DBG:
            return "[DBG] ";
        case _LOG_LEVEL_INFO:
            return "[INFO] ";
        case _LOG_LEVEL_WARN:
            return "[WARN] ";
        case _LOG_LEVEL_ERR:
            return "[ERR] ";
        default:
            return "[OMG] ";
    }
}

// Returns the terminal color for this log level
static inline int _logLevelColor(const int level)
{
    switch(level)
    {
        case _LOG_LEVEL_DBG:
            return _XTERM_COLOR_BLUE;
        case _LOG_LEVEL_INFO:
            return _XTERM_COLOR_YELLOW;
        case _LOG_LEVEL_WARN:
            return _XTERM_COLOR_ORANGE;
        case _LOG_LEVEL_ERR:
            return _XTERM_COLOR_RED;
        default:
            return _XTERM_COLOR_RED;
    }
}

// Prints a log
static inline void _log(const int level, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

#if defined(_WIN32) || defined(_WIN64)
    fprintf(stdout, "%s", _logLevelHeader(level));
#else
    fprintf(stdout, "\e[%dm%s\e[0m", _logLevelColor(level), _logLevelHeader(level));
#endif

    vfprintf(stdout, format, ap);
    fprintf(stdout, "\n");
    va_end(ap);
}


// -----------------------------------------------------------------------------
// Log macros
// -----------------------------------------------------------------------------

#if defined(ENGINE_ENABLE_LOG)

#define LOG_DBG(format, ...) _log(_LOG_LEVEL_DBG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) _log(_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) _log(_LOG_LEVEL_WARN, format, ##__VA_ARGS__)
#define LOG_ERR(format, ...) _log(_LOG_LEVEL_ERR, format, ##__VA_ARGS__)
#define LOG_OMG(format, ...) _log(_LOG_LEVEL_OMG, format, ##__VA_ARGS__)

#else

#define LOG_DBG(format, ...)
#define LOG_INFO(format, ...)
#define LOG_WARN(format, ...)
#define LOG_ERR(format, ...)
#define LOG_OMG(format, ...)

#endif


// -----------------------------------------------------------------------------
// Assert macros
// -----------------------------------------------------------------------------

#if defined(ENGINE_ENABLE_ASSERT)

#if defined(_WIN32) || defined(_WIN64)

/// Evaluate the given expression and assert if is false.
/// The msg is displayed if exp is false
#define ASSERT_MSG(exp, msg) \
    if(!(exp)) { \
        fprintf(stderr, "[ASSERT][%s:%d]: %s\n", __FILE__, __LINE__, msg); \
    } \
    assert(exp)
#else

#define ASSERT_MSG(exp, msg) \
    if(!(exp)) { \
        fprintf(stderr, "\e[%dm[ASSERT][%s:%d]: %s\e[0m\n", \
                _XTERM_COLOR_RED, __FILE__, __LINE__, msg); \
    } \
    assert(exp)
#endif

#else

#define ASSERT_MSG(exp, msg)

#endif

