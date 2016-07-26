#ifndef __BLADE_RHI_TEXTURE_BASE_H__
#define __BLADE_RHI_TEXTURE_BASE_H__

#include <RHIDevice.h>
#include <RHIContext.h>

namespace BladeEngine
{
    namespace RHI
    {
        struct RHITexuteSamplerInfo
        {
            ETEXTURE_FILTER_MODE Filter;
            ETEXTURE_ADDRESS_MODE AddressU;
            ETEXTURE_ADDRESS_MODE AddressV;
            ETEXTURE_ADDRESS_MODE AddressW;
            float MipLODBias;
            uint32 MaxAnisotropy;
            ECOMPARISON_FUNC ComparisonFunc;
            float BorderColor[4];
            float MinLOD;
            float MaxLOD;
        };

        /**
        * @Desc Structure contains all infomations to create a texture
        */
        struct RHITextureCreateInfo
        {
            RHITexuteSamplerInfo Sampler;

            uint32 Width;
            uint32 Height;
            ERHI_PIXEL_FORMAT Format;
            ECPU_GPU_ACCESS_MODE AccessMode;
            uint32 SampleQulity;
            uint32 SampleCount;

            uint32 DataSize;
            void* Data;
        };

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

        public:
            RHITexture2D(IRHIDevice* inDevice, const RHITextureCreateInfo& inCreateInfo) :
                RHITextureBase(inDevice, inCreateInfo), m_Width(inCreateInfo.Width), m_Height(inCreateInfo.Height)
            {}
        };
    }
}

#endif // !__BLADE_RHI_TEXTURE_BASE_H__