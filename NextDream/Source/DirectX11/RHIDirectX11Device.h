#ifndef __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
#define __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__

#include "RHIDevice.h"
#include "RHIDirectXEnumMapping.h"
#include <d3d11.h>
#include <RHITextureBase.h>
#include <RHIShaderBase.h>
#include <TMap.h>
#include <BMath.h>
#include <RHIDirectX11ShaderBase.h>

namespace BladeEngine
{
    namespace RHI
    {
        #define MAX_NUM_D3D11_RENDER_TARGET 8

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
                tTextureDesc.CPUAccessFlags =
                    (inCreateInfo.AccessMode & ECPU_READ ? D3D11_CPU_ACCESS_READ : 0) |
                    (inCreateInfo.AccessMode & ECPU_WRITE ? D3D11_CPU_ACCESS_WRITE : 0);
                tTextureDesc.SampleDesc.Count = 1;
                tTextureDesc.SampleDesc.Quality = 0;
                tTextureDesc.ArraySize = 1;
                tTextureDesc.MipLevels = 0;
                tTextureDesc.BindFlags =
                    (inCreateInfo.AccessMode & EGPU_READ ? D3D11_BIND_SHADER_RESOURCE : 0) |
                    (inCreateInfo.AccessMode & EGPU_WRITE ? D3D11_BIND_RENDER_TARGET : 0);
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

                ID3D11ShaderResourceView* pD3D11ShaderResourceView = NULL;
                if (inCreateInfo.AccessMode & EGPU_READ != 0)
                {
                    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
                    desc.Format = tTextureDesc.Format;
                    desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
                    desc.Texture2D.MipLevels = 0;
                    desc.Texture2D.MostDetailedMip = 0;

                    hr = m_pDevice->CreateShaderResourceView(pD3D11Texture2D, &desc, &pD3D11ShaderResourceView);
                    if (FAILED(hr))
                    {
                        //Logger::Log()
                        return NULL;
                    }
                }

                ID3D11RenderTargetView* pD3D11RenderTargetView = NULL;
                if (inCreateInfo.AccessMode & EGPU_WRITE != 0)
                {
                    D3D11_RENDER_TARGET_VIEW_DESC desc;
                    desc.Format = tTextureDesc.Format;
                    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                    desc.Texture2D.MipSlice = 0;

                    hr = m_pDevice->CreateRenderTargetView(pD3D11Texture2D, &desc, &pD3D11RenderTargetView);
                    if (FAILED(hr))
                    {
                        //Logger::Log()
                        return NULL;
                    }
                }


            }

            virtual RHIVertexShaderRef CreateVextexShader(const RHIVertexShaderCreateInfo& inCreateInfo)
            {
                BladeAssert(inCreateInfo.Defines.Length() <= MAX_SHADER_DEFINE_NUM);
                BladeAssert(inCreateInfo.CodeOffsetWithDefines.Length() == inCreateInfo.CodeSizeWithDefines.Length());

                ID3D11VertexShader* pD3D11VertexShader = NULL;
                HRESULT hr = m_pDevice->CreateVertexShader((const char*)inCreateInfo.Data , inCreateInfo.DataSize, NULL, &pD3D11VertexShader);
                if (FAILED(hr))
                {
                    //Logger::Log()
                    return NULL;
                }

                //return  pD3D11VertexShader;
            }

