#include "data/endian.h"
#include "octane/oct/oct.h"
#include "octane/oct/scene.h"
#include "octane/oct/scene_descriptor.h"
#include "platform.h"
#include <stdlib.h>

static int load_xbox()
{
    //Load the oct file
    ocl_set_platform(OCL_PLATFORM_TARGET_XBOX);
    oct_file oct = oct_load_file("bin/gamefiles/xbox360/worlds/oilrig/oilrig.oct");

    if(oct.header.endian == ENDIAN_UNKNOWN)
        return EXIT_FAILURE;

    //Serialize the oct file
    oct_sceneDescriptor desc = oct_parse_scene_tree(oct);

    //Load the scene data
    oct_sceneData scene = oct_load_scene_data(desc);

    //Unload the octane file data
    oct_free_scene_data(&scene);
    oct_free_scene_descriptor(&desc);
    oct_file_free(&oct);

    return EXIT_SUCCESS;
}

static int load_pc()
{
    //Load the oct file
    ocl_set_platform(OCL_PLATFORM_TARGET_PC);
    oct_file oct = oct_load_file("bin/gamefiles/pc/worlds/oilrig/oilrig.oct");

    if(oct.header.endian == ENDIAN_UNKNOWN)
        return EXIT_FAILURE;

    //Serialize the oct file
    oct_sceneDescriptor desc = oct_parse_scene_tree(oct);

    //Load the scene data itself
    oct_sceneData scene = oct_load_scene_data(desc);

    //Unload the octane file data
    oct_free_scene_data(&scene);
    oct_free_scene_descriptor(&desc);
    oct_file_free(&oct);

    return EXIT_SUCCESS;
}


int main()
{
    // WARN: Remember to change the ibuf and vbuf file references

    //return load_xbox();
    return load_pc();
}
