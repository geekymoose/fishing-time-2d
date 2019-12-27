#include "log.h"

#include <stdarg.h>
#include <stdio.h>

enum
{
    _XTERM_COLOR_DEFAULT    = 0,
    _XTERM_COLOR_ORANGE     = 31,
    _XTERM_COLOR_GREEN      = 32,
    _XTERM_COLOR_YELLOW     = 33,
    _XTERM_COLOR_BLUE       = 34,
    _XTERM_COLOR_RED        = 35,
};


// Returns a string representation of a log level
static inline const char * _logLevelHeader(const int _level)
{
    switch(_level)
    {
        case _LOG_LEVEL_DBG:
            return "[DBG]";
        case _LOG_LEVEL_INFO:
            return "[INFO]";
        case _LOG_LEVEL_WARN:
            return "[WARN]";
        case _LOG_LEVEL_ERR:
            return "[ERR]";
        default:
            return "[OMG]";
    }
}

// Returns the terminal color for this log level
static inline int _logLevelColor(const int _level)
{
    switch(_level)
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

void _log(const int _level, const char * _format, ...)
{
    va_list ap;
    va_start(ap, _format);

#if defined(_WIN32) || defined(_WIN64)
    fprintf(stdout, "%s ", _logLevelHeader(_level));
#else
    fprintf(stdout, "\e[%dm%s\e[0m ", _logLevelColor(_level), _logLevelHeader(_level));
#endif

    vfprintf(stdout, _format, ap);
    fprintf(stdout, "\n");
    va_end(ap);
}
