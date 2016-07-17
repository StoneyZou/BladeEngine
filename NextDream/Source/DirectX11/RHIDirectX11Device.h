#ifndef __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
#define __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__

#include "RHIDevice.h"
#include "RHIDirectXEnumMapping.h"
#include <d3d11.h>
#include <RHITextureBase.h>
#include <RHIShaderBase.h>
#include <TMap.h>

namespace BladeEngine
{
    namespace RHI
    {
        typedef RefCountObject<ID3D11Device> ID3D11DeviceRef;
        typedef RefCountObject<ID3D11DeviceContext> ID3D11ContextRef;

        class RHIDirectX11Device : public IRHIDevice
        {
        private:
            template<typename Type>
            struct MemCompareFunc
            {
            public:
                int32 Compare(const Type& lh, const Type& rh)
                {
                    return MemUtil::Memcmp(&lh, &rh, sizeof(Type));
                }
            };

            typedef TMap<RHIShaderRasterizerDesc, ID3D11RasterizerState*, MemCompareFunc<RHIShaderRasterizerDesc>> RasterizerStateMap;
            typedef TMap<RHIShaderBlendDesc, ID3D11BlendState*, MemCompareFunc<RHIShaderBlendDesc>> BlendStateMap;
            typedef TMap<RHIShaderDepthStencilDesc, ID3D11DepthStencilState*, MemCompareFunc<RHIShaderDepthStencilDesc>> DepthStencilStateMap;

        private:
            ID3D11DeviceRef m_pDevice;
            ID3D11ContextRef m_pContext;

            RasterizerStateMap m_RasterizerStateMap;
            BlendStateMap m_BlendStateMap;
            DepthStencilStateMap m_DepthStencilStateMap;

        public:
            virtual RHITextureBaseRef CreateTexture2D(const RHITexture2DCreateInfo& inCreateInfo)
            {
                D3D11_TEXTURE2D_DESC tTextureDesc = { 0 };
                //tDesc.BindFlags
                tTextureDesc.Width = inCreateInfo.Width;
                tTextureDesc.Height = inCreateInfo.Height;
                tTextureDesc.Usage = RHIDirectXEnumMapping::Get(inCreateInfo.AccessMode);
                tTextureDesc.CPUAccessFlags = inCreateInfo.Writable ? D3D11_CPU_ACCESS_WRITE : 0;
                tTextureDesc.SampleDesc.Count = 1;
                tTextureDesc.SampleDesc.Quality = 0;
                tTextureDesc.ArraySize = 1;
                tTextureDesc.MipLevels = 0;
                tTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (inCreateInfo.AsRenderTarget ? D3D11_BIND_RENDER_TARGET : 0);
                tTextureDesc.MiscFlags = 0;
                tTextureDesc.Format = RHIDirectXEnumMapping::Get(inCreateInfo.Format);
                
                D3D11_SUBRESOURCE_DATA tTextureData = { 0 };
                tTextureData.pSysMem = inCreateInfo.Data;
                tTextureData.SysMemPitch = 0;
                tTextureData.SysMemSlicePitch = 0;
                
                ID3D11Texture2D* pD3D11Texture2D = NULL;
                HRESULT hr = m_pDevice->CreateTexture2D(&tTextureDesc, &tTextureData, &pD3D11Texture2D);

                if (FAILED(hr))
                {
                    //Logger::Log()
                    return NULL;
                }
                
                D3D11_SHADER_RESOURCE_VIEW_DESC tShaderResViewDesc;
                tShaderResViewDesc.Format = tTextureDesc.Format;
                tShaderResViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
                tShaderResViewDesc.Texture2D.MipLevels = 0;
                tShaderResViewDesc.Texture2D.MostDetailedMip = 0;
                
                ID3D11ShaderResourceView* pD3D11ShaderResourceView = NULL;
                hr = m_pDevice->CreateShaderResourceView(pD3D11Texture2D, &tShaderResViewDesc, &pD3D11ShaderResourceView);

                if (FAILED(hr))
                {
                    //Logger::Log()
                    return NULL;
                }
            }

            virtual void* Lock(RHIResourceRef& inResource, ERHIRESOURCE_LOCK_TYPE inType, const SIZE_T inIndex = 0)
            {
                if (inResource->GetAccessMode() & ECPU_READ == 0)
                {
                    //log
                    return NULL;
                }

                ID3D11Resource* d3d11Resource = (ID3D11Resource*)inResource->GetPlatformSpecificPtr();
                D3D11_MAPPED_SUBRESOURCE subResource;

                HRESULT hr = m_pContext->Map(d3d11Resource, inIndex, RHIDirectXEnumMapping::Get(inType), D3D11_MAP_FLAG_DO_NOT_WAIT, &subResource);
                if (FAILED(hr))
                {
                    //Logger::Log()
                    return NULL;
                }

                return subResource.pData;
            }

            virtual void Unlock(RHIResourceRef& inResource, const SIZE_T inIndex = 0)
            {
                ID3D11Resource* d3d11Resource = (ID3D11Resource*)inResource->GetPlatformSpecificPtr();
                m_pContext->Unmap(d3d11Resource, inIndex);
            }

            virtual void Copy(RHIResourceRef& inDest, RHIResourceRef& inSrc)
            {
                ID3D11Resource* d3d11ResourceSrc = (ID3D11Resource*)inSrc->GetPlatformSpecificPtr();
                ID3D11Resource* d3d11ResourceDest = (ID3D11Resource*)inDest->GetPlatformSpecificPtr();

                m_pContext->CopyResource(d3d11ResourceDest, d3d11ResourceSrc);
            }

