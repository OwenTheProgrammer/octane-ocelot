#include "ocelot/engine/model.h"
#include "octane/ibuf.h"
#include "octane/vbuf.h"
#include <stdlib.h>

oce_model oce_load_octane_model(oct_rawDataDescriptor scene, uint32_t vstream_index, uint32_t istream_index)
{
    oce_model model = (oce_model){0};

    oct_vertexBuffer oct_vbuf = oct_decode_vertex_buffer(scene, vstream_index);

    model.vertex_count = oct_vbuf.vertex_count;

    //model.vertex_positions = oct_vbuf.positions;


    oct_indexBuffer oct_ibuf = oct_decode_index_buffer(scene, istream_index);

    model.index_count = oct_ibuf.index_count;

    model.indices = oct_ibuf.indices;

    return model;
}

void oce_free_model(oce_model* const model)
{
    if(model != NULL)
    {
        free(model->vertex_positions);
        free(model->indices);
    }
}
