#ifndef __BALDE_RHI_RHI_DIRECTX_ENUM_MAP_H__
#define __BLADE_RHI_RHI_DIRECTX_ENUM_MAP_H__

#include <RHIEnum.h>

#include <dxgi.h>
#include <d3d11.h>

namespace BladeEngine
{
    namespace RHI
    {   
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

            static D3D11_MAP Get(ERES_LOCK_TYPE inType)
            {
                return D3D11_MAP_READ;
            }

            static D3D11_USAGE Get(ECPU_GPU_ACCESS_MODE inMode)
            {
                if((inMode & EGPU_READ) != 0)
                {
                    if((inMode & EGPU_WRITE) != 0)
                    {
                        return D3D11_USAGE_DEFAULT;
                    }
                    else if((inMode & ECPU_WRITE) != 0)
                    {
                        return D3D11_USAGE_DYNAMIC;
                    }
                    else if ((inMode & ECPU_WRITE) == 0 && (inMode & EGPU_WRITE) == 0)
                    {
                        return D3D11_USAGE_IMMUTABLE;
                    }
                    else
                    {
                        //log
                    }
                }
                else if ((inMode & ECPU_READ) != 0)
                {
                    if ((inMode & ECPU_WRITE) != 0)
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

            static D3D11_BLEND Get(EBLEND_ARG inArg)
            {
                switch (inArg)
                {
                case BladeEngine::RHI::EBLEND_ARG_ZERO:
                    return D3D11_BLEND_ZERO;
                case BladeEngine::RHI::EBLEND_ARG_ONE:
                    return D3D11_BLEND_ONE;
                case BladeEngine::RHI::EBLEND_ARG_SRC_COLOR:
                    return D3D11_BLEND_SRC_COLOR;
                case BladeEngine::RHI::EBLEND_ARG_INV_SRC_COLOR:
                    return D3D11_BLEND_INV_SRC_COLOR;
                case BladeEngine::RHI::EBLEND_ARG_SRC_ALPHA:
                    return D3D11_BLEND_SRC_ALPHA;
                case BladeEngine::RHI::EBLEND_ARG_INV_SRC_ALPHA:
                    return D3D11_BLEND_INV_SRC_ALPHA;
                case BladeEngine::RHI::EBLEND_ARG_DEST_ALPHA:
                    return D3D11_BLEND_DEST_ALPHA;
                case BladeEngine::RHI::EBLEND_ARG_INV_DEST_ALPHA:
                    return D3D11_BLEND_INV_DEST_ALPHA;
                case BladeEngine::RHI::EBLEND_ARG_DEST_COLOR:
                    return D3D11_BLEND_DEST_COLOR;
                case BladeEngine::RHI::EBLEND_ARG_INV_DEST_COLOR:
                    return D3D11_BLEND_INV_DEST_COLOR;
                case BladeEngine::RHI::EBLEND_ARG_SRC_ALPHA_SAT:
                    return D3D11_BLEND_SRC_ALPHA_SAT;
                case BladeEngine::RHI::EBLEND_ARG_BLEND_FACTOR:
                    return D3D11_BLEND_BLEND_FACTOR;
                case BladeEngine::RHI::EBLEND_ARG_INV_BLEND_FACTOR:
                    return D3D11_BLEND_INV_BLEND_FACTOR;
                case BladeEngine::RHI::EBLEND_ARG_SRC1_COLOR:
                    return D3D11_BLEND_SRC1_COLOR;
                case BladeEngine::RHI::EBLEND_ARG_INV_SRC1_COLOR:
                    return D3D11_BLEND_INV_SRC1_COLOR;
                case BladeEngine::RHI::EBLEND_ARG_SRC1_ALPHA:
                    return D3D11_BLEND_SRC1_ALPHA;
                case BladeEngine::RHI::EBLEND_ARG_INV_SRC1_ALPHA:
                    return D3D11_BLEND_INV_SRC1_ALPHA;
                default:
                    //log
                    break;
                }
            }

            static D3D11_BLEND_OP Get(EBLEND_FUNC inFunc)
            {
                switch (inFunc)
                {
                case BladeEngine::RHI::EBLEND_FUNC_ADD:
                    return D3D11_BLEND_OP_ADD;
                case BladeEngine::RHI::EBLEND_FUNC_SUBTRACT:
                    return D3D11_BLEND_OP_SUBTRACT;
                case BladeEngine::RHI::EBLEND_FUNC_REV_SUBTRACT:
                    return D3D11_BLEND_OP_REV_SUBTRACT;
                case BladeEngine::RHI::EBLEND_FUNC_MIN:
                    return D3D11_BLEND_OP_MIN;
                case BladeEngine::RHI::EBLEND_FUNC_MAX:
                    return D3D11_BLEND_OP_MAX;;
                default:
                    //log
                    break;
                }
            }

            static D3D11_COMPARISON_FUNC Get(EDEPTH_STENCIL_COMPARISON_FUNC inFunc)
            {
                switch (inFunc)
                {
                case BladeEngine::RHI::EDEPTH_STENCIL_COMPARISON_NEVER:
                    return D3D11_COMPARISON_NEVER;
                case BladeEngine::RHI::EDEPTH_STENCIL_COMPARISON_LESS:
                    return D3D11_COMPARISON_LESS;
                case BladeEngine::RHI::EDEPTH_STENCIL_COMPARISON_EQUAL:
                    return D3D11_COMPARISON_EQUAL;
                case BladeEngine::RHI::EDEPTH_STENCIL_COMPARISON_LESS_EQUAL:
                    return D3D11_COMPARISON_LESS_EQUAL;
                case BladeEngine::RHI::EDEPTH_STENCIL_COMPARISON_GREATER:
                    return D3D11_COMPARISON_GREATER;
                case BladeEngine::RHI::EDEPTH_STENCIL_COMPARISON_NOT_EQUAL:
                    return D3D11_COMPARISON_NOT_EQUAL;
                case BladeEngine::RHI::EDEPTH_STENCIL_COMPARISON_GREATER_EQUAL:
                    return D3D11_COMPARISON_GREATER_EQUAL;
                case BladeEngine::RHI::EDEPTH_STENCIL_COMPARISON_ALWAYS:
                    return D3D11_COMPARISON_ALWAYS;
                default:
                    //log
                    break;
                }
            }

            static D3D11_STENCIL_OP Get(EDEPTH_STENCIL_WRITE_FUNC inFunc)
            {
                switch (inFunc)
                {
                case BladeEngine::RHI::EDEPTH_STENCIL_WRITE_KEEP:
                    return D3D11_STENCIL_OP_KEEP;
                case BladeEngine::RHI::EDEPTH_STENCIL_WRITE_ZERO:
                    return D3D11_STENCIL_OP_ZERO;
                case BladeEngine::RHI::EDEPTH_STENCIL_WRITE_REPLACE:
                    return D3D11_STENCIL_OP_REPLACE;
                case BladeEngine::RHI::EDEPTH_STENCIL_WRITE_INCR_SAT:
                    return D3D11_STENCIL_OP_INCR_SAT;
                case BladeEngine::RHI::EDEPTH_STENCIL_WRITE_DECR_SAT:
                    return D3D11_STENCIL_OP_DECR_SAT;
                case BladeEngine::RHI::EDEPTH_STENCIL_WRITE_INVERT:
                    return D3D11_STENCIL_OP_INVERT;
                case BladeEngine::RHI::EDEPTH_STENCIL_WRITE_INCR:
                    return D3D11_STENCIL_OP_INCR;
                case BladeEngine::RHI::EDEPTH_STENCIL_WRITE_DECR:
                    return D3D11_STENCIL_OP_DECR;
                default:
                    //log
                    break;
                }
            }
        };
    }
}

#endif  //!_BLADE_RHI_RHI_DIRECTX_ENUM_MAP_H__