            virtual RHIPixelShaderRef CreatePixelShader(const RHIShaderCreateInfo& inCreateInfo)
            {
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

                ID3D11BlendState* blendState = NULL;
                if (m_BlendStateMap.TryGetValue(inCreateInfo.BlendDesc, &blendState))
                {
                    blendState = _CreateBlendState(inCreateInfo.BlendDesc);
                    if (blendState != NULL)
                    {
                        m_BlendStateMap.Insert(inCreateInfo.BlendDesc, blendState);
                    }
                }

                ID3D11DepthStencilState* depthStencilState = NULL;
                if (m_DepthStencilStateMap.TryGetValue(inCreateInfo.DepthStencilDesc, &depthStencilState))
                {
                    depthStencilState = _CreateDepthStencilState(inCreateInfo.DepthStencilDesc);
                    if (depthStencilState != NULL)
                    {
                        m_DepthStencilStateMap.Insert(inCreateInfo.DepthStencilDesc, depthStencilState);
                    }
                }

                DirectX11ShaderState* shaderState = new DirectX11ShaderState(
                    rasterizerState,
                    blendState,
                    depthStencilState,
                    inCreateInfo);

                return RHIShaderStateRef(shaderState);
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
                
                SIZE_T numRT = Math::Min(MAX_NUM_RENDER_TARGET, MAX_NUM_D3D11_RENDER_TARGET);
                for( int i = 0; i < numRT; ++ i)
                {
                    d3d11Desc.RenderTarget[i].BlendEnable = inBlendDesc.RenderTarget[i].BlendEnable;
                    d3d11Desc.RenderTarget[i].RenderTargetWriteMask = inBlendDesc.RenderTarget[i].RenderTargetWriteMask;

                    d3d11Desc.RenderTarget[i].BlendOp = RHIDirectXEnumMapping::Get(inBlendDesc.RenderTarget[i].BlendOp);
                    d3d11Desc.RenderTarget[i].SrcBlend = RHIDirectXEnumMapping::Get(inBlendDesc.RenderTarget[i].SrcBlend);
                    d3d11Desc.RenderTarget[i].DestBlend = RHIDirectXEnumMapping::Get(inBlendDesc.RenderTarget[i].DestBlend);

                    d3d11Desc.RenderTarget[i].BlendOpAlpha = RHIDirectXEnumMapping::Get(inBlendDesc.RenderTarget[i].BlendOpAlpha);
                    d3d11Desc.RenderTarget[i].SrcBlendAlpha = RHIDirectXEnumMapping::Get(inBlendDesc.RenderTarget[i].SrcBlendAlpha);
                    d3d11Desc.RenderTarget[i].DestBlendAlpha = RHIDirectXEnumMapping::Get(inBlendDesc.RenderTarget[i].DestBlendAlpha);
                }


                ID3D11BlendState* blendState = NULL;
                HRESULT hr = m_pDevice->CreateBlendState(&d3d11Desc, &blendState);
                if (FAILED(hr))
                {
                    //log
                    return NULL;
                }

                return blendState;
            }

            ID3D11DepthStencilState* _CreateDepthStencilState(const RHIShaderDepthStencilDesc& inDepthStencilDesc)
            {
                D3D11_DEPTH_STENCIL_DESC d3d11Desc;
                d3d11Desc.DepthEnable = inDepthStencilDesc.DepthEnable;
                d3d11Desc.DepthWriteMask = inDepthStencilDesc.DepthEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
                d3d11Desc.DepthFunc = RHIDirectXEnumMapping::Get(inDepthStencilDesc.DepthFunc);
                d3d11Desc.StencilEnable = inDepthStencilDesc.StencilEnable;
                d3d11Desc.StencilReadMask = inDepthStencilDesc.StencilReadMask;
                d3d11Desc.StencilWriteMask = inDepthStencilDesc.StencilWriteMask;
                d3d11Desc.FrontFace.StencilFailOp = RHIDirectXEnumMapping::Get(inDepthStencilDesc.FrontFaceSFailFunc);
                d3d11Desc.FrontFace.StencilDepthFailOp = RHIDirectXEnumMapping::Get(inDepthStencilDesc.FrontFaceSPassDFailFunc);
                d3d11Desc.FrontFace.StencilPassOp = RHIDirectXEnumMapping::Get(inDepthStencilDesc.FrontFaceSPassDPassFunc);
                d3d11Desc.FrontFace.StencilFunc = RHIDirectXEnumMapping::Get(inDepthStencilDesc.FrontFaceStencilFunc);
                d3d11Desc.BackFace.StencilFailOp = RHIDirectXEnumMapping::Get(inDepthStencilDesc.BackFaceSFailFunc);
                d3d11Desc.BackFace.StencilDepthFailOp = RHIDirectXEnumMapping::Get(inDepthStencilDesc.BackFaceSPassDFailFunc);
                d3d11Desc.BackFace.StencilPassOp = RHIDirectXEnumMapping::Get(inDepthStencilDesc.BackFaceSPassDPassFunc);
                d3d11Desc.BackFace.StencilFunc = RHIDirectXEnumMapping::Get(inDepthStencilDesc.BackFaceStencilFunc);

                ID3D11DepthStencilState* depthStencilState = NULL;
                HRESULT hr = m_pDevice->CreateDepthStencilState(&d3d11Desc, &depthStencilState);
                if (FAILED(hr))
                {
                    //log
                    return NULL;
                }

                return depthStencilState;
            }
        };
    }
}

#endif // !__BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
