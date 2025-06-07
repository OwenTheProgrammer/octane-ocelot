#include "ocelot/endian.h"
#include "ocelot/engine/scene.h"
#include "octane/oct.h"
#include "octane/oct_scene.h"
#include <stdlib.h>

int main()
{
    //Load the oct file
    oct_file oct = oct_load_file("bin/xbfiles/oilrig/oilrig.oct");

    if(oct.header.endian == ENDIAN_UNKNOWN)
        return EXIT_FAILURE;

    oct_rawDataDescriptor raw_scene = oct_parse_raw_data_descriptor(oct);

    endian_set_current(ENDIAN_BIG);
    endian_set_target(ENDIAN_LITTLE);

    //Load the scene
    ocl_scene scene = ocl_load_oct_scene(raw_scene, oct);

    //Unload the octane file stuff
    oct_free_raw_data_descriptor(&raw_scene);
    oct_file_free(&oct);

    ocl_unload_scene(&scene);

    return EXIT_SUCCESS;
}
