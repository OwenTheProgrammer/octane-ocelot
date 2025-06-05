#pragma once
#include "octane/oct.h"
#include "octane/oct_scene.h"
#include <stdint.h>

/* == GLOBAL STRING TABLES == */

/* Print table of oct_atomType */
extern const char* _OCT_ATOM_TYPE_PRINT_TABLE[OCT_ATOM_TYPE_MAX];

/* Print table of oct_dataType */
extern const char* _OCT_DATA_TYPE_PRINT_TABLE[OCT_DATA_TYPE_MAX];

/* == HEX DISPLAYS == */

/* Prints the hex value of a uint8 value */
void phex_u8(uint8_t value);

/* Prints the hex value of a uint16 value */
void phex_u16(uint16_t value);

/* Prints the hex values of a uint8 array */
void phex_u8_array(void* const src, uint32_t count);

/* == OCTANE DISPLAYS == */

/* Prints the contents of an oct_header */
void oct_print_header(oct_header header);

/* Prints the contents of an oct_atomHeader */
void oct_print_atom_header(oct_atomHeader header);

/* Prints the contents of an oct_atomNode */
void oct_print_atom_node(oct_file oct, oct_atomNode node);

/* Prints the contents of the strings in the string table */
void oct_print_string_table(oct_file oct);

/* == OCTANE SCENE DISPLAYS == */

void oct_print_scene_descriptor(oct_file oct, oct_sceneDescriptor scene);

/* == OCTANE ATOM DISPLAYS == */

void oct_print_atom_name_table(oct_file oct, oct_atomNameTable ant);

void oct_print_atom_index_stream(oct_file oct, oct_indexStreamAtom atom);

void oct_print_atom_vertex_stream(oct_file oct, oct_vertexStreamAtom atom);

void oct_print_atom_vertex_element(oct_file oct, oct_vstreamElementAtom atom);

void oct_print_atom_scene_geometry(oct_file oct, oct_sceneGeometryNode atom);

void oct_print_atom_scene_sub_geometry(oct_file oct, oct_sceneSubGeometryNode atom);

void oct_print_atom_scene_sub_geometry_lit(oct_file oct, oct_sceneSubGeometryNode atom);

void oct_print_atom_scene_camera(oct_file oct, oct_sceneCameraNode atom);

void oct_print_atom_scene_light(oct_file oct, oct_sceneLightNode atom);

void oct_print_scene_tree_node(oct_file oct, oct_sceneTreeNodeAtom atom);
