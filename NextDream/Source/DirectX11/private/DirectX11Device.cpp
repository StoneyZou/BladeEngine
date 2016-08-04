#include <DirectX11Device.h>
#include <DirectX11Context.h>
#include <DirectX11BufferBase.h>
#include <RHIDirectX11ShaderBase.h>
#include <RHIDirectX11TextureBase.h>
#include <DirectXEnumMapping.h>

namespace BladeEngine
{
    namespace RHI
    {
        RHITextureBaseRef DirectX11Device::CreateTexture2D(const RHITextureCreateInfo& inCreateInfo)
        {
            D3D11_TEXTURE2D_DESC textureDesc = { 0 };
            //tDesc.BindFlags
            textureDesc.Width = inCreateInfo.Width;
            textureDesc.Height = inCreateInfo.Height;
            textureDesc.Usage = DirectXEnumMapping::Get(inCreateInfo.AccessMode);
            textureDesc.CPUAccessFlags =
                (inCreateInfo.AccessMode & ECPU_READ ? D3D11_CPU_ACCESS_READ : 0) |
                (inCreateInfo.AccessMode & ECPU_WRITE ? D3D11_CPU_ACCESS_WRITE : 0);
            textureDesc.SampleDesc.Count = 1;
            textureDesc.SampleDesc.Quality = 0;
            textureDesc.ArraySize = 1;
            textureDesc.MipLevels = 0;
            textureDesc.BindFlags =
                (inCreateInfo.AccessMode & EGPU_READ ? D3D11_BIND_SHADER_RESOURCE : 0) |
                (inCreateInfo.AccessMode & EGPU_WRITE ? D3D11_BIND_RENDER_TARGET : 0);
            textureDesc.MiscFlags = 0;
            textureDesc.Format = DirectXEnumMapping::Get(inCreateInfo.BaseFormat);

            D3D11_SUBRESOURCE_DATA textureData = { 0 };
            textureData.pSysMem = inCreateInfo.Data;
            textureData.SysMemPitch = 0;
            textureData.SysMemSlicePitch = 0;

            ID3D11Texture2D* pD3D11Texture2D = NULL;    

            HRESULT hr = m_pDevice->CreateTexture2D(&textureDesc, &textureData, &pD3D11Texture2D);
            D3D11PtrGuard(pD3D11Texture2D);

            if (FAILED(hr))
            {
                //Logger::Log()
                return NULL;
            }

            ID3D11ShaderResourceView* pD3D11ShaderResourceView = NULL;
            ID3D11SamplerState* pSamplerState = NULL;

            if ((inCreateInfo.AccessMode & EGPU_READ) != 0)
            {
                D3D11_SHADER_RESOURCE_VIEW_DESC desc;
                desc.Format = textureDesc.Format;
                desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
                desc.Texture2D.MipLevels = 0;
                desc.Texture2D.MostDetailedMip = 0;

                hr = m_pDevice->CreateShaderResourceView(pD3D11Texture2D, &desc, &pD3D11ShaderResourceView);
                D3D11PtrGuard(pD3D11ShaderResourceView);

                if (FAILED(hr))
                {
                    //Logger::Log()
                    return NULL;
                }

                D3D11_SAMPLER_DESC samplerDesc;
                samplerDesc.AddressU = DirectXEnumMapping::Get(inCreateInfo.Sampler.AddressU);
                samplerDesc.AddressV = DirectXEnumMapping::Get(inCreateInfo.Sampler.AddressV);
                samplerDesc.AddressW = DirectXEnumMapping::Get(inCreateInfo.Sampler.AddressW);
                samplerDesc.Filter = DirectXEnumMapping::Get(inCreateInfo.Sampler.Filter);
                samplerDesc.BorderColor[0] = inCreateInfo.Sampler.BorderColor[0];
                samplerDesc.BorderColor[1] = inCreateInfo.Sampler.BorderColor[1];
                samplerDesc.BorderColor[2] = inCreateInfo.Sampler.BorderColor[2];
                samplerDesc.BorderColor[3] = inCreateInfo.Sampler.BorderColor[3];
                samplerDesc.ComparisonFunc = DirectXEnumMapping::Get(inCreateInfo.Sampler.ComparisonFunc);
                samplerDesc.MaxAnisotropy = inCreateInfo.Sampler.MaxAnisotropy;
                samplerDesc.MaxLOD = inCreateInfo.Sampler.MaxLOD;
                samplerDesc.MinLOD = inCreateInfo.Sampler.MinLOD;
                samplerDesc.MipLODBias = inCreateInfo.Sampler.MipLODBias;

                if (!m_SamplerStateMap.TryGetValue(samplerDesc, &pSamplerState))
                {
                    hr = m_pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
                    D3D11PtrGuard(pSamplerState);

                    if (FAILED(hr))
                    {
                        //Logger::Log()
                        return NULL;
                    }
                }
            }

            ID3D11RenderTargetView* pD3D11RenderTargetView = NULL;
            if ((inCreateInfo.Usage & ETEXTURE_USAGE_RENDER_TARGET) != 0)
            {
                if ((inCreateInfo.AccessMode & EGPU_WRITE) == 0)
                {
                    //Logger::Log()
                    return NULL;
                }

                D3D11_RENDER_TARGET_VIEW_DESC desc;
                desc.Format = textureDesc.Format;
                desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                desc.Texture2D.MipSlice = 0;

                hr = m_pDevice->CreateRenderTargetView(pD3D11Texture2D, &desc, &pD3D11RenderTargetView);
                D3D11PtrGuard(pD3D11RenderTargetView);

                if (FAILED(hr))
                {
                    //Logger::Log()
                    return NULL;
                }
            }

            ID3D11DepthStencilView* pD3D11DepthStencilView = NULL;
            if ((inCreateInfo.Usage & ETEXTURE_USAGE_DEPTH_STENCIL) != 0)
            {
                if ((inCreateInfo.AccessMode & EGPU_WRITE) == 0)
                {
                    //Logger::Log()
                    return NULL;
                }

                D3D11_DEPTH_STENCIL_VIEW_DESC desc;
                desc.Format = textureDesc.Format;
                desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                desc.Texture2D.MipSlice = 0;

                hr = m_pDevice->CreateDepthStencilView(pD3D11Texture2D, &desc, &pD3D11DepthStencilView);
                D3D11PtrGuard(pD3D11DepthStencilView);

                if (FAILED(hr))
                {
                    //Logger::Log()
                    return NULL;
                }
            }

            DirectX11Texture2DInitInfo initInfo;
            initInfo.Texture = pD3D11Texture2D;
            initInfo.RenderTargetView = pD3D11RenderTargetView;
            initInfo.DepthStencilView = pD3D11DepthStencilView;
            initInfo.ShaderResourceView = pD3D11ShaderResourceView;
            initInfo.CanAsRenderTarget = inCreateInfo.Usage & ETEXTURE_USAGE_RENDER_TARGET;
            initInfo.CanAsDepthStencil = inCreateInfo.Usage & ETEXTURE_USAGE_DEPTH_STENCIL;
            initInfo.Width = inCreateInfo.Width;;
            initInfo.Height = inCreateInfo.Height;
            initInfo.SampleCount = inCreateInfo.SampleCount;
            initInfo.SampleQulity = inCreateInfo.SampleQulity;
            initInfo.Usage = inCreateInfo.Usage;
            initInfo.BaseFormat = inCreateInfo.BaseFormat;
            initInfo.AccessMode = inCreateInfo.AccessMode;

            RHIDirectX11Texture2D* texture2D = new RHIDirectX11Texture2D(this, initInfo);
            return RHITextureBaseRef(texture2D);
        }

