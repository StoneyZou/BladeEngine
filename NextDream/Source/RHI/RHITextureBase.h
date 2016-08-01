#ifndef __BLADE_RHI_TEXTURE_BASE_H__
#define __BLADE_RHI_TEXTURE_BASE_H__

#include <RHIDevice.h>
#include <RHIContext.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHITextureBase : public RHIResource, public IResourceLockable
        {
        protected:
            EDATA_FORMAT m_PixelFormat;
            uint32 m_SampleQulity;
            uint32 m_SampleCount;

        public:
            RHITextureBase(IRHIDevice* inDevice, const RHITextureCreateInfo& inCreateInfo) :
                RHIResource(inDevice, inCreateInfo.AccessMode),
                m_PixelFormat(inCreateInfo.Format),
                m_SampleQulity(inCreateInfo.SampleQulity),
                m_SampleCount(inCreateInfo.SampleCount)
            {}
        };

        class RHITexture2D : public RHITextureBase
        {
        protected:
            uint32 m_Width;
            uint32 m_Height;

            bool m_CanAsRenderTargetBuffer;
            bool m_CanAsDepthStencilBuffer;

        public:
            RHITexture2D(IRHIDevice* inDevice, const RHITextureCreateInfo& inCreateInfo) :
                RHITextureBase(inDevice, inCreateInfo), m_Width(inCreateInfo.Width), m_Height(inCreateInfo.Height)
            {}
        };
    }
}

#endif // !__BLADE_RHI_TEXTURE_BASE_H__