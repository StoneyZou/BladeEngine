#ifndef __BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__
#define __BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__

#include <d3d11.h>
#include <RHITextureBase.h>
#include <RHIContext.h>
#include <DirectX11Context.h>
#include <DirectXEnumMapping.h>
#include <DirectX11Device.h>

namespace BladeEngine
{
    namespace RHI
    {
        struct DirectX11TextureInitInfo : public RHITextureInitInfo
        {
            ID3D11SamplerState* SamplerState;
            ID3D11Texture2D* Texture2D;
            ID3D11RenderTargetView* RenderTarget;
            ID3D11DepthStencilView* DepthStencil;
            ID3D11ShaderResourceView* ShaderResource;
        };

        struct IDirectX11TextureInterface
        {
            virtual ID3D11SamplerState* GetSamplerState() = 0;
            virtual ID3D11RenderTargetView* GetRenderTargetView() = 0;
            virtual ID3D11DepthStencilView* GetDepthStencilView() = 0;
            virtual ID3D11ShaderResourceView* GetShaderResourceView() = 0;
        };

        class RHIDirectX11Texture2D : public RHITexture2D, public IDirectX11TextureInterface
        {
        private:
            ID3D11SamplerState* m_SampleState;

            ID3D11Texture2D* m_Texture;
            ID3D11Texture2D* m_ShadowTexture;
            ID3D11Texture2D* m_LockingTexture;

            ID3D11RenderTargetView* m_RenderTarget;
            ID3D11DepthStencilView* m_DepthStencil;
            ID3D11ShaderResourceView* m_ShaderResource;

        public:
            RHIDirectX11Texture2D(
                DirectX11Device* inDevice,
                const DirectX11TextureInitInfo& inInitInfo)
                : RHITexture2D(inDevice, inInitInfo),
                m_SampleState(inInitInfo.SamplerState),
                m_Texture(inInitInfo.Texture2D),
                m_ShadowTexture(NULL),
                m_LockingTexture(NULL),
                m_RenderTarget(inInitInfo.RenderTarget),
                m_DepthStencil(inInitInfo.DepthStencil),
                m_ShaderResource(inInitInfo.ShaderResource)
            {
                BladeAssert(m_Texture != NULL);
                BladeAssert(CanAsRenderTarget() && m_RenderTarget != NULL);
                BladeAssert(CanAsDepthStencil() && m_DepthStencil != NULL);
                BladeAssert(CanAsShaderResource() && m_ShaderResource != NULL);
                BladeAssert(CanAsShaderResource() && m_SampleState != NULL);

                m_Texture->AddRef();
                if (m_RenderTarget != NULL) { m_RenderTarget->AddRef(); }
                if (m_DepthStencil != NULL) { m_DepthStencil->AddRef(); }
                if (m_ShaderResource != NULL) { m_ShaderResource->AddRef(); }
                if (m_SampleState != NULL) { m_SampleState->AddRef(); }
            }

            ~RHIDirectX11Texture2D()
            {
                if (m_Texture != NULL) { m_Texture->Release(); }
                if (m_ShadowTexture != NULL) { m_ShadowTexture->Release(); }
                if (m_RenderTarget != NULL) { m_RenderTarget->Release(); }
                if (m_DepthStencil != NULL) { m_DepthStencil->Release(); }
                if (m_ShaderResource != NULL) { m_ShaderResource->Release(); }
                if (m_SampleState != NULL) { m_SampleState->Release(); }
            }

        public:
            virtual void* Lock(RHIContextBase* inContext, ERES_LOCK_TYPE inType, const SIZE_T inIndex)
            {
                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());

                bool cantLock =
                    (((GetAccessMode() & ECPU_READ) == 0) && ((inType & ERES_LOCK_READ) != 0)) ||
                    (((GetAccessMode() & ECPU_WRITE) == 0) && ((inType & ERES_LOCK_WRITE) != 0));

                D3D11_MAP mapType = D3D11_MAP_READ_WRITE;
                mapType = (inType == ERES_LOCK_ONLY_READ ? D3D11_MAP_READ : mapType);
                mapType = (inType == ERES_LOCK_ONLY_WRITE ? D3D11_MAP_WRITE : mapType);
                mapType = (inType == ERES_LOCK_READ_WRITE ? D3D11_MAP_READ_WRITE : mapType);
                mapType = (inType == ERES_LOCK_WRITE_DISCARD ? D3D11_MAP_WRITE_DISCARD : mapType);

#if _DEBUG
                if(((GetAccessMode() & ECPU_WRITE) != 0) && inType == ERES_LOCK_ONLY_WRITE)
                {
                    //log can create texture use EONLY_GPU_READ
                }
#endif

                if (mapType == D3D11_MAP_WRITE_DISCARD && (GetAccessMode() & ECPU_WRITE) == 0)
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

                    D3D11_TEXTURE2D_DESC desc = { 0 };
                    desc.Width = m_Width;
                    desc.Height = m_Height;
                    desc.Usage = D3D11_USAGE_STAGING;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
                    desc.SampleDesc.Count = m_SampleCount;
                    desc.SampleDesc.Quality = m_SampleQulity;
                    desc.ArraySize = 1;
                    desc.MipLevels = 0;
                    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
                    desc.MiscFlags = 0;
                    desc.Format = DirectXEnumMapping::Get(m_DataFormat);

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

                m_LockingTexture = NULL;
            }

        public:
            ID3D11SamplerState* GetSamplerState() { return m_SampleState; }
            ID3D11RenderTargetView* GetRenderTargetView() { return m_RenderTarget; }
            ID3D11DepthStencilView* GetDepthStencilView() { return m_DepthStencil; }
            ID3D11ShaderResourceView* GetShaderResourceView() { return m_ShaderResource; }
        };

    }
}

#endif // !__BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__