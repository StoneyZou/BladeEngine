#include <DirectX11Context.h>
#include <DirectX11BufferBase.h>
#include <DirectX11ShaderBase.h>
#include <DirectX11TextureBase.h>

namespace BladeEngine
{

    const FLOAT DirectX11ContextBaseImpl::BlendColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    void DirectX11ContextBaseImpl::SetVertexShader(RHIVertexShader* inRHIShader)
    {
        DirectX11VertexShader* shader = (DirectX11VertexShader*)inRHIShader;
        if (m_StateCahce.VS != shader)
        {
            m_Context->VSSetShader(shader->GetShader(), NULL, 0);
            m_StateCahce.VS = shader;
        }
    }

    void DirectX11ContextBaseImpl::SetGeometryShader(RHIGeometryShader* inRHIShader)
    {
        DirectX11GeometryShader* shader = (DirectX11GeometryShader*)inRHIShader;
        if (m_StateCahce.GS != shader)
        {
            m_Context->GSSetShader(shader->GetShader(), NULL, 0);
            m_StateCahce.GS = shader;
        }
    }

    void DirectX11ContextBaseImpl::SetHullShader(RHIHullShader* inRHIShader)
    {
        DirectX11HullShader* shader = (DirectX11HullShader*)inRHIShader;
        if (m_StateCahce.HS != shader)
        {
            m_Context->HSSetShader(shader->GetShader(), NULL, 0);
            m_StateCahce.HS = shader;
        }
    }

    void DirectX11ContextBaseImpl::SetDomainShader(RHIDomainShader* inRHIShader)
    {
        DirectX11DomainShader* shader = (DirectX11DomainShader*)inRHIShader;
        if (m_StateCahce.DS != shader)
        {
            m_Context->DSSetShader(shader->GetShader(), NULL, 0);
            m_StateCahce.DS = shader;
        }
    }

    void DirectX11ContextBaseImpl::SetPixelShader(RHIPixelShader* inRHIShader)
    {
        DirectX11PixelShader* shader = (DirectX11PixelShader*)inRHIShader;
        if (m_StateCahce.PS != shader)
        {
            m_Context->PSSetShader(shader->GetShader(), NULL, 0);
            m_StateCahce.PS = shader;
        }
    }

    void DirectX11ContextBaseImpl::SetShaderState(RHIShaderState* inRHIShaderState)
    {
        DirectX11ShaderState* state = (DirectX11ShaderState*)inRHIShaderState;

        if (m_StateCahce.RasterizerState != state->GetRasterizerState())
        {
            m_Context->RSSetState(state->GetRasterizerState());
            m_StateCahce.RasterizerState = state->GetRasterizerState();
        }

        if (m_StateCahce.BlendState != state->GetBlendState())
        {
            m_Context->OMSetBlendState(state->GetBlendState(), BlendColor, SamplerMask);
            m_StateCahce.BlendState = state->GetBlendState();
        }

        if (m_StateCahce.DepthStencilState != state->GetDepthStencilState())
        {
            m_Context->OMSetDepthStencilState(state->GetDepthStencilState(), 0);
            m_StateCahce.DepthStencilState = state->GetDepthStencilState();
        }
    }

    void DirectX11ContextBaseImpl::SetShaderResource(RHITextureBase* inTexture, ESHADER_TYPE inType, SIZE_T inSlot)
    {
        BladeAssert(inSlot >= 0);

        ID3D11ShaderResourceView* shaderResourceView = ((IDirectX11TextureInterface*)inTexture)->GetShaderResourceView();
        if (inSlot >= 0 && inSlot < MAX_SHADER_RESOURCE_CACHE_NUM)
        {
            if (m_StateCahce.Textures[inSlot] == shaderResourceView)
            {
                return;
            }
        }
        m_StateCahce.Textures[inSlot] = shaderResourceView;

        switch (inType)
        {
        case BladeEngine::ESHADER_VERTEX:
            m_Context->VSSetShaderResources(inSlot, 1, &shaderResourceView);
            break;
        case BladeEngine::ESHADER_HULL:
            m_Context->HSSetShaderResources(inSlot, 1, &shaderResourceView);
            break;
        case BladeEngine::ESHADER_DOMAIN:
            m_Context->DSSetShaderResources(inSlot, 1, &shaderResourceView);
            break;
        case BladeEngine::ESHADER_GEOMETRY:
            m_Context->GSSetShaderResources(inSlot, 1, &shaderResourceView);
            break;
        case BladeEngine::ESHADER_PIXEL:
            m_Context->PSSetShaderResources(inSlot, 1, &shaderResourceView);
            break;
        default:
            break;
        }
    }

    void DirectX11ContextBaseImpl::SetRenderTarget(RHITextureBase* inTex, SIZE_T inIndex)
    {
        if (!inTex->CanAsRenderTarget())
        {
            //log
            return;
        }

        if (inIndex < 0 || inIndex >= MAX_RENDER_TARGET_NUM)
        {
            //log
            return;
        }

        ID3D11RenderTargetView* renderTargetViewView = ((IDirectX11TextureInterface*)((DirectX11Texture2D*)inTex))->GetRenderTargetView();
        m_StateCahce.RenderTargetViews[inIndex] = renderTargetViewView;
        m_Context->OMSetRenderTargets(1, &m_StateCahce.RenderTargetViews[0], m_StateCahce.DepthStencilView);
    }

