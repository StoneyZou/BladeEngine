#ifndef __BLADE_RHI_RHI_CONTEXT_H__
#define __BLADE_RHI_RHI_CONTEXT_H__

namespace BladeEngine
{
    namespace RHI
    {
        class IRHIContextBase
        {
        public:
            void SetShaderTexture()
            {

            }
        };

        class RHIImmediateContext : public IRHIContextBase
        {

        };

        class RHIDeferredContext : public IRHIContextBase
        {

        };
    }
}

#endif // !__BLADE_RHI_RHI_CONTEXT_H__
