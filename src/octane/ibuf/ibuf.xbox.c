#include "octane/ibuf/ibuf.xbox.h"
#include <stdint.h>

uint32_t xb_oct_ibuf_get_index_count(uint32_t length, uint32_t stride)
{
    /* On Xbox, the `IndexStream/Length` term refers to the
     * amount of machine wide integers (4 bytes) are to be loaded.
     * Length=60 will take up 240 bytes of space.
     */
    return length * (sizeof(uint32_t) / stride);
}
