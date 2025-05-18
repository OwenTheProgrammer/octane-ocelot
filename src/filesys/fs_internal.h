#pragma once
#include "filesys/endian.h"
#include "filesys/file_io.h"
#include "filesys/oct.h"
#include <stdint.h>
#include <stddef.h>

/* Size of the oct file header section in bytes */
extern const size_t _OCT_HEADER_SIZE;

/* Little endian magic ID of an oct file */
extern const char _OCT_MAGIC_LE[8];

/* Big endian magic ID of an oct file */
extern const char _OCT_MAGIC_BE[8];


/* Determines the endianness of an oct files magic section */
endian_t _oct_get_magic_endian(void* magic);

/* Converts oct atom types into ocelots internal oct_nodeType */
oct_nodeType _oct_get_atom_node_type(io_dbuf* const buffer, oct_atomNode node);

/* Parses an oct atoms header flags section */
oct_atomHeader _oct_parse_atom_header(uint16_t data);