        RHIVertexShaderRef DirectX11Device::CreateVextexShader(const RHIShaderCreateInfo& inCreateInfo)
        {
            ID3D11VertexShader* pD3D11VertexShader = NULL;
            
            HRESULT hr = m_pDevice->CreateVertexShader((const char*)inCreateInfo.Data, inCreateInfo.DataSize, NULL, &pD3D11VertexShader);
            D3D11PtrGuard(pD3D11VertexShader);

            if (FAILED(hr))
            {
                //Logger::Log()
                return NULL;
            }

            DirectX11VertexShader* vertexShader = new DirectX11VertexShader(this, pD3D11VertexShader, inCreateInfo);
            return RHIVertexShaderRef(vertexShader);
        }

        RHIPixelShaderRef DirectX11Device::CreatePixelShader(const RHIShaderCreateInfo& inCreateInfo)
        {
            ID3D11PixelShader* pD3D11PixelShader = NULL;

            HRESULT hr = m_pDevice->CreatePixelShader((const char*)inCreateInfo.Data, inCreateInfo.DataSize, NULL, &pD3D11PixelShader);
            D3D11PtrGuard(pD3D11PixelShader);

            if (FAILED(hr))
            {
                //Logger::Log()
                return NULL;
            }

            DirectX11PixelShader* pixelShader = new DirectX11PixelShader(this, pD3D11PixelShader, inCreateInfo);
            return RHIPixelShaderRef(pixelShader);
        }

