#include "octane/oct/name_bindings.h"
#include <stdint.h>
#include <string.h>

oct_atomNameTable _oct_ant = (oct_atomNameTable){0};
oct_atomEnumTable _oct_aet = (oct_atomEnumTable){0};

/* Gets the string table index of a target string in an oct files string table.
 * Returns 0 if the string can not be found. */
static uint16_t _get_st_index_of(oct_file oct, const char* target)
{
    //Get the length of the string
    uint32_t len = strlen(target);

    for(uint32_t i = 0; i < oct.string_table_length; i++)
    {
        oct_string elem = oct.string_table[i];

        if(elem.len == len && strncmp(target, elem.data, elem.len) == 0)
            return (uint16_t)i;
    }

    //Empty string index when not found
    return 0;
}

/* Converts a macro object into a "string" literal */
#define STRINGIFY(x) #x

/* Loads the string index of _TYPE into _BASE._TYPE */
#define LOAD_TYPE(_BASE, _TYPE) table._BASE._TYPE = _get_st_index_of(oct, STRINGIFY(_TYPE))

/* Loads the top-level tree header of node _BASE into its _header field */
#define LOAD_TREE_HEADER(_BASE) table._BASE._header = _get_st_index_of(oct, STRINGIFY(_BASE));

/* Loads the _header section of _BASE._TYPE with the header name _TYPE */
#define LOAD_SUB_HEADER(_BASE, _TYPE) table._BASE._TYPE._header = _get_st_index_of(oct, STRINGIFY(_TYPE))

/* Loads the type _TYPE from the preloaded field _SRC into _DST */
#define REF_TYPE(_DST, _SRC, _TYPE) table._DST._TYPE = table._SRC._TYPE

/* Loads the _common._TYPE value into _BASE._TYPE */
#define REF_COMMON(_BASE, _TYPE) REF_TYPE(_BASE, _common, _TYPE)

/* Load the atom name table elements we need from an oct file */
static oct_atomNameTable _load_ant(oct_file oct)
{
    oct_atomNameTable table = (oct_atomNameTable){0};

    LOAD_TYPE(_common, Name);
    LOAD_TYPE(_common, Flags);
    LOAD_TYPE(_common, FileName);
    LOAD_TYPE(_common, Width);
    LOAD_TYPE(_common, Size);
    LOAD_TYPE(_common, Length);
    LOAD_TYPE(_common, Type);

    /* VertexBufferPool VertexBuffer */
    LOAD_TREE_HEADER(VertexBufferPool);
    LOAD_SUB_HEADER(VertexBufferPool, VertexBuffer);
    REF_COMMON(VertexBufferPool.VertexBuffer, Name);
    REF_COMMON(VertexBufferPool.VertexBuffer, Flags);
    REF_COMMON(VertexBufferPool.VertexBuffer, Size);
    LOAD_TYPE(VertexBufferPool.VertexBuffer, HeapLoc);
    REF_COMMON(VertexBufferPool.VertexBuffer, FileName);

    /* IndexBufferPool IndexBuffer */
    LOAD_TREE_HEADER(IndexBufferPool);
    LOAD_SUB_HEADER(IndexBufferPool, IndexBuffer);
    REF_COMMON(IndexBufferPool.IndexBuffer, Width);
    REF_COMMON(IndexBufferPool.IndexBuffer, Name);
    REF_COMMON(IndexBufferPool.IndexBuffer, Flags);
    REF_COMMON(IndexBufferPool.IndexBuffer, Size);
    REF_COMMON(IndexBufferPool.IndexBuffer, FileName);

    /* IndexStreamPool IndexStream */
    LOAD_TREE_HEADER(IndexStreamPool);
    LOAD_SUB_HEADER(IndexStreamPool, IndexStream);
    REF_COMMON(IndexStreamPool.IndexStream, Length);
    LOAD_TYPE(IndexStreamPool.IndexStream, IndexBufferReference);
    LOAD_TYPE(IndexStreamPool.IndexStream, IndexBufferOffset);
    LOAD_TYPE(IndexStreamPool.IndexStream, IndexStreamElement);

    /* VertexStreamPool VertexStream Elements Element */
    LOAD_TREE_HEADER(VertexStreamPool);
    LOAD_SUB_HEADER(VertexStreamPool, VertexStream);
    REF_COMMON(VertexStreamPool.VertexStream, Length);
    REF_COMMON(VertexStreamPool.VertexStream, Width);
    LOAD_TYPE(VertexStreamPool.VertexStream, ExtraStride);
    LOAD_TYPE(VertexStreamPool.VertexStream, VertexBufferReference);
    LOAD_TYPE(VertexStreamPool.VertexStream, VertexBufferOffset);

    LOAD_SUB_HEADER(VertexStreamPool.VertexStream, Elements);
    LOAD_SUB_HEADER(VertexStreamPool.VertexStream.Elements, Element);
    REF_COMMON(VertexStreamPool.VertexStream.Elements.Element, Type);
    REF_COMMON(VertexStreamPool.VertexStream.Elements.Element, Name);
    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element, Offset);

    /* SceneTreeNodePool Node StaticKeyValuePairs */
    LOAD_TREE_HEADER(SceneTreeNodePool);
    LOAD_SUB_HEADER(SceneTreeNodePool, Node);
    REF_COMMON(SceneTreeNodePool.Node, Type);
    LOAD_TYPE(SceneTreeNodePool.Node, NodeName);
    LOAD_TYPE(SceneTreeNodePool.Node, Uuid);
    LOAD_TYPE(SceneTreeNodePool.Node, DisplayLayer);
    LOAD_TYPE(SceneTreeNodePool.Node, ParentNodeReferences);
    LOAD_TYPE(SceneTreeNodePool.Node, LocalToParentMatrix);
    LOAD_TYPE(SceneTreeNodePool.Node, Visible);
    LOAD_TYPE(SceneTreeNodePool.Node, DynamicVisPlacement);
    LOAD_TYPE(SceneTreeNodePool.Node, MeshName);
    LOAD_TYPE(SceneTreeNodePool.Node, MaterialReference);
    LOAD_TYPE(SceneTreeNodePool.Node, VertexStreamReferences);
    LOAD_TYPE(SceneTreeNodePool.Node, IndexStreamReference);
    LOAD_TYPE(SceneTreeNodePool.Node, IsOrthographic);
    LOAD_TYPE(SceneTreeNodePool.Node, FocalLength);
    LOAD_TYPE(SceneTreeNodePool.Node, CameraScale);
    LOAD_TYPE(SceneTreeNodePool.Node, NearClipPlane);
    LOAD_TYPE(SceneTreeNodePool.Node, FarClipPlane);

    //LOAD_TYPE(SceneTreeNodePool.Node, HorizontalFilmAperture);
    _oct_ant.SceneTreeNodePool.Node.HorizontalFilmAperture = _get_st_index_of(oct, "HorizontalFilmAmperture");

    LOAD_TYPE(SceneTreeNodePool.Node, VerticalFilmAperture);
    LOAD_TYPE(SceneTreeNodePool.Node, LensSqueezeRatio);
    LOAD_TYPE(SceneTreeNodePool.Node, LightType);
    LOAD_TYPE(SceneTreeNodePool.Node, LightColor);
    LOAD_TYPE(SceneTreeNodePool.Node, LightIntensity);
    LOAD_TYPE(SceneTreeNodePool.Node, AmbientShade);
    LOAD_TYPE(SceneTreeNodePool.Node, ShadowColor);
    LOAD_TYPE(SceneTreeNodePool.Node, DecayRate);
    LOAD_TYPE(SceneTreeNodePool.Node, ExclusionType);

    LOAD_SUB_HEADER(SceneTreeNodePool.Node, StaticKeyValuePairs);
    LOAD_TYPE(SceneTreeNodePool.Node.StaticKeyValuePairs, FalloffRadius);
    LOAD_TYPE(SceneTreeNodePool.Node.StaticKeyValuePairs, Softness);
    LOAD_TYPE(SceneTreeNodePool.Node.StaticKeyValuePairs, IsRoaming);
    LOAD_TYPE(SceneTreeNodePool.Node.StaticKeyValuePairs, UseTertiary);
    LOAD_TYPE(SceneTreeNodePool.Node.StaticKeyValuePairs, IsNegative);
    LOAD_TYPE(SceneTreeNodePool.Node.StaticKeyValuePairs, LightUsageLightmapNormal);
    LOAD_TYPE(SceneTreeNodePool.Node.StaticKeyValuePairs, LightUsageLightmapSpecular);

    return table;
}

