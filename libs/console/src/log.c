#include "console/log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define _MAX_LOG_LEN 1024

static const char* _LOG_CODES[LOG_SEVERITY_MAX] =
{
    "[\x1b[32m%s|INFO\x1b[0m]: ",     // info as green
    "[\x1b[33m%s|WARN\x1b[0m]: ",     // warn as yellow
    "[\x1b[31m%s|ERROR\x1b[0m]: ",     // error as red
    "[\x1b[1;31m%s|FATAL\x1b[0m]: ",   // fatal as bolded red
};


void _ocl_log(log_severity sev, const char* scope, const char* format, ...)
{
    char buf[_MAX_LOG_LEN];
    memset(buf, 0, _MAX_LOG_LEN);

    const char* log_fmt = _LOG_CODES[(int)sev];

    int pre_count = snprintf(buf, _MAX_LOG_LEN, log_fmt, scope);

    va_list args;
    va_start(args, format);
    vsnprintf(buf + pre_count, _MAX_LOG_LEN - pre_count, format, args);
    va_end(args);

    FILE* stream;

    switch(sev)
    {
        case LOG_SEVERITY_INFO:
        case LOG_SEVERITY_WARN:
            stream = stdout;
            break;
        default:
        case LOG_SEVERITY_ERROR:
        case LOG_SEVERITY_FATAL:
            stream = stderr;
            break;
    }

    fputs(buf, stream);
    //fflush(stream);
}
