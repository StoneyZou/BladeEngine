#ifndef __BLADE_RHI_TEXTURE_BASE_H__
#define __BLADE_RHI_TEXTURE_BASE_H__

#include <RHIDevice.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHITextureBase
        {
        private:
            ERHI_PIXEL_FORMAT m_PixelFormat;
            ECPU_GPU_ACCESS_MODE m_CpuGpuAccessMode;

            void* m_LockingData;

        public:
            ERHI_PIXEL_FORMAT GetFormat() const
            {
                return m_PixelFormat;
            }

            virtual void* Lock() = 0;
            virtual void Unlock(void* inData) = 0;
        };

        class RHITexture2D : public RHITextureBase
        {
        };
    }
}

#endif // !__BLADE_RHI_TEXTURE_BASE_H__