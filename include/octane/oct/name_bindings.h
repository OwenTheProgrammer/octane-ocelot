#pragma once
#include "octane/oct/oct.h"
#include <stdint.h>

typedef struct oct_atomNameTable    oct_atomNameTable;
typedef struct oct_atomEnumTable    oct_atomEnumTable;

struct oct_atomNameTable
{
    struct
    {
        /* Name */
        uint16_t Name;

        /* Flags */
        uint16_t Flags;

        /* FileName */
        uint16_t FileName;

        /* Width */
        uint16_t Width;

        /* Size */
        uint16_t Size;

        /* Length */
        uint16_t Length;

        /* Type */
        uint16_t Type;

    } _common;

    struct
    {
        /* VertexBufferPool */
        uint16_t _header;

        struct
        {
            /* VertexBufferPool/VertexBuffer */
            uint16_t _header;

            /* VertexBufferPool/VertexBuffer/Name */
            uint16_t Name;

            /* VertexBufferPool/VertexBuffer/Flags */
            uint16_t Flags;

            /* VertexBufferPool/VertexBuffer/Size */
            uint16_t Size;

            /* VertexBufferPool/VertexBuffer/HeapLoc */
            uint16_t HeapLoc;

            /* VertexBufferPool/VertexBuffer/FileName */
            uint16_t FileName;

        } VertexBuffer;

    } VertexBufferPool;

    struct
    {
        /* IndexBufferPool */
        uint16_t _header;

        struct
        {
            /* IndexBufferPool/IndexBuffer */
            uint16_t _header;

            /* IndexBufferPool/IndexBuffer/Width */
            uint16_t Width;

            /* IndexBufferPool/IndexBuffer/Name */
            uint16_t Name;

            /* IndexBufferPool/IndexBuffer/Flags */
            uint16_t Flags;

            /* IndexBufferPool/IndexBuffer/Size */
            uint16_t Size;

            /* IndexBufferPool/IndexBuffer/FileName */
            uint16_t FileName;

        } IndexBuffer;

    } IndexBufferPool;

    struct
    {
        /* IndexStreamPool */
        uint16_t _header;

        struct
        {
            /* IndexStreamPool/IndexStream/ */
            uint16_t _header;

            /* IndexStreamPool/IndexStream/Length */
            uint16_t Length;

            /* IndexStreamPool/IndexStream/IndexBufferReference */
            uint16_t IndexBufferReference;

            /* IndexStreamPool/IndexStream/IndexBufferOffset */
            uint16_t IndexBufferOffset;

            /* IndexStreamPool/IndexStream/IndexStreamPrimitive */
            uint16_t IndexStreamPrimitive;

            /* IndexStreamPool/IndexStream/IndexStreamElement */
            uint16_t IndexStreamElement;

        } IndexStream;

    } IndexStreamPool;

    struct
    {
        /* VertexStreamPool */
        uint16_t _header;

        struct
        {
            /* VertexStreamPool/VertexStream */
            uint16_t _header;

            /* VertexStreamPool/VertexStream/Length */
            uint16_t Length;

            /* VertexStreamPool/VertexStream/Width */
            uint16_t Width;

            /* VertexStreamPool/VertexStream/ExtraStride */
            uint16_t ExtraStride;

            /* VertexStreamPool/VertexStream/VertexBufferReference */
            uint16_t VertexBufferReference;

            /* VertexStreamPool/VertexStream/VertexBufferOffset */
            uint16_t VertexBufferOffset;

            struct
            {
                /* VertexStreamPool/VertexStream/Elements */
                uint16_t _header;

                struct
                {
                    /* VertexStreamPool/VertexStream/Elements/Element */
                    uint16_t _header;

                    /* VertexStreamPool/VertexStream/Elements/Element/Type */
                    uint16_t Type;

                    /* VertexStreamPool/VertexStream/Elements/Element/Name */
                    uint16_t Name;

                    /* VertexStreamPool/VertexStream/Elements/Element/Offset */
                    uint16_t Offset;

                } Element;

            } Elements;

        } VertexStream;

    } VertexStreamPool;

