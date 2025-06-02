#include "ocelot/dbuf.h"
#include "ocelot/engine/model.h"
#include "ocelot/engine/window.h"
#include "octane/oct.h"
#include "octane/oct_scene.h"
#include <stdio.h>

int main()
{

    //Load the oct file
    oct_file oct = oct_load_file("bin/oilrig.oct");
    oct_sceneDescriptor scene = oct_parse_scene_descriptor(oct);

    //Load the index and vertex buffer files
    ocl_dbuf ibuf_file = ocl_dbuf_load("bin/oilrig_0.ibuf");
    ocl_dbuf vbuf_file = ocl_dbuf_load("bin/oilrig_0.vbuf");

    oce_model model = oce_load_octane_model(ibuf_file, vbuf_file, scene, 2829, 1474, 2u);

    printf("VertexCount: %u\n", model.vertex_count);
    printf("IndexCount: %u\n", model.index_count);


    if(!ocl_gui_init())
        return -1;

    ocl_gui_loop(model);

    oce_free_model(&model);
    oct_free_scene_descriptor(&scene);
    oct_file_free(&oct);
    ocl_dbuf_free(&ibuf_file);
    ocl_dbuf_free(&vbuf_file);
    return 0;
}