static oct_atomEnumTable _load_aet(oct_file oct)
{
    oct_atomEnumTable table = (oct_atomEnumTable){0};

    LOAD_TYPE(Bool, False);
    LOAD_TYPE(Bool, True);

    LOAD_TYPE(VertexBufferPool.VertexBuffer.Name, Static);
    LOAD_TYPE(VertexBufferPool.VertexBuffer.Name, Dynamic);

    REF_TYPE(IndexBufferPool.IndexBuffer.Name, VertexBufferPool.VertexBuffer.Name, Static);
    REF_TYPE(IndexBufferPool.IndexBuffer.Name, VertexBufferPool.VertexBuffer.Name, Dynamic);

    LOAD_TYPE(IndexStreamPool.IndexStream.IndexStreamPrimitive, TRIANGLES);

    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element.Name, Position);
    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element.Name, Uv1);
    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element.Name, Tangent);
    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element.Name, Normal);
    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element.Name, Binormal);
    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element.Name, lightmapUV);
    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element.Name, ColorAdditive);
    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element.Name, ColorOcclusion);
    LOAD_TYPE(VertexStreamPool.VertexStream.Elements.Element.Name, VertexBaked);

    LOAD_TYPE(SceneTreeNodePool.Node.Type, Scene);
    LOAD_TYPE(SceneTreeNodePool.Node.Type, Transform);
    LOAD_TYPE(SceneTreeNodePool.Node.Type, Geometry);
    LOAD_TYPE(SceneTreeNodePool.Node.Type, SubGeometry);
    LOAD_TYPE(SceneTreeNodePool.Node.Type, SubGeometryLit);
    LOAD_TYPE(SceneTreeNodePool.Node.Type, Camera);
    LOAD_TYPE(SceneTreeNodePool.Node.Type, Light);

    LOAD_TYPE(SceneTreeNodePool.Node.LightType, Directional);
    LOAD_TYPE(SceneTreeNodePool.Node.LightType, Point);
    LOAD_TYPE(SceneTreeNodePool.Node.LightType, Ambient);

    return table;
}

#undef STRINGIFY
#undef LOAD_TYPE
#undef LOAD_TREE_HEADER
#undef LOAD_SUB_HEADER
#undef REF_TYPE
#undef REF_COMMON

void oct_load_name_bindings(oct_file oct)
{
    _oct_ant = _load_ant(oct);
    _oct_aet = _load_aet(oct);
}