        RHIHullShaderRef DirectX11Device::CreateHullShader(const RHIShaderCreateInfo& inCreateInfo)
        {
            ID3D11HullShader* pD3D11HullShader = NULL;

            HRESULT hr = m_pDevice->CreateHullShader((const char*)inCreateInfo.Data, inCreateInfo.DataSize, NULL, &pD3D11HullShader);
            D3D11PtrGuard(pD3D11HullShader);

            if (FAILED(hr))
            {
                //Logger::Log()
                return NULL;
            }

            DirectX11HullShader* hullShader = new DirectX11HullShader(this, pD3D11HullShader, inCreateInfo);
            return RHIHullShaderRef(hullShader);
        }

        RHIDomainShaderRef DirectX11Device::CreateDomainShader(const RHIShaderCreateInfo& inCreateInfo)
        {
            ID3D11DomainShader* pD3D11DomainShader = NULL;

            HRESULT hr = m_pDevice->CreateDomainShader((const char*)inCreateInfo.Data, inCreateInfo.DataSize, NULL, &pD3D11DomainShader);
            D3D11PtrGuard(pD3D11DomainShader);

            if (FAILED(hr))
            {
                //Logger::Log()
                return NULL;
            }

            DirectX11DomainShader* domainShader = new DirectX11DomainShader(this, pD3D11DomainShader, inCreateInfo);
            return RHIDomainShaderRef(domainShader);
        }

        RHIGeometryShaderRef DirectX11Device::CreateGeometryShader(const RHIShaderCreateInfo& inCreateInfo)
        {
            ID3D11GeometryShader* pD3D11GeometryShader = NULL;

            HRESULT hr = m_pDevice->CreateGeometryShader((const char*)inCreateInfo.Data, inCreateInfo.DataSize, NULL, &pD3D11GeometryShader);
            D3D11PtrGuard(pD3D11GeometryShader);

            if (FAILED(hr))
            {
                //Logger::Log()
                return NULL;
            }

            DirectX11GeometryShader* geometryShader = new DirectX11GeometryShader(this, pD3D11GeometryShader, inCreateInfo);
            return RHIGeometryShaderRef(geometryShader);
        }

        RHIVertexBufferRef DirectX11Device::CreateVertexBuffer(const RHIVertexBufferCreateInfo& inCreateInfo)
        {
            ID3D11Buffer* pD3D11VertexBuffer = NULL;

            D3D11_BUFFER_DESC desc;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.ByteWidth = inCreateInfo.DataSize;
            desc.CPUAccessFlags = inCreateInfo.CanCpuWrite ? D3D11_CPU_ACCESS_WRITE : 0;
            desc.MiscFlags = 0;
            desc.StructureByteStride = inCreateInfo.DataSize / inCreateInfo.VertexNum;
            desc.Usage = inCreateInfo.CanCpuWrite ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;

            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = inCreateInfo.Data;
            data.SysMemPitch = inCreateInfo.DataSize;
            data.SysMemSlicePitch = 0;

            HRESULT hr = m_pDevice->CreateBuffer(&desc, &data, &pD3D11VertexBuffer);
            D3D11PtrGuard(pD3D11VertexBuffer);

            if (FAILED(hr))
            {
                //log
                return NULL;
            }

            DirectX11VertexBuffer* vertexBuffer = new DirectX11VertexBuffer(this, pD3D11VertexBuffer, inCreateInfo);
            return RHIVertexBufferRef(vertexBuffer);
        }

