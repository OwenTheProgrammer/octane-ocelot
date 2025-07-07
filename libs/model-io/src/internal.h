#pragma once

#include "console/log.h"

#define LOG_INFO(_FMT, ...) OCL_LOG(LOG_SEVERITY_INFO, "model-io", _FMT, ##__VA_ARGS__)
#define LOG_WARN(_FMT, ...) OCL_LOG(LOG_SEVERITY_WARN, "model-io", _FMT, ##__VA_ARGS__)
#define LOG_ERROR(_FMT, ...) OCL_LOG(LOG_SEVERITY_ERROR, "model-io", _FMT, ##__VA_ARGS__)
#define LOG_fATAL(_FMT, ...) OCL_LOG(LOG_SEVERITY_FATAL, "model-io", _FMT, ##__VA_ARGS__)