    void DirectX11ContextBaseImpl::SetDepthStencil(RHITextureBase* inTex)
    {
        if (!inTex->CanAsDepthStencil())
        {
            //log
            return;
        }

        ID3D11DepthStencilView* depthStencilView = ((IDirectX11TextureInterface*)inTex)->GetDepthStencilView();
        m_StateCahce.DepthStencilView = depthStencilView;
        m_Context->OMSetRenderTargets(MAX_RENDER_TARGET_NUM, m_StateCahce.RenderTargetViews, m_StateCahce.DepthStencilView);
    }

    void DirectX11ContextBaseImpl::ClearRenderTarget(const BColor& inColor)
    {
        if (m_StateCahce.RenderTargetViews != NULL)
        {
            for (int i = 0; i < MAX_RENDER_TARGET_NUM; ++i)
            {
                if (m_StateCahce.RenderTargetViews[i] != NULL)
                {
                    m_Context->ClearRenderTargetView(m_StateCahce.RenderTargetViews[i], inColor._data);
                }
            }
        }
    }

    void DirectX11ContextBaseImpl::ClearDepthStencil(float inDepthValue, uint32 inStencilValue)
    {
        if (m_StateCahce.DepthStencilView != NULL)
        {
            m_Context->ClearDepthStencilView(m_StateCahce.DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, inDepthValue, inStencilValue);
        }
    }

    void DirectX11ContextBaseImpl::SetVertexBuufer(RHIVertexBuffer* inVertexBuffer, uint32 inStride, uint32 inOffset, uint32 inSlot)
    {
        ID3D11Buffer* vertexBuffer = ((DirectX11VertexBuffer*)inVertexBuffer)->GetBuffer();
        if (vertexBuffer == NULL)
        {
            //log
            return;
        }

        m_StateCahce.VertexBuffer = inVertexBuffer;
        m_Context->IASetVertexBuffers(inSlot, 1, &vertexBuffer, &inStride, &inOffset);
    }

    void DirectX11ContextBaseImpl::DrawAuto()
    {
        if (m_StateCahce.VertexBuffer == NULL || m_StateCahce.VS == NULL || m_StateCahce.PS == NULL)
        {
            return;
        }

        const RHIShaderInputTable* inputTable = m_StateCahce.VS->GetInputTable();
        const RHIVertexBuffer::DeclarationTable& declarationTable = m_StateCahce.VertexBuffer->GetDeclarationTable();

        uint32 inputElementDescIndex = 0;
        m_InputElementDescs.Resize(inputTable->m_InputTable.Size());

        for (SIZE_T iTableIndex = 0; iTableIndex < inputTable->m_InputTable.Size(); ++iTableIndex)
        {
            bool foundSemantic = false;
            for (SIZE_T dTableIndex = 0; dTableIndex < declarationTable.Size(); ++dTableIndex)
            {
                if (declarationTable[dTableIndex].Semantic == inputTable->m_InputTable[iTableIndex].Semantic ||
                    declarationTable[dTableIndex].Index == declarationTable[dTableIndex].Index)
                {
                    m_InputElementDescs[inputElementDescIndex].SemanticName = ShaderSemanticNames[declarationTable[dTableIndex].Semantic];
                    m_InputElementDescs[inputElementDescIndex].SemanticIndex = declarationTable[dTableIndex].Index;
                    m_InputElementDescs[inputElementDescIndex].Format = DirectXEnumMapping::GetDataFormat(declarationTable[dTableIndex].Format);
                    m_InputElementDescs[inputElementDescIndex].InputSlot = inputTable->m_InputTable[iTableIndex].Slot;
                    m_InputElementDescs[inputElementDescIndex].AlignedByteOffset = declarationTable[dTableIndex].Offset;
                    m_InputElementDescs[inputElementDescIndex].InputSlotClass = DirectXEnumMapping::GetInputClassification(declarationTable[dTableIndex].InputType);
                    m_InputElementDescs[inputElementDescIndex].InstanceDataStepRate = declarationTable[dTableIndex].InstanceDataStepRate;

                    ++inputElementDescIndex;
                    foundSemantic = true;

                    break;
                }
            }

            if (!foundSemantic)
            {
                return;
            }
        }

        ID3D11Device* device = NULL;
        m_Context->GetDevice(&device);

        ID3D11InputLayout* inputLayout = NULL;
        HRESULT hr = device->CreateInputLayout(m_InputElementDescs.TypePtr(), inputElementDescIndex, NULL, 0, &inputLayout);
        if (FAILED(hr))
        {
            return;
        }

        m_Context->IASetInputLayout(inputLayout);
        inputLayout->Release();

        m_Context->DrawAuto();
    }

    void DirectX11ContextBaseImpl::Draw(uint32 inVertexNum, uint32 inVertexOffset)
    {
        if (m_StateCahce.VertexBuffer == NULL)
        {
            return;
        }

        m_Context->Draw(inVertexNum, inVertexOffset);
    }

    void DirectX11ContextBaseImpl::SetViewport(SIZE_T inIndex, float inLeft, float inTop, float inWidth, float inHeight,
        float inMinDepth, float inMaxDepth)
    {
        if (inIndex < 0 || inIndex >= MAX_RENDER_TARGET_NUM)
        {
            //log
            return;
        }

        D3D11_VIEWPORT viewport = { 0 };
        FLOAT TopLeftX = inLeft;
        FLOAT TopLeftY = inTop;
        FLOAT Width = inWidth;
        FLOAT Height = inHeight;
        FLOAT MinDepth = inMinDepth;
        FLOAT MaxDepth = inMaxDepth;

        m_StateCahce.Viewports[inIndex] = viewport;

        m_Context->RSSetViewports(1, &m_StateCahce.Viewports[0]);
    }
}
