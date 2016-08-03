#ifndef __BLADE_RHI_DIRECTX11_CONTEXT_H__
#define __BLADE_RHI_DIRECTX11_CONTEXT_H__

#include <d3d11.h>
#include <RHIContext.h>
#include <RHIDirectX11ShaderBase.h>
#include <RHIDirectX11TextureBase.h>
#include <DirectX11BufferBase.h>

namespace BladeEngine
{
    namespace RHI
    {
        struct MyStruct
        {

        };

        class DirectX11ContextBaseImpl : public IRHIContextBaseImpl
        {
        private:
            static const SIZE_T MAX_SHADER_RESOURCE_CACHE_NUM = 8;
            static const SIZE_T MAX_RENDER_TARGET_NUM = 8;
            static const FLOAT BlendColor[4];
            static const UINT8 SamplerMask = 0;

        private:
            struct ContextStateCahce
            {
                RHIVertexShader* VS;
                RHIGeometryShader* GS;
                RHIHullShader* HS;
                RHIDomainShader* DS;
                RHIPixelShader* PS;

                ID3D11RasterizerState* RasterizerState;
                ID3D11BlendState* BlendState;
                ID3D11DepthStencilState* DepthStencilState;

                ID3D11DepthStencilView* DepthStencilView;
                ID3D11RenderTargetView* RenderTargetViews[MAX_RENDER_TARGET_NUM];

                ID3D11Buffer* UniformBuffers[MAX_SHADER_RESOURCE_CACHE_NUM];
                ID3D11ShaderResourceView* Textures[MAX_SHADER_RESOURCE_CACHE_NUM];
                ID3D11SamplerState* Samplers[MAX_SHADER_RESOURCE_CACHE_NUM];

                RHIVertexBuffer* VertexBuffer;
            };

        protected:
            ID3D11DeviceContext* m_Context;
            ContextStateCahce m_StateCahce;
            TArray<D3D11_INPUT_ELEMENT_DESC> m_InputElementDescs;

        public:
            DirectX11ContextBaseImpl(ID3D11DeviceContext* mContext)
            {
                m_Context->AddRef();
            }

            ~DirectX11ContextBaseImpl()
            {
                m_Context->Release();
            }

        public:
            virtual void SetVertexShader(RHIVertexShader* inRHIShader) 
            {
                DirectX11VertexShader* shader = (DirectX11VertexShader*)inRHIShader;
                if (m_StateCahce.VS != shader->GetShader())
                {
                    m_Context->VSSetShader(shader->GetShader(), NULL, 0);
                    m_StateCahce.VS = shader->GetShader();
                }
            }

            virtual void SetGeometryShader(RHIGeometryShader* inRHIShader)
            {
                DirectX11GeometryShader* shader = (DirectX11GeometryShader*)inRHIShader;
                if (m_StateCahce.GS != shader->GetShader())
                {
                    m_Context->GSSetShader(shader->GetShader(), NULL, 0);
                    m_StateCahce.GS = shader->GetShader();
                }
            }

            virtual void SetHullShader(RHIHullShader* inRHIShader)
            {
                DirectX11HullShader* shader = (DirectX11HullShader*)inRHIShader;
                if (m_StateCahce.HS != shader->GetShader())
                {
                    m_Context->HSSetShader(shader->GetShader(), NULL, 0);
                    m_StateCahce.HS = shader->GetShader();
                }
            }

            virtual void SetDomainShader(RHIDomainShader* inRHIShader)
            {
                DirectX11DomainShader* shader = (DirectX11DomainShader*)inRHIShader;
                if (m_StateCahce.DS != shader->GetShader())
                {
                    m_Context->DSSetShader(shader->GetShader(), NULL, 0);
                    m_StateCahce.DS = shader->GetShader();
                }
            }

            virtual void SetPixelShader(RHIPixelShader* inRHIShader)
            {
                DirectX11PixelShader* shader = (DirectX11PixelShader*)inRHIShader;
                if (m_StateCahce.PS != shader->GetShader())
                {
                    m_Context->PSSetShader(shader->GetShader(), NULL, 0);
                    m_StateCahce.PS = shader->GetShader();
                }
            }

            virtual void SetShaderState(RHIShaderState* inRHIShaderState)
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

            virtual void SetShaderResource(RHITextureBase* inTexture, ESHADER_TYPE inType, SIZE_T inSlot)
            {
                BladeAssert(inSlot >= 0);

                ID3D11ShaderResourceView* shaderResourceView = ((IDirectX11TextureInterface*)inTexture)->GetShaderResourceView();
                if(inSlot >= 0 && inSlot < SHADER_RESOURCE_CACHE_NUM)
                {
                    if (m_StateCahce.Textures[inSlot] == shaderResourceView)
                    {
                        return;
                    }
                }
                m_StateCahce.Textures[inSlot] = shaderResourceView;

                switch (inType)
                {
                case BladeEngine::RHI::ESHADER_VERTEX:
                    m_Context->VSSetShaderResources(inSlot, 1, &shaderResourceView);
                    break;
                case BladeEngine::RHI::ESHADER_HULL:
                    m_Context->HSSetShaderResources(inSlot, 1, &shaderResourceView);
                    break;
                case BladeEngine::RHI::ESHADER_DOMAIN:
                    m_Context->DSSetShaderResources(inSlot, 1, &shaderResourceView);
                    break;
                case BladeEngine::RHI::ESHADER_GEOMETRY:
                    m_Context->GSSetShaderResources(inSlot, 1, &shaderResourceView);
                    break;
                case BladeEngine::RHI::ESHADER_PIXEL:
                    m_Context->PSSetShaderResources(inSlot, 1, &shaderResourceView);
                    break;
                default:
                    break;
                }
            }

