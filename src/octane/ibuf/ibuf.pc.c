#include "octane/ibuf/ibuf.pc.h"

uint32_t pc_oct_ibuf_get_index_count(uint32_t length, uint32_t stride)
{
    /* On PC, the `IndexStream/Length` term refers to the
     * amount of elements are to be loaded.
     * Length=60 and stride=2 would take up 120 bytes of space.
     */
    return length;
}
