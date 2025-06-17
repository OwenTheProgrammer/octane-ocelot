#pragma once
#include "byteconv/endian.h"
#include "octane/oct_atoms.h"
#include "octane/oct.h"
#include <stdint.h>
#include <stddef.h>

extern const size_t _OCT_MAGIC_SIZE;
extern const size_t _OCT_HEADER_SIZE;

extern const uint32_t _OCT_MAGIC_LE;
extern const uint32_t _OCT_MAGIC_BE;

endian_t _oct_magic_to_endian(uint32_t* const magic);

uint32_t _oct_endian_to_magic(endian_t endian);

/* == oct_parseEnum.c == */

oct_sceneNodeType _scene_node_type_from_st_idx(uint16_t name_key);

oct_sceneLightType _scene_light_type_from_st_idx(uint16_t name_key);

oct_vstreamElementName _vstream_element_name_from_st_idx(uint16_t name_key);

oct_primitiveType _primitive_type_from_st_idx(uint16_t name_key);

/* == oct_parseAtom.c == */

oct_vertexBufferAtom _oct_atom_read_vertex_buffer(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_indexBufferAtom _oct_atom_read_index_buffer(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_indexStreamAtom _oct_atom_read_index_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_vertexStreamAtom _oct_atom_read_vertex_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_vstreamElementAtom _oct_atom_read_vstream_element(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_sceneTreeNodeAtom _oct_atom_read_scene_tree_node(oct_file oct, uint32_t start_idx, uint32_t end_idx);

/* == oct_parseScene.c == */

uint32_t* _load_cache_hierarchy_indexed(uint32_t start_idx, uint16_t target_node, uint32_t* const node_count, oct_file oct);

uint32_t* _load_cache_hierarchy_named(uint16_t root_node, uint16_t target_node, uint32_t* const node_count, oct_file oct);