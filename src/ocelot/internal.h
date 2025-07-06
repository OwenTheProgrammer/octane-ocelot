#pragma once

#include "console/log.h"

#define LOG_INFO(_FMT, ...) OCL_LOG(LOG_SEVERITY_INFO, "ocelot", _FMT, ##__VA_ARGS__)
#define LOG_ERROR(_FMT, ...) OCL_LOG(LOG_SEVERITY_ERROR, "ocelot", _FMT, ##__VA_ARGS__)
#define LOG_FATAL(_FMT, ...) OCL_LOG(LOG_SEVERITY_FATAL, "ocelot", _FMT, ##__VA_ARGS__)
