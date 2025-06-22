#include "internal.h"
#include "octane/oct/enums.h"
#include "octane/oct/name_bindings.h"
#include <stdint.h>

/* Check if `name_key` matches the [S]cene [N]ode [T]ype*/
#define EVAL_SNT(NODE_TYPE, ELEM_TYPE) \
    if(name_key == _oct_aet.SceneTreeNodePool.Node.Type.NODE_TYPE)  \
        return OCT_SCENE_NODE_TYPE_##ELEM_TYPE;

/* Check if `name_key` matches the [S]cene [L]ight [T]ype */
#define EVAL_SLT(NODE_TYPE, ELEM_TYPE) \
    if(name_key == _oct_aet.SceneTreeNodePool.Node.LightType.NODE_TYPE) \
        return OCT_SCENE_LIGHT_TYPE_##ELEM_TYPE;

/* Check if `name_key` matches the [V]stream [E]lement [N]ame */
#define EVAL_VEN(OCT_TYPE, ELEM_TYPE) \
    if(name_key == _oct_aet.VertexStreamPool.VertexStream.Elements.Element.Name.OCT_TYPE)  \
        return OCL_VERTEX_ELEMENT_TYPE_##ELEM_TYPE;

/* Check if `name_key` matches the [P]rimitive [T]ype */
#define EVAL_PT(OCT_TYPE, ELEM_TYPE) \
    if(name_key == _oct_aet.IndexStreamPool.IndexStream.IndexStreamPrimitive.OCT_TYPE)  \
        return OCT_PRIMITIVE_TYPE_##ELEM_TYPE;

oct_sceneNodeType _scene_node_type_from_st_idx(uint16_t name_key)
{
    EVAL_SNT(Scene, SCENE)
    EVAL_SNT(Transform, TRANSFORM)
    EVAL_SNT(Geometry, GEOMETRY)
    EVAL_SNT(SubGeometry, SUB_GEOMETRY)
    EVAL_SNT(SubGeometryLit, SUB_GEOMETRY_LIT)
    EVAL_SNT(Camera, CAMERA)
    EVAL_SNT(Light, LIGHT)

    return OCT_SCENE_NODE_TYPE_NONE;
}

oct_sceneLightType _scene_light_type_from_st_idx(uint16_t name_key)
{
    EVAL_SLT(Directional, DIRECTIONAL)
    EVAL_SLT(Point, POINT)
    EVAL_SLT(Ambient, AMBIENT)

    return OCT_SCENE_LIGHT_TYPE_NONE;
}

ocl_vertexElementType _vstream_element_name_from_st_idx(uint16_t name_key)
{
    EVAL_VEN(Position, POSITION)
    EVAL_VEN(Uv1, UV1)
    EVAL_VEN(Tangent, TANGENT)
    EVAL_VEN(Normal, NORMAL)
    EVAL_VEN(Binormal, BINORMAL)
    EVAL_VEN(lightmapUV, LIGHTMAP_UV)
    EVAL_VEN(ColorAdditive, COLOR_ADD)
    EVAL_VEN(ColorOcclusion, COLOR_OCCL)
    EVAL_VEN(VertexBaked, VERTEX_BAKED)

    return OCL_VERTEX_ELEMENT_TYPE_NONE;
}

oct_primitiveType _primitive_type_from_st_idx(uint16_t name_key)
{
    EVAL_PT(TRIANGLES, TRIANGLES)

    return OCT_PRIMITIVE_TYPE_NONE;
}

#undef EVAL_SNT
#undef EVAL_SLT
#undef EVAL_VEN
#undef EVAL_PT


