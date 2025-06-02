#pragma once
#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include "octane/oct_scene.h"
#include <stdint.h>

oct_indexStreamAtom _oct_atom_read_index_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_vertexStreamAtom _oct_atom_read_vertex_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_vstreamElementAtom _oct_atom_read_vstream_element(oct_file oct, uint32_t start_idx, uint32_t end_idx);

oct_sceneTreeNodeAtom _oct_atom_read_scene_tree_node(oct_file oct, uint32_t start_idx, uint32_t end_idx);

void _oct_parse_index_stream_pool(oct_sceneDescriptor* const scene, oct_file oct);

void _oct_parse_vertex_stream_pool(oct_sceneDescriptor* const scene, oct_file oct);

void _oct_parse_scene_tree_node_pool(oct_sceneDescriptor* const scene, oct_file oct);
