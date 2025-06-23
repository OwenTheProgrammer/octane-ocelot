#include "ocelot/print_utils.h"
#include <stdio.h>

void oct_print_header(oct_fileHeader header)
{
    printf("Oct Header V%.2f:\n", header.version);
    printf("|\tEndian: %s\n", endian_to_string(header.endian));
    printf("|\tString table: %u bytes\n", header.string_table_size);
    printf("|\tData tree: %u bytes\n", header.data_tree_size);
}
