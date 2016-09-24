#ifndef __BLADE_RHI_RHI_DIRECTX_ENUM_MAP_H__
#define __BLADE_RHI_RHI_DIRECTX_ENUM_MAP_H__

#include <dxgi.h>
#include <d3d11.h>
#include <assert.h>

#include <RHIEnum.h>
#include <TypeDefine.h>
#include <StringUtil.h>

namespace BladeEngine
{
    namespace RHI
    {   
        class DirectXEnumMapping
        {
        public:
            static DXGI_FORMAT GetPixelFormat(EDATA_FORMAT inFormat)
            {
                DXGI_FORMAT result = DXGI_FORMAT_UNKNOWN;
                switch (inFormat)
                {
                case BladeEngine::RHI::EDATA_FORMAT_R8G8B8A8_UINT:
                    result = DXGI_FORMAT_R8G8B8A8_UINT;
                    break;
                default:
                    break;
                }

                //log
                return DXGI_FORMAT_R8G8B8A8_UINT;
            }

            static D3D11_MAP GetPixelFormat(ERES_LOCK_TYPE inType)
            {
                //log
                return D3D11_MAP_READ;
            }

            static D3D11_USAGE GetPixelFormat(ECPU_GPU_USAGE_MODE inMode)
            {
                D3D11_USAGE result = D3D11_USAGE_DEFAULT;
                switch (inMode)
                {
                case BladeEngine::RHI::EONLY_GPU_READ:
                    result = D3D11_USAGE_IMMUTABLE;
                    break;
                case BladeEngine::RHI::EGPU_READ_GPU_WRITE:
                case BladeEngine::RHI::EONLY_GPU_WRITE:
                    result = D3D11_USAGE_DEFAULT;
                    break;
                case BladeEngine::RHI::EGPU_READ_CPU_WRITE:
                    result = D3D11_USAGE_DYNAMIC;
                    break;
                default:
                    assert(0);
                    break;
                }

                //log
                return D3D11_USAGE_DEFAULT;
            }

            static D3D11_FILL_MODE GetPixelFormat(EMESH_FILL_MODE inMode)
            {
                switch (inMode)
                {
                case EMESH_FILL_SOLID:
                    return D3D11_FILL_SOLID;
                case EMESH_FILL_WIREFRAME:
                    return D3D11_FILL_WIREFRAME;
                default:
                    assert(0);
                    break;
                }

                //log()
                return D3D11_FILL_SOLID;
            }

            static D3D11_CULL_MODE GetPixelFormat(EFACE_CULL_MODE inMode)
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
                    break;
                }

                //log
                return D3D11_CULL_NONE;
            }

            static D3D11_BLEND GetPixelFormat(EBLEND_ARG inArg)
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
                    break;
                }

                //log
                return D3D11_BLEND_ZERO;
            }

            static D3D11_BLEND_OP GetPixelFormat(EBLEND_FUNC inFunc)
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
                    break;
                }

                //log
                return D3D11_BLEND_OP_ADD;
            }

            static D3D11_COMPARISON_FUNC GetPixelFormat(ECOMPARISON_FUNC inFunc)
            {
                switch (inFunc)
                {
                case BladeEngine::RHI::ECOMPARISON_NEVER:
                    return D3D11_COMPARISON_NEVER;
                case BladeEngine::RHI::ECOMPARISON_LESS:
                    return D3D11_COMPARISON_LESS;
                case BladeEngine::RHI::ECOMPARISON_EQUAL:
                    return D3D11_COMPARISON_EQUAL;
                case BladeEngine::RHI::ECOMPARISON_LESS_EQUAL:
                    return D3D11_COMPARISON_LESS_EQUAL;
                case BladeEngine::RHI::ECOMPARISON_GREATER:
                    return D3D11_COMPARISON_GREATER;
                case BladeEngine::RHI::ECOMPARISON_NOT_EQUAL:
                    return D3D11_COMPARISON_NOT_EQUAL;
                case BladeEngine::RHI::ECOMPARISON_GREATER_EQUAL:
                    return D3D11_COMPARISON_GREATER_EQUAL;
                case BladeEngine::RHI::ECOMPARISON_ALWAYS:
                    return D3D11_COMPARISON_ALWAYS;
                default:
                    break;
                }

                //log
                return D3D11_COMPARISON_NEVER;
            }

            static D3D11_STENCIL_OP GetPixelFormat(EDEPTH_STENCIL_WRITE_FUNC inFunc)
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
                    break;
                }

                //log
                return D3D11_STENCIL_OP_KEEP;
            }

            static D3D11_TEXTURE_ADDRESS_MODE GetPixelFormat(ETEXTURE_ADDRESS_MODE inMode)
            {
                switch (inMode)
                {
                case BladeEngine::RHI::ETEXTURE_ADDRESS_WRAP:
                    return D3D11_TEXTURE_ADDRESS_WRAP;
                case BladeEngine::RHI::ETEXTURE_ADDRESS_CLAMP:
                    return D3D11_TEXTURE_ADDRESS_CLAMP;
                default:
                    break;
                }

                //log
                return D3D11_TEXTURE_ADDRESS_WRAP;
            }

            static D3D11_FILTER GetPixelFormat(ETEXTURE_FILTER_MODE inMode)
            {
                switch (inMode)
                {
                case BladeEngine::RHI::ETEXTURE_FILTER_MIN_MAG_MIP_POINT:
                    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
                case BladeEngine::RHI::ETEXTURE_FILTER_MIN_LINEAR_MAG_MIP_POINT:
                    return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
                case BladeEngine::RHI::ETEXTURE_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT:
                    return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
                case BladeEngine::RHI::ETEXTURE_FILTER_MIN_MAG_POINT_MIP_LINEAR:
                    return D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
                case BladeEngine::RHI::ETEXTURE_FILTER_MIN_MAG_LINEAR_MIP_POINT:
                    return D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
                case BladeEngine::RHI::ETEXTURE_FILTER_MIN_POINT_MAG_MIP_LINEAR:
                    return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
                case BladeEngine::RHI::ETEXTURE_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
                    return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                case BladeEngine::RHI::ETEXTURE_FILTER_MIN_MAG_MIP_LINEAR:
                    return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                default:
                    break;
                }

                //log
                return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
            }

            static D3D11_INPUT_CLASSIFICATION GetPixelFormat(EINPUT_CLASSIFICATION inType)
            {
                switch (inType)
                {
                case BladeEngine::RHI::EINPUT_PER_VERTEX_DATA:
                    return D3D11_INPUT_PER_VERTEX_DATA;
                case BladeEngine::RHI::EINPUT_PER_INSTANCE_DATA:
                    return D3D11_INPUT_PER_INSTANCE_DATA;
                default:
                    break;
                }

                //log
                return D3D11_INPUT_PER_VERTEX_DATA;
            }

        public:
            static ESHADER_SEMANTIC_TYPE GetSematicType(const ANSICHAR* inSemanticStr);

            static ESHADER_ATTRIB_TYPE GetAttrType(D3D_SHADER_VARIABLE_TYPE inD3DType)
            {
                ESHADER_ATTRIB_TYPE result = ESHADER_ATTRIB_FLOAT;
                switch(inD3DType)
                {
                case D3D_SVT_INT:
                    result = ESHADER_ATTRIB_INT;
                    break;
                case D3D_SVT_FLOAT:
                    result = ESHADER_ATTRIB_FLOAT;
                    break;
                default:
                    break;
                    // log
                }
                return result;
            }
        };
    }
}

#endif  //!_BLADE_RHI_RHI_DIRECTX_ENUM_MAP_H__