            virtual RHIVertexShaderRef CreateVextexShader(const RHIShaderCreateInfo& inCreateInfo)
            {
                BladeAssert(inCreateInfo.Defines.Length() <= MAX_SHADER_DEFINE_NUM);
                BladeAssert(inCreateInfo.CodeOffsetWithDefines.Length() == inCreateInfo.CodeSizeWithDefines.Length());

                ID3D11VertexShader* pD3D11VertexShader = NULL;
                HRESULT hr = S_OK;

                for (int i = 0; i < inCreateInfo.CodeOffsetWithDefines.Length(); ++i)
                {
                    inCreateInfo.Data;
                    SIZE_T codeOffest = inCreateInfo.CodeOffsetWithDefines[i];
                    SIZE_T codeSize = inCreateInfo.CodeSizeWithDefines[i];

                    hr = m_pDevice->CreateVertexShader(((const char*)inCreateInfo.Data + codeOffest) , codeSize, NULL, &pD3D11VertexShader);
                    if (FAILED(hr))
                    {
                        //Logger::Log()
                        return NULL;
                    }
                }
            }

            virtual RHIPixelShaderRef CreatePixelShader(const RHIShaderCreateInfo& inCreateInfo)
            {
                BladeAssert(inCreateInfo.Defines.Length() <= MAX_SHADER_DEFINE_NUM);
                BladeAssert(inCreateInfo.CodeOffsetWithDefines.Length() == inCreateInfo.CodeSizeWithDefines.Length());

                ID3D11PixelShader* pD3D11PixelShader = NULL;
                HRESULT hr = S_OK;

                for (int i = 0; i < inCreateInfo.CodeOffsetWithDefines.Length(); ++i)
                {
                    inCreateInfo.Data;
                    SIZE_T codeOffest = inCreateInfo.CodeOffsetWithDefines[i];
                    SIZE_T codeSize = inCreateInfo.CodeSizeWithDefines[i];

                    hr = m_pDevice->CreatePixelShader(((const char*)inCreateInfo.Data + codeOffest), codeSize, NULL, &pD3D11PixelShader);
                    if (FAILED(hr))
                    {
                        //Logger::Log()
                        return NULL;
                    }
                }
            }

            virtual RHIHullShaderRef CreateHullShader(const RHIShaderCreateInfo) = 0;

            virtual RHIDomainShaderRef CreateDomainShader(const RHIShaderCreateInfo) = 0;

            virtual RHIGeometryShaderRef CreateGeometryShader(const RHIShaderCreateInfo) = 0;

            virtual RHIVertexBufferRef CreateVertexBuffer(const RHIVertexBufferCreateInfo) = 0;

            virtual RHIIndexBufferRef CreateIndexBuffer(const RHIIndexBufferCreateInfo) = 0;

            virtual RHIShaderStateRef CreateShaderState(const RHIShaderStateCreateInfo& inCreateInfo)
            {
                ID3D11RasterizerState* rasterizerState = NULL;
                if( m_RasterizerStateMap.TryGetValue(inCreateInfo.RasterizerDesc, &rasterizerState) )
                {
                    rasterizerState = _CreateRasterizerState(inCreateInfo.RasterizerDesc);
                    if (rasterizerState != NULL)
                    {
                        m_RasterizerStateMap.Insert(inCreateInfo.RasterizerDesc, rasterizerState);
                    }
                }
            }

            ID3D11RasterizerState* _CreateRasterizerState(const RHIShaderRasterizerDesc& inRasterizerDesc)
            {
                D3D11_RASTERIZER_DESC d3d11Desc;
                d3d11Desc.FillMode = RHIDirectXEnumMapping::Get(inRasterizerDesc.FillMode);
                d3d11Desc.CullMode = RHIDirectXEnumMapping::Get(inRasterizerDesc.CullMode);
                d3d11Desc.FrontCounterClockwise = inRasterizerDesc.FrontCounterClockwise;
                d3d11Desc.DepthBias = inRasterizerDesc.DepthBias;
                d3d11Desc.DepthBiasClamp = inRasterizerDesc.DepthBiasClamp;
                d3d11Desc.SlopeScaledDepthBias = inRasterizerDesc.SlopeScaledDepthBias;
                d3d11Desc.DepthClipEnable = inRasterizerDesc.DepthClipEnable;
                d3d11Desc.ScissorEnable = inRasterizerDesc.ScissorEnable;
                d3d11Desc.MultisampleEnable = inRasterizerDesc.MultisampleEnable;
                d3d11Desc.AntialiasedLineEnable = inRasterizerDesc.AntialiasedLineEnable;

                ID3D11RasterizerState* rasterizerState = NULL;
                HRESULT hr = m_pDevice->CreateRasterizerState(&d3d11Desc, &rasterizerState);
                if (FAILED(hr))
                {
                    //log
                    return NULL;
                }

                return rasterizerState;
            }

            ID3D11BlendState* _CreateBlendState(const RHIShaderBlendDesc& inBlendDesc)
            {
                D3D11_BLEND_DESC d3d11Desc;
                d3d11Desc.AlphaToCoverageEnable = inBlendDesc.AlphaTest;
                d3d11Desc.IndependentBlendEnable = inBlendDesc.IndependentBlendEnable;


                ID3D11RasterizerState* rasterizerState = NULL;
                HRESULT hr = m_pDevice->CreateRasterizerState(&d3d11Desc, &rasterizerState);
                if (FAILED(hr))
                {
                    //log
                    return NULL;
                }

                return rasterizerState;
            }
        };
    }
}

#endif // !__BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
