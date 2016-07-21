#ifndef __BLADE_RHI_UNIFORM_BUFFER_H__
#define __BLADE_RHI_UNIFORM_BUFFER_H__

#include <RHIDevice.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHIUniformBuffer : public RHIResource, public IResourceLockable
        {
        private:
            SIZE_T m_Size;

        public:
            RHIUniformBuffer(IRHIDevice* inDevice, const RHIUniformCreateInfo& inCreateInfo) 
                : RHIResource(inDevice, EGPU_READ_CPU_WRITE),
                m_Size(inCreateInfo.DataSize)
            {}
        };
    }
}

#endif // !__BLADE_RHI_UNIFORM_BUFFER_H__