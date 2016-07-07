#ifndef __BALDE_RHI_RHI_DIRECTX_ENUM_MAP_H__
#define __BLADE_RHI_RHI_DIRECTX_ENUM_MAP_H__

#include <RHIEnum.h>

namespace BladeEngine
{
    namespace RHI
    {
        #include <dxgi.h>
        
        class RHIDirectXEnumMapping
        {
        public:
            static DXGI_FORMAT Get(ERHI_PIXEL_FORMAT inFormat)
            {
                DXGI_FORMAT result = DXGI_FORMAT_UNKNOWN;
                switch (inFormat)
                {
                case BladeEngine::RHI::ERHI_PIXEL_FORMAT_R8G8B8A8:
                    result = DXGI_FORMAT_R8G8B8A8_UINT;
                    break;
                default:
                    break;
                }

                return result;
            }
        };
    }
}

#endif  //!_BLADE_RHI_RHI_DIRECTX_ENUM_MAP_H__
