#include "byteconv/bswap.h"
#include "ocelot/dbuf.h"
#include "byteconv/endian.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* == READ FUNCTIONS == */

uint16_t ocl_dbuf_read_u16(ocl_dbuf* const buffer)
{
    // TODO: This is complaining about unaligned loads
    uint16_t value = *(uint16_t*)ocl_dbuf_pos(buffer);
    ocl_dbuf_advance(buffer, sizeof(uint16_t));

    return endian_swap_u16(value);
}

uint32_t ocl_dbuf_read_u32(ocl_dbuf* const buffer)
{
    uint32_t value = *(uint32_t*)ocl_dbuf_pos(buffer);
    ocl_dbuf_advance(buffer, sizeof(uint32_t));

    value = endian_swap_u32(value);
    return value;
}

uint32_t ocl_dbuf_read_uvar(ocl_dbuf* const buffer, unsigned int stride)
{
    uint32_t value = *(uint32_t*)ocl_dbuf_pos(buffer);
    ocl_dbuf_advance(buffer, stride);

    value = endian_swap_uvar(value, stride);
    return value;
}



uint8_t* ocl_dbuf_read_u8_array(ocl_dbuf* const buffer, size_t count)
{
    if(count == 0)
        return NULL;

    uint8_t* result = calloc(count, sizeof(uint8_t));
    memcpy(result, ocl_dbuf_pos(buffer), count);

    ocl_dbuf_advance(buffer, count);

    return result;
}


uint16_t* ocl_dbuf_read_u16_array(ocl_dbuf* const buffer, size_t count)
{
    if(count == 0)
        return NULL;

    uint16_t* result = calloc(count, sizeof(uint16_t));

    memcpy(result, ocl_dbuf_pos(buffer), count * sizeof(uint16_t));
    ocl_dbuf_advance(buffer, count * sizeof(uint16_t));

    if(endian_should_convert())
    {
        for(size_t i = 0; i < count; i++)
        {
            result[i] = bswap_u16(result[i]);
        }
    }

    return result;
}


uint32_t* ocl_dbuf_read_u32_array(ocl_dbuf* const buffer, size_t count)
{
    if(count == 0)
        return NULL;

    uint32_t* result = calloc(count, sizeof(uint32_t));

    memcpy(result, ocl_dbuf_pos(buffer), count * sizeof(uint32_t));
    ocl_dbuf_advance(buffer, count * sizeof(uint32_t));

    if(endian_should_convert())
    {
        for(size_t i = 0; i < count; i++)
        {
            result[i] = bswap_u32(result[i]);
        }
    }

    return result;
}

int32_t* ocl_dbuf_read_svar_array(ocl_dbuf* const buffer, size_t count, unsigned int stride)
{
    if(count == 0 || stride == 0)
        return NULL;

    int32_t* result = calloc(count, sizeof(int32_t));

    for(size_t i = 0; i < count; i++)
    {
        uint32_t value = *(uint32_t*)ocl_dbuf_pos(buffer);
        result[i] = endian_load_svar(value, stride);

        ocl_dbuf_advance(buffer, stride);
    }

    return result;
}


/* == WRITE FUNCTIONS == */


void ocl_dbuf_write_u16(ocl_dbuf* const buffer, uint16_t value)
{
    value = endian_swap_u16(value);
    ocl_dbuf_write_data(buffer, &value, sizeof(uint16_t), true);
}

void ocl_dbuf_write_u32(ocl_dbuf* const buffer, uint32_t value)
{
    value = endian_swap_u32(value);
    ocl_dbuf_write_data(buffer, &value, sizeof(uint32_t), true);
}


void ocl_dbuf_write_uvar(ocl_dbuf* const buffer, uint32_t value, unsigned int stride)
{
    value = endian_swap_uvar(value, stride);
    ocl_dbuf_write_data(buffer, &value, stride, true);
}


void ocl_dbuf_write_u8_array(ocl_dbuf* const buffer, uint8_t* const src, size_t count)
{
    ocl_dbuf_write_data(buffer, src, count, true);
}


void ocl_dbuf_write_u16_array(ocl_dbuf* const buffer, uint16_t* const src, size_t count)
{
    if(count == 0)
        return;

    if(!endian_should_convert())
    {
        ocl_dbuf_write_data(buffer, src, count * sizeof(uint16_t), true);
    }
    else
    {
        for(size_t i = 0; i < count; i++)
        {
            ocl_dbuf_write_u16(buffer, src[i]);
        }
    }
}


void ocl_dbuf_write_u32_array(ocl_dbuf* const buffer, uint32_t* const src, size_t count)
{
    if(count == 0)
        return;

    if(!endian_should_convert())
    {
        ocl_dbuf_write_data(buffer, src, count * sizeof(uint32_t), true);
    }
    else
    {
        for(size_t i = 0; i < count; i++)
        {
            ocl_dbuf_write_u32(buffer, src[i]);
        }
    }
}


void ocl_dbuf_write_uvar_array(ocl_dbuf* const buffer, uint32_t* const src, size_t count, unsigned int stride)
{
    if(count == 0)
        return;

    if(!endian_should_convert())
    {
        ocl_dbuf_write_data(buffer, src, count * stride, true);
    }
    else
    {
        for(size_t i = 0; i < count; i++)
        {
            ocl_dbuf_write_uvar(buffer, src[i], stride);
        }
    }
}
