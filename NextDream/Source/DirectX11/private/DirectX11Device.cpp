#include <DirectX11Device.h>
#include <DirectX11UniformBuffer.h>

namespace BladeEngine
{
    namespace RHI
    {
        RHITextureBaseRef DirectX11Device::CreateTexture2D(const RHITexture2DCreateInfo& inCreateInfo)
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

        RHIVertexShaderRef DirectX11Device::CreateVextexShader(const RHIVertexShaderCreateInfo& inCreateInfo)
        {
            BladeAssert(inCreateInfo.Defines.Length() <= MAX_SHADER_DEFINE_NUM);
            BladeAssert(inCreateInfo.CodeOffsetWithDefines.Length() == inCreateInfo.CodeSizeWithDefines.Length());

            ID3D11VertexShader* pD3D11VertexShader = NULL;
            HRESULT hr = m_pDevice->CreateVertexShader((const char*)inCreateInfo.Data, inCreateInfo.DataSize, NULL, &pD3D11VertexShader);
            if (FAILED(hr))
            {
                //Logger::Log()
                return NULL;
            }

            //return  pD3D11VertexShader;
        }

        RHIPixelShaderRef DirectX11Device::CreatePixelShader(const RHIShaderCreateInfo& inCreateInfo)
        {
        }

        RHIHullShaderRef DirectX11Device::CreateHullShader(const RHIShaderCreateInfo)
        {}

        RHIDomainShaderRef DirectX11Device::CreateDomainShader(const RHIShaderCreateInfo)
        {}

        RHIGeometryShaderRef DirectX11Device::CreateGeometryShader(const RHIShaderCreateInfo)
        {}

        RHIVertexBufferRef DirectX11Device::CreateVertexBuffer(const RHIVertexBufferCreateInfo)
        {}

        RHIIndexBufferRef DirectX11Device::CreateIndexBuffer(const RHIIndexBufferCreateInfo)
        {}

        RHIShaderStateRef DirectX11Device::CreateShaderState(const RHIShaderStateCreateInfo& inCreateInfo)
        {
            ID3D11RasterizerState* rasterizerState = NULL;
            if (m_RasterizerStateMap.TryGetValue(inCreateInfo.RasterizerDesc, &rasterizerState))
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

        RHIUniformBufferRef DirectX11Device::CreateUniformBuffer(const RHIUniformCreateInfo& inCreateInfo)
        {
            SIZE_T packDataSize = (inCreateInfo.DataSize >> 4 + 1) << 4;

            DirectX11UniformBuffer* uniformBuffer = NULL;
            for (int32 i = 0; i < m_UniformBufferList.Length; ++i)
            {
                if (m_UniformBufferList[i]->GetPackSize() > packDataSize && m_UniformBufferList[i]->IsUnique())
                {
                    uniformBuffer = m_UniformBufferList[i];
                    break;
                }
            }

            if(uniformBuffer != NULL)
            {
                if(inCreateInfo.Data != NULL)
                {
                    D3D11_MAPPED_SUBRESOURCE data = NULL;
                    HRESULT hr = m_pContext->Map(uniformBuffer->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
                    if (FAILED(hr))
                    {
                        //log
                        return NULL;
                    }

                    MemUtil::Memcopy(data.pData, uniformBuffer->GetPackSize(), inCreateInfo.Data, inCreateInfo.DataSize);
                    m_pContext->Unmap(uniformBuffer->GetBuffer(), 0);
                }
                return uniformBuffer;
            }

            D3D11_BUFFER_DESC desc = { 0 };
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.ByteWidth = packDataSize;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA data = { 0 };
            data.pSysMem = inCreateInfo.Data;
            data.SysMemPitch = inCreateInfo.DataSize;
            data.SysMemSlicePitch = 0;

            ID3D11Buffer* buffer = NULL;
            HRESULT hr = m_pDevice->CreateBuffer(&desc, &data, &buffer);
            if (FAILED(hr))
            {
                //
                return NULL;
            }

            uniformBuffer = new DirectX11UniformBuffer(
                this,
                buffer,
                packDataSize,
                inCreateInfo);

            // keep a reference
            int32 insertIndex = m_UniformBufferList.Length - 1;
            for(int32 i = 0; i < m_UniformBufferList.Length; ++ i)
            {
                if(m_UniformBufferList[i]->GetPackSize() > packDataSize)
                {
                    insertIndex = i;
                    break;
                }
            }
            uniformBuffer->AddRef();
            m_UniformBufferList.Insert(insertIndex, uniformBuffer);

            return RHIUniformBufferRef(uniformBuffer);
        }

        ID3D11RasterizerState* DirectX11Device::_CreateRasterizerState(const RHIShaderRasterizerDesc& inRasterizerDesc)
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

        ID3D11BlendState* DirectX11Device::_CreateBlendState(const RHIShaderBlendDesc& inBlendDesc)
        {
            D3D11_BLEND_DESC d3d11Desc;
            d3d11Desc.AlphaToCoverageEnable = inBlendDesc.AlphaTest;
            d3d11Desc.IndependentBlendEnable = inBlendDesc.IndependentBlendEnable;

            SIZE_T numRT = Math::Min(MAX_NUM_RENDER_TARGET, MAX_NUM_D3D11_RENDER_TARGET);
            for (int i = 0; i < numRT; ++i)
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

        ID3D11DepthStencilState* DirectX11Device::_CreateDepthStencilState(const RHIShaderDepthStencilDesc& inDepthStencilDesc)
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
    }
}