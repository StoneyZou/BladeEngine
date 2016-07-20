#ifndef __BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__
#define __BLADE_RHI_DIRECTX11_TEXTURE_BASE_H__

#include <d3d11.h>
#include <RHITextureBase.h>
#include <RHIContext.h>
#include <RHIDirectX11Context.h>
#include <RHIDirectXEnumMapping.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHIDirectX11Texture2D : public RHITexture2D
        {
        private:
            ID3D11Texture2D* m_Texture;
            ID3D11Texture2D* m_ShadowTexture;

            ID3D11RenderTargetView* m_RenderTarget;
            ID3D11ShaderResourceView* m_ShaderResource;

            ID3D11Texture2D* m_LockingTexture;

        public:
            RHIDirectX11Texture2D(ID3D11Texture2D* inTexture2D, ID3D11RenderTargetView* inRenderTarget,
                ID3D11ShaderResourceView* inShaderResource, const RHITexture2DCreateInfo& inCreateInfo)
                : RHITexture2D(inCreateInfo),
                m_Texture(inTexture2D),
                m_RenderTarget(inRenderTarget),
                m_ShaderResource(inShaderResource)
            {
                BladeAssert(inTexture2D != NULL);
                BladeAssert(GetAccessMode() & EGPU_WRITE != 0 && m_RenderTarget != NULL);
                BladeAssert(GetAccessMode() & EGPU_READ != 0 && m_ShaderResource != NULL);

                m_Texture->AddRef();
                if (GetAccessMode() & EGPU_WRITE != 0)
                    m_RenderTarget->AddRef();
                if (GetAccessMode() & EGPU_READ != 0)
                    m_ShaderResource->AddRef();
            }

            ~RHIDirectX11Texture2D()
            {
                m_Texture->Release();
                if (GetAccessMode() & EGPU_WRITE != 0)
                    m_RenderTarget->Release();
                if (GetAccessMode() & EGPU_READ != 0)
                    m_ShaderResource->Release();
            }

        public:
            virtual void* Lock(RHIContextBase* inContext, ERES_LOCK_TYPE inType, const SIZE_T inIndex)
            {
                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());

                bool cantLock =
                    ((GetAccessMode() & ECPU_READ == 0) && (inType & ERES_LOCK_READ != 0)) ||
                    ((GetAccessMode() & ECPU_WRITE == 0) && (inType & ERES_LOCK_WRITE != 0));

                D3D11_MAP mapType = D3D11_MAP_READ_WRITE;
                mapType = (inType == ERES_LOCK_ONLY_READ ? D3D11_MAP_READ : mapType);
                mapType = (inType == ERES_LOCK_ONLY_WRITE ? D3D11_MAP_WRITE : mapType);
                mapType = (inType == ERES_LOCK_READ_WRITE ? D3D11_MAP_READ_WRITE : mapType);
                mapType = (inType == ERES_LOCK_WRITE_DISCARD ? D3D11_MAP_WRITE_DISCARD : mapType);

#if _DEBUG
                if((GetAccessMode() & ECPU_WRITE != 0) && inType == ERES_LOCK_ONLY_WRITE)
                {
                    //log can create texture use EONLY_GPU_READ
                }
#endif

                if (mapType == D3D11_MAP_WRITE_DISCARD && GetAccessMode() & ECPU_WRITE == 0)
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
                    desc.Format = RHIDirectXEnumMapping::Get(m_PixelFormat);

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
            ID3D11Texture2D* GetTexture()
            {
                return m_Texture;
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