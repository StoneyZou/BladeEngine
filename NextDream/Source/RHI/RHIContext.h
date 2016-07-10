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
            virtual bool SetVertexShader(const RHIVertexShaderRef& inRHIShader) = 0;
        };

        class RHIImmediateContext : public IRHIContextBase
        {
        public:

        };

        class RHIDeferredContext : public IRHIContextBase
        {

        };
    }
}

#endif // !__BLADE_RHI_RHI_CONTEXT_H__
