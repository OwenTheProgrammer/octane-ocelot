#pragma once

#ifndef __cplusplus

typedef enum log_severity   log_severity;

#endif

enum log_severity
{
    LOG_SEVERITY_INFO,
    LOG_SEVERITY_WARN,
    LOG_SEVERITY_ERROR,
    LOG_SEVERITY_FATAL,
    LOG_SEVERITY_MAX
};

void _ocl_log(log_severity sev, const char* scope, const char* format, ...);

#define OCL_LOG(_SEV, _SCOPE, _FMT, ...) _ocl_log(_SEV, _SCOPE, _FMT, ##__VA_ARGS__);

#define OCL_FILEPATH_FMT "\x1b[34m\"%s\"\x1b[0m"
