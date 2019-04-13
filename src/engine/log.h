#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>


// -----------------------------------------------------------------------------
// Constants
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
    _XTERM_COLOR_WHITE = 0,
    _XTERM_COLOR_RED = 31,
    _XTERM_COLOR_GREEN = 32,
    _XTERM_COLOR_YELLOW = 33,
};


// -----------------------------------------------------------------------------
// Log methods
// -----------------------------------------------------------------------------

// Returns a string representation of a log level
static inline const char* _logLevelHeader(const int level)
{
    switch(level)
    {
        case _LOG_LEVEL_DBG:
            return "[LOG DBG]: ";
        case _LOG_LEVEL_INFO:
            return "[LOG INFO]: ";
        case _LOG_LEVEL_WARN:
            return "[LOG WARN]: ";
        case _LOG_LEVEL_ERR:
            return "[LOG ERR]: ";
        default:
            return "[LOG OMG]: ";
    }
}

// Returns a the terminal color for a log level
static inline int _logLevelColor(const int level)
{
    switch(level)
    {
        case _LOG_LEVEL_DBG:
            return _XTERM_COLOR_WHITE;
        case _LOG_LEVEL_INFO:
            return _XTERM_COLOR_WHITE;
        case _LOG_LEVEL_WARN:
            return _XTERM_COLOR_YELLOW;
        case _LOG_LEVEL_ERR:
            return _XTERM_COLOR_RED;
        default:
            return _XTERM_COLOR_RED;
    }
}

// Print a log
static inline void _log(const int level, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    fprintf(stdout, "\e[%dm%s\e[0m", _logLevelColor(level), _logLevelHeader(level));
    vfprintf(stdout, format, ap);
    va_end(ap);
}


// -----------------------------------------------------------------------------
// Log macros
// -----------------------------------------------------------------------------

#define LOG_DBG(format, ...) _log(_LOG_LEVEL_DBG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) _log(_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) _log(_LOG_LEVEL_WARN, format, ##__VA_ARGS__)
#define LOG_ERR(format, ...) _log(_LOG_LEVEL_ERR, format, ##__VA_ARGS__)
#define LOG_OMG(format, ...) _log(_LOG_LEVEL_OMG, format, ##__VA_ARGS__)


// -----------------------------------------------------------------------------
// Assert macros
// -----------------------------------------------------------------------------

/// Evaluate the given expression and assert if is false.
/// The msg is displayed if exp is false
#define ASSERT_MSG(exp, msg) \
    if(!(exp)) { \
        fprintf(stderr, "\e[%dm[ASSERT][%s:%d]: %s\e[0m\n", \
                _XTERM_COLOR_RED, __FILE__, __LINE__, msg); \
    } \
    assert(exp)


