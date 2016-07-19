#ifndef __BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__
#define __BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__

#include <d3d11.h>
#include <RHITextureBase.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHIDirectX11Texture2D : public RHITexture2D
        {
        private:
            ID3D11Texture2D* m_Texture2D;
            ID3D11RenderTargetView* m_RenderTarget;
            ID3D11ShaderResourceView* m_ShaderResource;

        public:
            RHIDirectX11Texture2D(ID3D11Texture2D* inTexture2D, ID3D11RenderTargetView* inRenderTarget,
                ID3D11ShaderResourceView* inShaderResource, const RHITexture2DCreateInfo& inCreateInfo)
                : RHITexture2D(inCreateInfo),
                m_Texture2D(inTexture2D),
                m_RenderTarget(inRenderTarget),
                m_ShaderResource(inShaderResource)
            {
                BladeAssert(inTexture2D != NULL);
                BladeAssert(GetAccessMode() & EGPU_WRITE != 0 && m_RenderTarget != NULL);
                BladeAssert(GetAccessMode() & EGPU_READ != 0 && m_ShaderResource != NULL);

                m_Texture2D->AddRef();
                if (GetAccessMode() & EGPU_WRITE != 0)
                    m_RenderTarget->AddRef();
                if (GetAccessMode() & EGPU_READ != 0)
                    m_ShaderResource->AddRef();
            }

            ~RHIDirectX11Texture2D()
            {
                m_Texture2D->Release();
                if (GetAccessMode() & EGPU_WRITE != 0)
                    m_RenderTarget->Release();
                if (GetAccessMode() & EGPU_READ != 0)
                    m_ShaderResource->Release();
            }

        public:
            virtual void* Lock(IRHIDevice* inParam, ERHIRESOURCE_LOCK_TYPE inType, const SIZE_T inIndex)
            {

            }

            virtual void Unlock(IRHIDevice* inParam, const SIZE_T inIndex) = 0;

        public:
            ID3D11Texture2D* GetTexture()
            {
                return m_Texture2D;
            }

            ID3D11RenderTargetView* GetRenderTarget()
            {
                return m_RenderTarget;
            }

            ID3D11ShaderResourceView* GetShaderResource()
            {
                return m_ShaderResource;
            }
        };

    }
}

#endif // !__BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__