    struct
    {
        /* SceneTreeNodePool */
        uint16_t _header;

        struct
        {
            /* SceneTreeNodePool/Node */
            uint16_t _header;

            /* SceneTreeNodePool/Node/Type */
            uint16_t Type;

            /* SceneTreeNodePool/Node/NodeName */
            uint16_t NodeName;

            /* SceneTreeNodePool/Node/Uuid */
            uint16_t Uuid;

            /* SceneTreeNodePool/Node/DisplayLayer */
            uint16_t DisplayLayer;

            /* SceneTreeNodePool/Node/ParentNodeReferences */
            uint16_t ParentNodeReferences;

            /* SceneTreeNodePool/Node/LocalToParentMatrix */
            uint16_t LocalToParentMatrix;

            /* SceneTreeNodePool/Node/Visible */
            uint16_t Visible;

            /* SceneTreeNodePool/Node/DynamicVisPlacement */
            uint16_t DynamicVisPlacement;

            /* SceneTreeNodePool/Node/MeshName */
            uint16_t MeshName;

            /* SceneTreeNodePool/Node/MaterialReference */
            uint16_t MaterialReference;

            /* SceneTreeNodePool/Node/VertexStreamReferences */
            uint16_t VertexStreamReferences;

            /* SceneTreeNodePool/Node/IndexStreamReference */
            uint16_t IndexStreamReference;

            /* SceneTreeNodePool/Node/IsOrthographic */
            uint16_t IsOrthographic;

            /* SceneTreeNodePool/Node/FocalLength */
            uint16_t FocalLength;

            /* SceneTreeNodePool/Node/CameraScale */
            uint16_t CameraScale;

            /* SceneTreeNodePool/Node/NearClipPlane */
            uint16_t NearClipPlane;

            /* SceneTreeNodePool/Node/FarClipPlane */
            uint16_t FarClipPlane;

            // NOTE: The octane file format has the typo for "aperture"
            /* SceneTreeNodePool/Node/HorizontalFilmAmperture */
            uint16_t HorizontalFilmAperture;

            /* SceneTreeNodePool/Node/VerticalFilmAperture */
            uint16_t VerticalFilmAperture;

            /* SceneTreeNodePool/Node/LensSqueezeRatio */
            uint16_t LensSqueezeRatio;

            /* SceneTreeNodePool/Node/LightType */
            uint16_t LightType;

            /* SceneTreeNodePool/Node/LightColor */
            uint16_t LightColor;

            /* SceneTreeNodePool/Node/LightIntensity */
            uint16_t LightIntensity;

            /* SceneTreeNodePool/Node/AmbientShade */
            uint16_t AmbientShade;

            /* SceneTreeNodePool/Node/ShadowColor */
            uint16_t ShadowColor;

            /* SceneTreeNodePool/Node/DecayRate */
            uint16_t DecayRate;

            /* SceneTreeNodePool/Node/ExclusionType */
            uint16_t ExclusionType;

            struct
            {
                /* SceneTreeNodePool/Node/StaticKeyValuePairs */
                uint16_t _header;

                /* SceneTreeNodePool/Node/StaticKeyValuePairs/FalloffRadius */
                uint16_t FalloffRadius;

                /* SceneTreeNodePool/Node/StaticKeyValuePairs/Softness */
                uint16_t Softness;

                /* SceneTreeNodePool/Node/StaticKeyValuePairs/IsRoaming */
                uint16_t IsRoaming;

                /* SceneTreeNodePool/Node/StaticKeyValuePairs/UseTertiary */
                uint16_t UseTertiary;

                /* SceneTreeNodePool/Node/StaticKeyValuePairs/IsNegative */
                uint16_t IsNegative;

                /* SceneTreeNodePool/Node/StaticKeyValuePairs/LightUsageLightmapNormal */
                uint16_t LightUsageLightmapNormal;

