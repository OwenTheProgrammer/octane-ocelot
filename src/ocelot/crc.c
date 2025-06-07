#include "types/crc.h"
#include <stdint.h>
#include <string.h>

/* Cant be fucked to implement something better
 * https://stackoverflow.com/a/27950866 */
uint32_t crc32_from_string(char* const str)
{
    //CRC-32 from ethernet, zip, etc in reversed bit order
    static const uint32_t POLY = 0xedb88320;

    uint32_t len = strlen(str);
    uint32_t rem = UINT32_MAX;

    for(uint32_t i = 0; i < len; i++)
    {
        rem ^= str[i];
        for(int j = 0; j < 8; j++)
        {
            uint32_t pmask = (rem & 1u) * POLY;
            rem = (rem >> 1u) ^ pmask;
        }
    }
    return ~rem;
}
