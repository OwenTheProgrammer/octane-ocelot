#include "types/uuid.h"
#include <string.h>

void uuid_from_bytes(uuid_t dst, void* const src)
{
    memcpy(&dst[0], src, sizeof(uuid_t));
}
