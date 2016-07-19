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

        enum ERHIRESOURCE_LOCK_TYPE
        {
            ERHIRESOURCE_LOCK_READ,
            ERHIRESOURCE_LOCK_WRITE,
            ERHIRESOURCE_LOCK_READ_WRITE,
            ERHIRESOURCE_LOCK_WRITE_DISCARD,
        };

        enum EMESH_FILL_MODE
        {
            EMESH_FILL_WIREFRAME,
            EMESH_FILL_SOLID,
        };

        enum EFACE_CULL_MODE
        {
            EFACE_CULL_NONE,
            EFACE_CULL_FRONT,
            EFACE_CULL_BACK,
        };

        enum EBLEND_ARG
        {
            EBLEND_ARG_ZERO = 1,
            EBLEND_ARG_ONE = 2,
            EBLEND_ARG_SRC_COLOR = 3,
            EBLEND_ARG_INV_SRC_COLOR = 4,
            EBLEND_ARG_SRC_ALPHA = 5,
            EBLEND_ARG_INV_SRC_ALPHA = 6,
            EBLEND_ARG_DEST_ALPHA = 7,
            EBLEND_ARG_INV_DEST_ALPHA = 8,
            EBLEND_ARG_DEST_COLOR = 9,
            EBLEND_ARG_INV_DEST_COLOR = 10,
            EBLEND_ARG_SRC_ALPHA_SAT = 11,
            EBLEND_ARG_BLEND_FACTOR = 14,
            EBLEND_ARG_INV_BLEND_FACTOR = 15,
            EBLEND_ARG_SRC1_COLOR = 16,
            EBLEND_ARG_INV_SRC1_COLOR = 17,
            EBLEND_ARG_SRC1_ALPHA = 18,
            EBLEND_ARG_INV_SRC1_ALPHA = 19
        };

        enum EBLEND_FUNC
        {
            EBLEND_FUNC_ADD = 1,
            EBLEND_FUNC_SUBTRACT = 2,
            EBLEND_FUNC_REV_SUBTRACT = 3,
            EBLEND_FUNC_MIN = 4,
            EBLEND_FUNC_MAX = 5
        };

        enum EDEPTH_STENCIL_COMPARISON_FUNC
        {
            EDEPTH_STENCIL_COMPARISON_NEVER = 1,
            EDEPTH_STENCIL_COMPARISON_LESS = 2,
            EDEPTH_STENCIL_COMPARISON_EQUAL = 3,
            EDEPTH_STENCIL_COMPARISON_LESS_EQUAL = 4,
            EDEPTH_STENCIL_COMPARISON_GREATER = 5,
            EDEPTH_STENCIL_COMPARISON_NOT_EQUAL = 6,
            EDEPTH_STENCIL_COMPARISON_GREATER_EQUAL = 7,
            EDEPTH_STENCIL_COMPARISON_ALWAYS = 8
        };

        enum EDEPTH_STENCIL_WRITE_FUNC
        {
            EDEPTH_STENCIL_WRITE_KEEP = 1,
            EDEPTH_STENCIL_WRITE_ZERO = 2,
            EDEPTH_STENCIL_WRITE_REPLACE = 3,
            EDEPTH_STENCIL_WRITE_INCR_SAT = 4,
            EDEPTH_STENCIL_WRITE_DECR_SAT = 5,
            EDEPTH_STENCIL_WRITE_INVERT = 6,
            EDEPTH_STENCIL_WRITE_INCR = 7,
            EDEPTH_STENCIL_WRITE_DECR = 8
        };
    }
}

#endif  //!__BLADE_RHI_RHI_ENUM_H__