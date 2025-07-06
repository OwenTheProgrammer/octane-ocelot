#pragma once
#include "console/log.h"

#define LOG_API_INFO(_API, _FMT, ...) OCL_LOG(LOG_SEVERITY_INFO, _API, _FMT, ##__VA_ARGS__)
#define LOG_API_WARN(_API, _FMT, ...) OCL_LOG(LOG_SEVERITY_WARN, _API, _FMT, ##__VA_ARGS__)
#define LOG_API_ERROR(_API, _FMT, ...) OCL_LOG(LOG_SEVERITY_ERROR, _API, _FMT, ##__VA_ARGS__)
#define LOG_API_FATAL(_API, _FMT, ...) OCL_LOG(LOG_SEVERITY_FATAL, _API, _FMT, ##__VA_ARGS__)

#define LOG_INFO(_FMT, ...) LOG_API_INFO("engine", _FMT, ##__VA_ARGS__)
#define LOG_WARN(_FMT, ...) LOG_API_WARN("engine", _FMT, ##__VA_ARGS__)
#define LOG_ERROR(_FMT, ...) LOG_API_ERROR("engine", _FMT, ##__VA_ARGS__)
#define LOG_FATAL(_FMT, ...) LOG_API_FATAL("engine", _FMT, ##__VA_ARGS__)


/* Returns 1 on success */
int _oce_init_glfw();


