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
            bool m_CanAsRenderTarget;
            bool m_CanAsDepthStencil;
            bool m_CanAsShaderResource;
            
            uint32 m_SampleCount;
            uint32 m_SampleQulity;

            EDATA_FORMAT m_DataFormat;

        public:
            RHITextureBase(IRHIDevice* inDevice, const RHITextureInitInfo& inInitInfo) :
                RHIResource(inDevice, inInitInfo.AccessMode),
                m_DataFormat(inInitInfo.BaseFormat),
                m_SampleQulity(inInitInfo.SampleQulity),
                m_SampleCount(inInitInfo.SampleCount),
                m_CanAsRenderTarget(inInitInfo.CanAsRenderTarget),
                m_CanAsDepthStencil(inInitInfo.CanAsDepthStencil),
                m_CanAsShaderResource(inInitInfo.CanAsShaderResource)
            {}

        public:
            EDATA_FORMAT GetDataFormat() const { return m_DataFormat; }
            uint32 GetSampleQulity() const { return m_SampleQulity; }
            uint32 GetSampleCount() const { return m_SampleQulity; }

            bool CanAsRenderTarget() const      { return m_CanAsRenderTarget; }
            bool CanAsDepthStencil() const      { return m_CanAsDepthStencil; }
            bool CanAsShaderResource() const    { return m_CanAsShaderResource; }
        };

        class RHITexture2D : public RHITextureBase
        {
        protected:
            uint32 m_Width;
            uint32 m_Height;

        public:
            RHITexture2D(IRHIDevice* inDevice, const RHITextureInitInfo& inInitInfo) :
                RHITextureBase(inDevice, inInitInfo),
                m_Width(inInitInfo.Width),
                m_Height(inInitInfo.Height)
            {}

        public:
            uint32 GetWidth() const { return m_Width; }
            uint32 GetHeight() const { return m_Height; }
        };
    }
}

#endif // !__BLADE_RHI_TEXTURE_BASE_H__