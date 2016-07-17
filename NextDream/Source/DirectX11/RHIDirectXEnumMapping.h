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

            static D3D11_MAP Get(ERHIRESOURCE_LOCK_TYPE inType)
            {
                return D3D11_MAP_READ;
            }

            static D3D11_USAGE Get(ECPU_GPU_ACCESS_MODE inMode)
            {
                if(inMode & EGPU_READ != 0)
                {
                    if(inMode & EGPU_WRITE != 0)
                    {
                        return D3D11_USAGE_DEFAULT;
                    }
                    else if(inMode & ECPU_WRITE != 0)
                    {
                        return D3D11_USAGE_DYNAMIC;
                    }
                    else if (inMode & ECPU_WRITE == 0 && inMode & EGPU_WRITE == 0)
                    {
                        return D3D11_USAGE_IMMUTABLE;
                    }
                    else
                    {
                        //log
                    }
                }
                else if(inMode & ECPU_READ != 0)
                {
                    if (inMode & ECPU_WRITE != 0)
                    {
                        return D3D11_USAGE_STAGING;
                    }
                    else
                    {
                        //log
                    }
                }
                //log
            }

            static D3D11_FILL_MODE Get(EMESH_FILL_MODE inMode)
            {
                switch (inMode)
                {
                case EMESH_FILL_SOLID:
                    return D3D11_FILL_SOLID;
                case EMESH_FILL_WIREFRAME:
                    return D3D11_FILL_WIREFRAME;
                default:
                    break;
                }
                return D3D11_FILL_SOLID;
                //log()
            }

            static D3D11_CULL_MODE Get(EFACE_CULL_MODE inMode)
            {
                switch (inMode)
                {
                case BladeEngine::RHI::EFACE_CULL_NONE:
                    return D3D11_CULL_NONE;
                case BladeEngine::RHI::EFACE_CULL_FRONT:
                    return D3D11_CULL_FRONT;
                case BladeEngine::RHI::EFACE_CULL_BACK:
                    return D3D11_CULL_FRONT;
                default:
                    //log
                    break;
                }
                return D3D11_CULL_NONE;
            }


        };
    }
}

#endif  //!_BLADE_RHI_RHI_DIRECTX_ENUM_MAP_H__