        RHIShaderStateRef DirectX11Device::CreateShaderState(const RHIShaderStateCreateInfo& inCreateInfo)
        {
            D3D11_RASTERIZER_DESC rasterizerStateDesc;
            rasterizerStateDesc.FillMode = DirectXEnumMapping::Get(inCreateInfo.RasterizerDesc.FillMode);
            rasterizerStateDesc.CullMode = DirectXEnumMapping::Get(inCreateInfo.RasterizerDesc.CullMode);
            rasterizerStateDesc.FrontCounterClockwise = inCreateInfo.RasterizerDesc.FrontCounterClockwise;
            rasterizerStateDesc.DepthBias = inCreateInfo.RasterizerDesc.DepthBias;
            rasterizerStateDesc.DepthBiasClamp = inCreateInfo.RasterizerDesc.DepthBiasClamp;
            rasterizerStateDesc.SlopeScaledDepthBias = inCreateInfo.RasterizerDesc.SlopeScaledDepthBias;
            rasterizerStateDesc.DepthClipEnable = inCreateInfo.RasterizerDesc.DepthClipEnable;
            rasterizerStateDesc.ScissorEnable = inCreateInfo.RasterizerDesc.ScissorEnable;
            rasterizerStateDesc.MultisampleEnable = inCreateInfo.RasterizerDesc.MultisampleEnable;
            rasterizerStateDesc.AntialiasedLineEnable = inCreateInfo.RasterizerDesc.AntialiasedLineEnable;

            ID3D11RasterizerState* rasterizerState = NULL;
            if (!m_RasterizerStateMap.TryGetValue(rasterizerStateDesc, &rasterizerState))
            {
                HRESULT hr = m_pDevice->CreateRasterizerState(&rasterizerStateDesc, &rasterizerState);
                D3D11PtrGuard(rasterizerState);

                if (FAILED(hr))
                {
                    //log
                    return NULL;
                }

                rasterizerState->AddRef();
                m_RasterizerStateMap.Insert(rasterizerStateDesc, rasterizerState);
            }
            D3D11PtrGuard(rasterizerState);

            D3D11_BLEND_DESC blendStateDesc;
            blendStateDesc.AlphaToCoverageEnable = inCreateInfo.BlendDesc.AlphaTest;
            blendStateDesc.IndependentBlendEnable = inCreateInfo.BlendDesc.IndependentBlendEnable;
            SIZE_T numRT = Math::Min(MAX_NUM_RENDER_TARGET, MAX_NUM_D3D11_RENDER_TARGET);
            for (SIZE_T i = 0; i < numRT; ++i)
            {
                blendStateDesc.RenderTarget[i].BlendEnable = inCreateInfo.BlendDesc.RenderTarget[i].BlendEnable;
                blendStateDesc.RenderTarget[i].RenderTargetWriteMask = inCreateInfo.BlendDesc.RenderTarget[i].RenderTargetWriteMask;

                blendStateDesc.RenderTarget[i].BlendOp = DirectXEnumMapping::Get(inCreateInfo.BlendDesc.RenderTarget[i].BlendOp);
                blendStateDesc.RenderTarget[i].SrcBlend = DirectXEnumMapping::Get(inCreateInfo.BlendDesc.RenderTarget[i].SrcBlend);
                blendStateDesc.RenderTarget[i].DestBlend = DirectXEnumMapping::Get(inCreateInfo.BlendDesc.RenderTarget[i].DestBlend);

                blendStateDesc.RenderTarget[i].BlendOpAlpha = DirectXEnumMapping::Get(inCreateInfo.BlendDesc.RenderTarget[i].BlendOpAlpha);
                blendStateDesc.RenderTarget[i].SrcBlendAlpha = DirectXEnumMapping::Get(inCreateInfo.BlendDesc.RenderTarget[i].SrcBlendAlpha);
                blendStateDesc.RenderTarget[i].DestBlendAlpha = DirectXEnumMapping::Get(inCreateInfo.BlendDesc.RenderTarget[i].DestBlendAlpha);
            }

            ID3D11BlendState* blendState = NULL;
            if (!m_BlendStateMap.TryGetValue(blendStateDesc, &blendState))
            {
                HRESULT hr = m_pDevice->CreateBlendState(&blendStateDesc, &blendState);
                if (FAILED(hr))
                {
                    //log
                    return NULL;
                }

                blendState->AddRef();
                m_BlendStateMap.Insert(blendStateDesc, blendState);
            }
            D3D11PtrGuard(blendState);

            D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
            depthStencilStateDesc.DepthEnable = inCreateInfo.DepthStencilDesc.DepthEnable;
            depthStencilStateDesc.DepthWriteMask = inCreateInfo.DepthStencilDesc.DepthEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
            depthStencilStateDesc.DepthFunc = DirectXEnumMapping::Get(inCreateInfo.DepthStencilDesc.DepthFunc);
            depthStencilStateDesc.StencilEnable = inCreateInfo.DepthStencilDesc.StencilEnable;
            depthStencilStateDesc.StencilReadMask = inCreateInfo.DepthStencilDesc.StencilReadMask;
            depthStencilStateDesc.StencilWriteMask = inCreateInfo.DepthStencilDesc.StencilWriteMask;
            depthStencilStateDesc.FrontFace.StencilFailOp = DirectXEnumMapping::Get(inCreateInfo.DepthStencilDesc.FrontFaceSFailFunc);
            depthStencilStateDesc.FrontFace.StencilDepthFailOp = DirectXEnumMapping::Get(inCreateInfo.DepthStencilDesc.FrontFaceSPassDFailFunc);
            depthStencilStateDesc.FrontFace.StencilPassOp = DirectXEnumMapping::Get(inCreateInfo.DepthStencilDesc.FrontFaceSPassDPassFunc);
            depthStencilStateDesc.FrontFace.StencilFunc = DirectXEnumMapping::Get(inCreateInfo.DepthStencilDesc.FrontFaceStencilFunc);
            depthStencilStateDesc.BackFace.StencilFailOp = DirectXEnumMapping::Get(inCreateInfo.DepthStencilDesc.BackFaceSFailFunc);
            depthStencilStateDesc.BackFace.StencilDepthFailOp = DirectXEnumMapping::Get(inCreateInfo.DepthStencilDesc.BackFaceSPassDFailFunc);
            depthStencilStateDesc.BackFace.StencilPassOp = DirectXEnumMapping::Get(inCreateInfo.DepthStencilDesc.BackFaceSPassDPassFunc);
            depthStencilStateDesc.BackFace.StencilFunc = DirectXEnumMapping::Get(inCreateInfo.DepthStencilDesc.BackFaceStencilFunc);

            ID3D11DepthStencilState* depthStencilState = NULL;
            if (!m_DepthStencilStateMap.TryGetValue(depthStencilStateDesc, &depthStencilState))
            {
                HRESULT hr = m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
                if (FAILED(hr))
                {
                    //log
                    return NULL;
                }

                depthStencilState->AddRef();
                m_DepthStencilStateMap.Insert(depthStencilStateDesc, depthStencilState);
            }
            D3D11PtrGuard(depthStencilState);

            DirectX11ShaderState* shaderState = new DirectX11ShaderState(
                this,
                rasterizerState,
                blendState,
                depthStencilState,
                inCreateInfo);

            return RHIShaderStateRef(shaderState);
        }

