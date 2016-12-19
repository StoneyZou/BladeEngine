#pragma once

#include <TArray.h>
#include <TypeDefine.h>
#include <BColor.h>
#include <BVector2.h>
#include <BVector3.h>

namespace BladeEngine
{
    static const SIZE_T MAX_TEXCOORDS = 4;

    struct RawMeshWedge
    {
        uint32 VertexIndex;
        BColor Color;
        BVector2 UVs[MAX_TEXCOORDS];
    };

    struct RawMeshFace
    {
        uint32 WedgeIndex[3];
        BVector3 Tangent[3];
        BVector3 Normal[3];
        BVector3 Binormal[3];
        int32 MaterialIndex;
    };

    struct RawMesh
    {
        TArray<BVector3> Vertexs;
        TArray<RawMeshWedge> Wedges;
        TArray<RawMeshFace> Faces;
    };


}