                /* SceneTreeNodePool/Node/StaticKeyValuePairs/LightUsageLightmapSpecular */
                uint16_t LightUsageLightmapSpecular;

            } StaticKeyValuePairs;

        } Node;

    } SceneTreeNodePool;
};

/* Global [A]tom [N]ame [T]able for the current oct file context */
extern oct_atomNameTable _oct_ant;

struct oct_atomEnumTable
{
    struct
    {
        /* Bool:False */
        uint16_t False;

        /* Bool:True */
        uint16_t True;

    } Bool;

    struct
    {
        struct
        {
            struct
            {
                /* VertexBufferPool/VertexBuffer/Name:Static */
                uint16_t Static;

                /* VertexBufferPool/VertexBuffer/Name:Dynamic */
                uint16_t Dynamic;

            } Name;

        } VertexBuffer;

    } VertexBufferPool;

    struct
    {
        struct
        {
            struct
            {
                /* IndexBufferPool/IndexBuffer/Name:Static */
                uint16_t Static;

                /* IndexBufferPool/IndexBuffer/Name:Dynamic */
                uint16_t Dynamic;

            } Name;

        } IndexBuffer;

    } IndexBufferPool;

    struct
    {
        struct
        {
            struct
            {
                /* IndexStreamPool/IndexStream/IndexStreamPrimitive:TRIANGLES */
                uint16_t TRIANGLES;

            } IndexStreamPrimitive;

        } IndexStream;

    } IndexStreamPool;

    struct
    {
        struct
        {
            struct
            {
                struct
                {
                    struct
                    {
                        /* VertexStreamPool/VertexStream/Elements/Element/Name:Position */
                        uint16_t Position;

                        /* VertexStreamPool/VertexStream/Elements/Element/Name:Uv1 */
                        uint16_t Uv1;

                        /* VertexStreamPool/VertexStream/Elements/Element/Name:Tangent */
                        uint16_t Tangent;

                        /* VertexStreamPool/VertexStream/Elements/Element/Name:Normal */
                        uint16_t Normal;

                        /* VertexStreamPool/VertexStream/Elements/Element/Name:Binormal */
                        uint16_t Binormal;

                        /* VertexStreamPool/VertexStream/Elements/Element/Name:lightmapUV */
                        uint16_t lightmapUV;

                        /* VertexStreamPool/VertexStream/Elements/Element/Name:ColorAdditive */
                        uint16_t ColorAdditive;

                        /* VertexStreamPool/VertexStream/Elements/Element/Name:ColorOcclusion */
                        uint16_t ColorOcclusion;

                        /* VertexStreamPool/VertexStream/Elements/Element/Name:VertexBaked */
                        uint16_t VertexBaked;

                    } Name;

                } Element;

            } Elements;

        } VertexStream;

    } VertexStreamPool;

    struct
    {
        struct
        {
            struct
            {
                /* SceneTreeNodePool/Node/Type:Scene */
                uint16_t Scene;

                /* SceneTreeNodePool/Node/Type:Transform */
                uint16_t Transform;

                /* SceneTreeNodePool/Node/Type:Geometry */
                uint16_t Geometry;

                /* SceneTreeNodePool/Node/Type:SubGeometry */
                uint16_t SubGeometry;

                /* SceneTreeNodePool/Node/Type:SubGeometryLit */
                uint16_t SubGeometryLit;

                /* SceneTreeNodePool/Node/Type:Camera */
                uint16_t Camera;

                /* SceneTreeNodePool/Node/Type:Light */
                uint16_t Light;
            } Type;

            struct
            {
                /* SceneTreeNodePool/Node/LightType:Directional */
                uint16_t Directional;

                /* SceneTreeNodePool/Node/LightType:Point */
                uint16_t Point;

                /* SceneTreeNodePool/Node/LightType:Ambient */
                uint16_t Ambient;

            } LightType;

        } Node;

    } SceneTreeNodePool;

};

/* Global [A]tom [E]num [T]able for the current oct file context */
extern oct_atomEnumTable _oct_aet;

/* Finds all the needed atom name references and enum types from an oct file */
void oct_load_name_bindings(oct_file oct);

