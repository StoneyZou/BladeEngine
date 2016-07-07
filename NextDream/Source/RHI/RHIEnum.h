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
    }
}

#endif  //!__BLADE_RHI_RHI_ENUM_H__