            virtual void SetRenderTarget(RHITextureBase* inTex, SIZE_T inIndex = 0)
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

                ID3D11RenderTargetView* renderTargetViewView = ((IDirectX11TextureInterface*)inTex)->GetRenderTargetView();
                m_StateCahce.RenderTargetViews[inIndex] = renderTargetViewView;
                m_Context->OMSetRenderTargets(MAX_RENDER_TARGET_NUM, m_StateCahce.RenderTargetViews, m_StateCahce.DepthStencilView);
            }

            virtual void SetDepthStencil(RHITextureBase* inTex)
            {
                if (!inTex->CanAsDepthStencil())
                {
                    //log
                    return;
                }

                ID3D11DepthStencilView* depthStencilView = ((IDirectX11TextureInterface*)inTex)->GetDepthStencilView();
                m_StateCahce.DepthStencilView = depthStencilView;
                m_Context->OMSetRenderTargets(MAX_RENDER_TARGET_NUM£¬ m_StateCahce.RenderTargetViews, m_StateCahce.DepthStencilView);
            }

            virtual void ClearRenderTarget(const BColor& inColor)
            {
                if (m_StateCahce.RenderTargetViews != NULL)
                {
                    m_Context->ClearRenderTargetView(m_StateCahce.RenderTargetViews, inColor._data);
                }
            }

            virtual void ClearDepthStencil(float inDepthValue = 0.0f, uint32 inStencilValue = 0)
            {
                if (m_StateCahce.DepthStencilView != NULL)
                {
                    m_Context->ClearDepthStencilView(m_StateCahce->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, inDepthValue, inStencilValue);
                }
            }

            virtual void SetVertexBuufer(RHIVertexBuffer* inVertexBuffer, uint32 inStride, uint32 inOffset, uint32 inSlot = 0)
            {
                ID3D11Buffer* vertexBuffer = ((DirectX11VertexBuffer*)inVertexBuffer)->GetBuffer();
                if (vertexBuffer == NULL)
                {
                    //log
                    return;
                }

                m_StateCahce.VertexBuffer = vertexBuffer;
                m_Context->IASetVertexBuffers(inSlot, 1, &vertexBuffer, &inStride, &inOffset);
            }

            virtual void DrawAuto()
            {
                if(m_StateCahce.VertexBuffer == NULL || m_StateCahce.VS == NULL || m_StateCahce.PS == NULL)
                {
                    return;
                }

                const RHIVertexShader::InputTable& inputTable = m_StateCahce.VS->GetInputTable();
                const RHIVertexBuffer::DeclarationTable& declarationTable = m_StateCahce.VertexBuffer->GetDeclarationTable();

                for(int iTableIndex = 0; iTableIndex< inputTable.GetLength(); ++ iTableIndex)
                {
                    for( int dTableIndex = 0; dTableIndex < declarationTable.GetLength(); ++ dTableIndex)
                    {
                        if (declarationTable[dTableIndex].Semantic == inputTable[iTableIndex].Semantic ||
                            declarationTable[dTableIndex].Index == declarationTable[dTableIndex].Index)
                        {
                            D3D11_INPUT_ELEMENT_DESC desc = 
                            {
                                ShaderSemanticName[declarationTable[dTableIndex].Semantic],
                                declarationTable[dTableIndex].Index,
                                D3D11 declarationTable[dTableIndex].Format,
                            }
                        }
                    }
                }

                m_Context->DrawAuto();
            }

            virtual void Draw(uint32 inVertexNum, uint32 inVertexOffset)
            {
                if (m_StateCahce.VertexBuffer == NULL)
                {
                    return;
                }

                m_Context->Draw(inVertexNum, inVertexOffset);
            }

        public:
            void* LockGpuResource(ID3D11Resource* inResource, uint32 inIndex, D3D11_MAP inMapType, bool inWaitForGPU)
            {
                D3D11_MAPPED_SUBRESOURCE data;
                HRESULT hr = m_Context->Map(inResource, inIndex, inMapType, inWaitForGPU ? 0 : D3D11_MAP_FLAG_DO_NOT_WAIT, &data);
                if( FAILED(hr) )
                {
                    // log
                    return NULL;
                }
                return data.pData;
            }

            void UnlockGpuResource(ID3D11Resource* inResource, uint32 inIndex)
            {
                m_Context->Unmap(inResource, inIndex);
            }

            void CopyGpuResource(ID3D11Resource* inDest, ID3D11Resource* inSrc)
            {
                m_Context->CopyResource(inDest, inSrc);
            }

            ID3D11Device* GetDevice() 
            { 
                ID3D11Device* device;
                m_Context->GetDevice(&device);
                return device;
            }
        };

        const FLOAT DirectX11ContextBaseImpl::BlendColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        class DirectX11ImmediateContextImpl : public DirectX11ContextBaseImpl
        {
        public:
            virtual void Flush()
            {
                // no thing, becase command is finish Immediatly;
            }
        };

        class DirectX11DeferredContextImpl : public DirectX11ContextBaseImpl
        {
        public:
            virtual void Flush()
            {
                m_Context->Flush();
            }
        };
    }
}

#endif // !__BLADE_RHI_DIRECTX11_CONTEXT_H__