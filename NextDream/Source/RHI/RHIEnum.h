#ifndef __BLADE_RHI_RHI_ENUM_H__
#define __BLADE_RHI_RHI_ENUM_H__

namespace BladeEngine
{
    namespace RHI
    {
        enum ERHI_PIXEL_FORMAT
        {
            ERHI_PIXEL_FORMAT_R8G8B8A8,
        };

        enum ENUM_SHADER_TYPE
        {
            VERTEX_SHADER,
            HULL_SHADER,
            DOMAIM_SHADER,
            GEOMETRY_SHADER,
            PIXEL_SHADER,
        };

        enum ESHADER_ATTRIB_TYPE
        {
            ESHADER_ATTRIB_FLOAT1,
            ESHADER_ATTRIB_FLOAT2,
            ESHADER_ATTRIB_FLOAT3,
            ESHADER_ATTRIB_FLOAT4,
            ESHADER_ATTRIB_INT1,
            ESHADER_ATTRIB_INT2,
            ESHADER_ATTRIB_INT3,
            ESHADER_ATTRIB_INT4,
            ESHADER_ATTRIB_TEX1D,
            ESHADER_ATTRIB_TEX2D,
            ESHADER_ATTRIB_TEX3D,
            ESHADER_ATTRIB_TEXCUBE,
            ESHADER_ATTRIB_TEX1D_ARRAY,
            ESHADER_ATTRIB_TEX2D_ARRAY,
            ESHADER_ATTRIB_TEX3D_ARRAY,
            ESHADER_ATTRIB_TEXCUBE_ARRAY,
        };

        enum ESHADER_RESOURCE_TYPE
        {
            ESHADER_RESOURCE_UNKNOWN = -1,
            ESHADER_RESOURCE_TEXTURE2D = 0,
            ESHADER_RESOURCE_SAMPLER,
        };

        enum ESHADER_SEMANTIC_TYPE
        {
            ESHADER_SEMANTIC_UNKNOWN = -1,
            ESHADER_SEMANTIC_POSITION = 0,
            ESHADER_SEMANTIC_NORMAL,
            ESHADER_SEMANTIC_COLOR,
            ESHADER_SEMANTIC_TANGNET,
            ESHADER_SEMANTIC_TEXCOORD,
        };

        enum EBASE_ACCESS_MODE
        {
            ECPU_READ = 0x01,
            ECPU_WRITE = 0x02,
            EGPU_READ = 0x04,
            EGPU_WRITE = 0x08,
        };

        enum ECPU_GPU_ACCESS_MODE
        {
            EONLY_GPU_READ = EGPU_READ,
            EONLY_CPU_READ = EGPU_READ,
            ECPU_READ_CPU_WRITE = ECPU_READ | ECPU_WRITE,
            EGPU_READ_CPU_WRITE = EGPU_READ | ECPU_WRITE,
            EGPU_READ_GPU_WRITE = EGPU_READ | EGPU_WRITE,
        };

        enum ELOCK_TYPE
        {
            ELOCK_READ,
            ELOCK_WRITE,
            ELOCK_READ_WRITE,
            ELOCK_WRITE_DISCARD,
        };
    }
}

#endif  //!__BLADE_RHI_RHI_ENUM_H__