#ifndef __BLADE_RHI_RHI_CONTEXT_H__
#define __BLADE_RHI_RHI_CONTEXT_H__

#include <RHIDevice.h>

namespace BladeEngine
{
    namespace RHI
    {
        class IRHIContextBase
        {
        public:
            virtual bool SetVertexShader(RHIVertexShader* inRHIShader) = 0;

            virtual bool SetPixelShader(RHIPixelShader* inRHIShader) = 0;

            virtual bool SetGeometryShader(RHIGeometryShader* inRHIShader) = 0;

            virtual bool SetHullShader(RHIHullShader* inRHIShader) = 0;

            virtual bool SetDomainShader(RHIDomainShader* inRHIShader) = 0;

            virtual bool SetShaderState(RHIShaderState* inRHIShader) = 0;

            virtual bool SetTexture(RHITextureBase* inTex) = 0;

            //virtual bool SetUniformBuffer(RHIShaderState* inRHIShader) = 0;
        };

        class RHIImmediateContext : public IRHIContextBase
        {
        public:

        };

        class RHIDeferredContext : public IRHIContextBase
        {
        public:

        };
    }
}

#endif // !__BLADE_RHI_RHI_CONTEXT_H__
