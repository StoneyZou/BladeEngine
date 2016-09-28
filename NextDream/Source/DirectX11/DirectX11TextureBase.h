#ifndef __BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__
#define __BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__

#include <d3d11.h>
#include <RHITextureBase.h>
#include <RHIContext.h>
#include <DirectX11Device.h>
#include <DirectX11Context.h>
#include <DirectXEnumMapping.h>

namespace BladeEngine
{
    namespace RHI
    {
        struct DirectX11Texture2DInitInfo : public RHITextureInitInfo
        {
            ID3D11Texture2D* Texture;
            ID3D11RenderTargetView* RenderTargetView;
            ID3D11DepthStencilView* DepthStencilView;
            ID3D11ShaderResourceView* ShaderResourceView;
        };

        class DirectX11Texture2D : public RHITexture2D, public IDirectX11TextureInterface
        {
        private:
            ID3D11Texture2D* m_Texture;
            ID3D11Texture2D* m_ShadowTexture;
            ID3D11Texture2D* m_LockingTexture;

            ID3D11RenderTargetView* m_RenderTarget;
            ID3D11DepthStencilView* m_DepthStencil;
            ID3D11ShaderResourceView* m_ShaderResource;


        public:
            DirectX11Texture2D( DirectX11Device* inDevice, const DirectX11Texture2DInitInfo& inInitInfo)
                : RHITexture2D(inDevice, inInitInfo),
                m_Texture(inInitInfo.Texture),
                m_ShadowTexture(NULL),
                m_LockingTexture(NULL),
                m_RenderTarget(inInitInfo.RenderTargetView),
                m_DepthStencil(inInitInfo.DepthStencilView),
                m_ShaderResource(inInitInfo.ShaderResourceView)
            {
                BladeAssert(m_Texture != NULL);
                BladeAssert(m_ShaderResource != NULL)
                BladeAssert((CanAsRenderTarget() && m_RenderTarget != NULL) || (!CanAsRenderTarget() && m_RenderTarget == NULL));
                BladeAssert((CanAsDepthStencil() && m_ShaderResource != NULL) || (!CanAsDepthStencil() && m_ShaderResource == NULL));

                m_Texture->AddRef();
                m_ShaderResource->AddRef();
                if (m_RenderTarget != NULL) { m_RenderTarget->AddRef(); }
                if (m_DepthStencil != NULL) { m_DepthStencil->AddRef(); }
            }

            ~DirectX11Texture2D()
            {
                m_Texture->Release();
                if (m_ShadowTexture != NULL) { m_ShadowTexture->Release(); }

                m_ShaderResource->Release();
                if (m_RenderTarget != NULL) { m_RenderTarget->Release(); }
                if (m_DepthStencil != NULL) { m_DepthStencil->Release(); }

                m_LockingTexture = NULL;
            }

        public:
            virtual void* Lock(RHIContextBase* inContext, ERES_LOCK_TYPE inType, const SIZE_T inIndex)
            {
                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());

                bool cantLock =(CanCpuRead() && (inType & ERES_LOCK_READ) != 0) ||
                    (CanCpuWrite() && (inType & ERES_LOCK_WRITE) != 0);

                D3D11_MAP mapType = D3D11_MAP_READ_WRITE;
                mapType = (inType == ERES_LOCK_ONLY_READ ? D3D11_MAP_READ : mapType);
                mapType = (inType == ERES_LOCK_ONLY_WRITE ? D3D11_MAP_WRITE : mapType);
                mapType = (inType == ERES_LOCK_READ_WRITE ? D3D11_MAP_READ_WRITE : mapType);
                mapType = (inType == ERES_LOCK_WRITE_DISCARD ? D3D11_MAP_WRITE_DISCARD : mapType);

//#if _DEBUG
//                if(!Suit && inType == ERES_LOCK_ONLY_WRITE)
//                {
//                    //log can create texture use EGPU_READ_CPU_WRITE
//                }
//#endif

                if (mapType == D3D11_MAP_WRITE_DISCARD && (GetUsageMode() == ESUIT_GPU_READ_CPU_WRITE))
                {
                    //log
                    return NULL;
                }

                if(cantLock)
                {
                    m_LockingTexture = m_Texture;
                    return contextImpl->LockGpuResource(m_Texture, inIndex, mapType, true);
                }

                if (m_ShadowTexture != NULL)
                {
                    ID3D11Device* device = contextImpl->GetDevice();
                    if (device == NULL)
                    {
                        return NULL;
                    }

                    // 检查多重采样支持情况
                    UINT sampleQuality = 0;
                    if (m_SampleCount > 1)
                    {
                        if (device->CheckMultisampleQualityLevels(DirectXEnumMapping::GetPixelFormat(m_DataFormat), m_SampleCount, &sampleQuality) != S_OK)
                        {
                            sampleQuality = 0;
                        }
                    }

                    D3D11_TEXTURE2D_DESC desc = { 0 };
                    desc.Width = m_Width;
                    desc.Height = m_Height;
                    desc.Usage = D3D11_USAGE_STAGING;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
                    desc.SampleDesc.Count = m_SampleCount;
                    desc.SampleDesc.Quality = sampleQuality;
                    desc.ArraySize = 1;
                    desc.MipLevels = 0;
                    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
                    desc.MiscFlags = 0;
                    desc.Format = DirectXEnumMapping::GetPixelFormat(m_DataFormat);

                    HRESULT hr = device->CreateTexture2D(&desc, NULL, &m_ShadowTexture);
                    if (FAILED(hr))
                    {
                        //log
                    }
                }

                contextImpl->CopyGpuResource(m_Texture, m_ShadowTexture);

                m_LockingTexture = m_ShadowTexture;
                return contextImpl->LockGpuResource(m_ShadowTexture, 0, mapType, true);
            }

            virtual void Unlock(RHIContextBase* inContext, const SIZE_T inIndex)
            {
                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());
                contextImpl->UnlockGpuResource(m_LockingTexture, 0);
            }

        public:
            ID3D11RenderTargetView* GetRenderTargetView() { return m_RenderTarget; }
            ID3D11DepthStencilView* GetDepthStencilView() { return m_DepthStencil; }
            ID3D11ShaderResourceView* GetShaderResourceView() { return m_ShaderResource; }
        };

        class DirectX11SwapChain : public RHISwapChain
        {
        private:
            IDXGISwapChain* m_SwapChain;

        public:
            DirectX11SwapChain(DirectX11Device* inDevice, IDXGISwapChain* inSwapChain, const RHISwapChainInitInfo& inIntiInfo)
                : RHISwapChain(inDevice, inIntiInfo),
                m_SwapChain(inSwapChain)
            {
                BladeAssert(inSwapChain != NULL);
            }

            ~DirectX11SwapChain()
            {
                if (m_SwapChain != NULL) { m_SwapChain->Release(); m_SwapChain = NULL; }
            }

        public:
            void Present()
            {
                m_SwapChain->Present(0, 0);
            }
        };

    }
}

#endif // !__BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__