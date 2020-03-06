#pragma once

#if defined(ENGINE_ENABLE_LOG)

enum
{
    _LOG_LEVEL_DBG,
    _LOG_LEVEL_INFO,
    _LOG_LEVEL_WARN,
    _LOG_LEVEL_ERR,
    _LOG_LEVEL_OMG,
};

void _log(const int _level, const char* _format, ...);

#define LOG_DBG(_format, ...) _log(_LOG_LEVEL_DBG, _format, ##__VA_ARGS__)
#define LOG_INFO(_format, ...) _log(_LOG_LEVEL_INFO, _format, ##__VA_ARGS__)
#define LOG_WARN(_format, ...) _log(_LOG_LEVEL_WARN, _format, ##__VA_ARGS__)
#define LOG_ERR(_format, ...) _log(_LOG_LEVEL_ERR, _format, ##__VA_ARGS__)
#define LOG_OMG(_format, ...) _log(_LOG_LEVEL_OMG, _format, ##__VA_ARGS__)

#else

#define LOG_DBG(_format, ...)
#define LOG_INFO(_format, ...)
#define LOG_WARN(_format, ...)
#define LOG_ERR(_format, ...)
#define LOG_OMG(_format, ...)

#endif
