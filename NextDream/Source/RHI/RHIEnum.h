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
    }
}

#endif  //!__BLADE_RHI_RHI_ENUM_H__