#pragma once
#include "byteconv/endian.h"
#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include "octane/oct_scene.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

extern const size_t _OCT_MAGIC_SIZE;
extern const size_t _OCT_HEADER_SIZE;

extern const uint32_t _OCT_MAGIC_LE;
extern const uint32_t _OCT_MAGIC_BE;

endian_t _oct_magic_to_endian(uint32_t* const magic);

uint32_t _oct_endian_to_magic(endian_t endian);


oct_indexStreamAtom _oct_atom_read_index_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_vertexStreamAtom _oct_atom_read_vertex_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_vstreamElementAtom _oct_atom_read_vstream_element(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_sceneTreeNodeAtom _oct_atom_read_scene_tree_node(oct_file oct, uint32_t start_idx, uint32_t end_idx);

void _oct_parse_index_stream_pool(oct_rawDataDescriptor* const scene, oct_file oct);

void _oct_parse_vertex_stream_pool(oct_rawDataDescriptor* const scene, oct_file oct);

void _oct_parse_scene_tree_node_pool(oct_rawDataDescriptor* const scene, oct_file oct);
