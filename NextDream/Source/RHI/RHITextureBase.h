#ifndef __BLADE_RHI_TEXTURE_BASE_H__
#define __BLADE_RHI_TEXTURE_BASE_H__

#include <PlatformAPI.h>
#include <RHIDevice.h>
#include <RHIContext.h>
#include <RHIUtility.h>

namespace BladeEngine
{
    struct RHITextureCreateInfo
    {
        RHITexuteSamplerInfo Sampler;
        uint32 Width, Height, Depth;
        uint32 SampleCount;
        ETEX_FORMAT BaseFormat;
        ECPU_GPU_USAGE_MODE UsageMode;
        ECPU_ACCESS_MODE CpuAccessMode;
        uint32 DataSize;
        void* Data;
    };

    struct RHITextureInitInfo
    {
        uint32 Width, Height, Depth;
        uint32 SampleCount;
        ETEX_FORMAT BaseFormat;
        ECPU_GPU_USAGE_MODE UsageMode;
        ECPU_ACCESS_MODE CpuAccessMode;
    };

    class RHITextureBase : public RHIResource, public IResourceLockable
    {
    protected:
        ETEX_FORMAT m_DataFormat;
        uint32 m_SampleCount;

    public:
        RHITextureBase(IRHIDevice* inDevice, const RHITextureInitInfo& inInitInfo) :
            RHIResource(inDevice, inInitInfo.UsageMode),
            m_DataFormat(inInitInfo.BaseFormat),
            m_SampleCount(inInitInfo.SampleCount)
        {}

    public:
        ETEX_FORMAT GetDataFormat() const { return m_DataFormat; }

        uint32 GetSamplerCount() const { return m_SampleCount; }

        bool CanAsRenderTarget() const { return RHIUtility::CanAsRenderTarget(m_DataFormat, GetUsageMode()); }
        bool CanAsDepthStencil() const { return RHIUtility::CanAsRenderTarget(m_DataFormat, GetUsageMode());; }
        bool CanAsShadowResource() const { return RHIUtility::CanAsRenderTarget(m_DataFormat, GetUsageMode());; }
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

    struct RHISwapChainCreateInfo
    {
        RHITexuteSamplerInfo Sampler;
        uint32 BufferNum;
        uint32 SampleCount;
        uint32 SampleQulity;
        uint32 RefreshRateDenominator;
        uint32 RefreshRateNumerator;
        PlatformWindowRef Window;
    };

    struct RHISwapChainInitInfo
    {
        uint32 Width, Height;
        uint32 BufferNum;
        uint32 SampleCount;
        uint32 SampleQulity;
        uint32 RefreshRateDenominator;
        uint32 RefreshRateNumerator;
        PlatformWindowRef Window;
        RHITexture2DRef Texture;
    };

    class RHISwapChain : public RHIResource
    {
    protected:
        RHITexture2DRef m_texture2D;
        PlatformWindowRef m_belongToWindow;

    public:
        RHISwapChain(IRHIDevice* inDevice, const RHISwapChainInitInfo& inInitInfo) :
            RHIResource(inDevice, ESUIT_GPU_WRITE),
            m_texture2D(inInitInfo.Texture),
            m_belongToWindow(inInitInfo.Window)
        {}

        virtual ~RHISwapChain()
        {
            if (m_belongToWindow != NULL)
                m_belongToWindow = NULL;
            if (m_texture2D != NULL)
                m_texture2D = NULL;
        }

    public:
        virtual void Present() = 0;

    public:
        uint32 GetWidth() const { return m_texture2D->GetWidth(); }
        uint32 GetHeight() const { return m_texture2D->GetHeight(); }

        RHITexture2DRef AsTexture() { return m_texture2D; }
        PlatformWindowRef GetBelongToWindow() { return m_belongToWindow; }
    };
}

#endif // !__BLADE_RHI_TEXTURE_BASE_H__