#ifndef __BLADE_RHI_TEXTURE_BASE_H__
#define __BLADE_RHI_TEXTURE_BASE_H__

#include <RHIDevice.h>

namespace BladeEngine
{
    namespace RHI
    {
        /**
        * @Desc Structure contains all infomations to create a texture
        */
        struct RHITextureCreateInfo
        {
            ERHI_PIXEL_FORMAT Format;
            ECPU_GPU_ACCESS_MODE AccessMode;
            uint32 DataSize;
            void* Data;
        };

        struct RHITexture2DCreateInfo : public RHITextureCreateInfo
        {
            uint32 Width;
            uint32 Height;
        };

        class RHITextureBase : public RHIResource, public IResourceCopyable, public IResourceLockable
        {
        private:
            ERHI_PIXEL_FORMAT m_PixelFormat;
            ECPU_GPU_ACCESS_MODE m_AccessMode;

            void* m_LockingData;

        public:
            RHITextureBase(const RHITextureCreateInfo& inCreateInfo) :
                RHIResource(inCreateInfo.AccessMode), m_PixelFormat(inCreateInfo.Format), m_LockingData(NULL)
            {}

        public:
            ERHI_PIXEL_FORMAT GetFormat() const { return m_PixelFormat; }
            ECPU_GPU_ACCESS_MODE GetAccessMode() const { return m_AccessMode; }
        };

        class RHITexture2D : public RHITextureBase
        {
        private:
            uint32 m_Width;
            uint32 m_Height;

        public:
            RHITexture2D(const RHITexture2DCreateInfo& inCreateInfo) :
                RHITextureBase(inCreateInfo), m_Width(inCreateInfo.Width), m_Height(inCreateInfo.Height)
            {}
        };
    }
}

#endif // !__BLADE_RHI_TEXTURE_BASE_H__