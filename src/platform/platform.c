#include "platform.h"
#include "data/endian.h"
#include "octane/ibuf/ibuf.pc.h"
#include "octane/ibuf/ibuf.xbox.h"
#include <stdio.h>

/* The current runtime platform target */
static ocl_platformTarget _target = OCL_PLATFORM_TARGET_PC;

static const ocl_platformInfo _pinf_pc =
{
    .target_type = OCL_PLATFORM_TARGET_PC,
    .endian = ENDIAN_LITTLE,

    .pfn_ibuf =
    {
        .get_index_count = pc_oct_ibuf_get_index_count
    }
};

static const ocl_platformInfo _pinf_xbox360 =
{
    .target_type = OCL_PLATFORM_TARGET_XBOX,
    .endian = ENDIAN_BIG,

    .pfn_ibuf =
    {
        .get_index_count = xb_oct_ibuf_get_index_count
    }
};

static ocl_platformInfo _PLATFORM_TABLE[OCL_PLATFORM_TARGET_MAX] =
{
    _pinf_pc,
    _pinf_xbox360
};
static const char* _PLATFORM_NAME_TABLE[OCL_PLATFORM_TARGET_MAX] =
{
    "PC",
    "Xbox360"
};

ocl_platformInfo _platform = _pinf_pc;

void ocl_set_platform(ocl_platformTarget target)
{
    _target = target;
    _platform = _PLATFORM_TABLE[target];

    printf("Changed platform target: %s\n", _PLATFORM_NAME_TABLE[target]);
}