        RHIUniformBufferRef DirectX11Device::CreateUniformBuffer(const RHIUniformCreateInfo& inCreateInfo)
        {
            SIZE_T packDataSize = ((inCreateInfo.DataSize >> 4) + 1) << 4;

            DirectX11UniformBuffer* uniformBuffer = NULL;
            for (SIZE_T i = 0; i < m_UniformBufferList.GetLength(); ++i)
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
                    D3D11_MAPPED_SUBRESOURCE data;
                    HRESULT hr = m_pImmediateContext->Map(uniformBuffer->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
                    if (FAILED(hr))
                    {
                        //log
                        return NULL;
                    }

                    MemUtil::Memcopy(data.pData, uniformBuffer->GetPackSize(), inCreateInfo.Data, inCreateInfo.DataSize);
                    m_pImmediateContext->Unmap(uniformBuffer->GetBuffer(), 0);
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
            SIZE_T insertIndex = m_UniformBufferList.GetLength() - 1;
            for(SIZE_T i = 0; i < m_UniformBufferList.GetLength(); ++ i)
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

        RHIImmediateContextRef DirectX11Device::GetImmediateContext()
        {
            ID3D11DeviceContext* pD3D11DeviceContext = NULL;
            
            m_pDevice->GetImmediateContext(&pD3D11DeviceContext);
            D3D11PtrGuard(pD3D11DeviceContext);

            if(pD3D11DeviceContext == NULL)
            {
                //log
                return NULL;
            }

            DirectX11ImmediateContextImpl* immediateContext = new DirectX11ImmediateContextImpl(pD3D11DeviceContext);
            return RHIImmediateContextRef(new RHIImmediateContext(immediateContext));
        }

        RHIDeferredContextRef DirectX11Device::CreateDeferredContext()
        {
            ID3D11DeviceContext* pD3D11DeviceContext = NULL;

            HRESULT hr = m_pDevice->CreateDeferredContext(0, &pD3D11DeviceContext);
            D3D11PtrGuard(pD3D11DeviceContext);

            if (FAILED(hr))
            {
                //log
                return NULL;
            }

            DirectX11DeferredContextImpl* deferredContext = new DirectX11DeferredContextImpl(pD3D11DeviceContext);
            return RHIDeferredContextRef(new RHIDeferredContext(deferredContext));
        }
    }
}