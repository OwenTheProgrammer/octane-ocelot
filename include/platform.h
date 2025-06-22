#pragma once
#include "data/endian.h"

#ifndef __cplusplus
typedef enum ocl_platformTarget ocl_platformTarget;
#endif

/* Implements which platform the ocelot runtimes should be targeting */
enum ocl_platformTarget
{
    OCL_PLATFORM_TARGET_PC      = 0,    /* Target the PC platform */
    OCL_PLATFORM_TARGET_XBOX    = 1,    /* Target the Xbox 360 platform */
    OCL_PLATFORM_TARGET_MAX
};

typedef struct ocl_platformInfo ocl_platformInfo;

struct ocl_platformInfo
{
    /* The target platform */
    ocl_platformTarget target_type;

    /* The endianness of the target platform */
    endian_t endian;
};

/* The global runtime platform */
extern ocl_platformInfo _platform;

void ocl_set_platform(ocl_platformTarget target);
