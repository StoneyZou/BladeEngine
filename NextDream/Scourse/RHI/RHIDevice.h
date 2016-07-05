#ifndef __BLADE_RHI_RHI_DEVICE_H__
#define __BLADE_RHI_RHI_DEVICE_H__

#include "Utility.h"
#include "RHIEnum.h"

namespace BladeEngine
{
    namespace RHI
    {
        template<ENUM_SHADER_TYPE ShaderType>
        class RHIShader : RHIShaderBase
        {};

        typedef RHIShader<VERTEX_SHADER> RHIVertexShader;
        typedef RHIShader<HULL_SHADER> RHIHullShader;
        typedef RHIShader<DOMAIM_SHADER> RHIDomainShader;
        typedef RHIShader<GEOMETRY_SHADER> RHIGeometryShader;
        typedef RHIShader<PIXEL_SHADER> RHIPixelShader;

        class RHITextureBase;
        typedef RefCountObject<RHITextureBase> RHITextureBaseRef;

        class RHIShaderBase;
        typedef RefCountObject<RHIShaderBase> RHIShaderBaseRef;

        class RHIVertexBuffer;
        typedef RefCountObject<RHIVertexBuffer> RHIVertexBufferRef;

        class RHIIndexBuffer;
        typedef RefCountObject<RHIIndexBuffer> RHIIndexBufferRef;

        typedef RefCountObject<RHIVertexShader> RHIVertexBufferRef;
        typedef RefCountObject<RHIHullShader> RHIHullShaderRef;
        typedef RefCountObject<RHIDomainShader> RHIDomainShaderRef;
        typedef RefCountObject<RHIGeometryShader> RHIGeometryShaderRef;
        typedef RefCountObject<RHIPixelShader> RHIPixelShaderRef;

        class IRHIDevice
        {
        public:
           virtual RHITextureBaseRef CreateTexture2D(uint32 inWidth, uint32 inHeight, ENUM_RHI_PIXEL_FORMAT inFormat, bool inEnableReadWrite, void* inScource) = 0;

           virtual RHIVertexShader CreateVextexShader(void* inScource) = 0;
        };
    }
}
#endif // !__BLADE_RHI_RHI_DEVICE_H__