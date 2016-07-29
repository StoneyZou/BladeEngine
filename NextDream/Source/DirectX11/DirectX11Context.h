#ifndef __BLADE_RHI_DIRECTX11_CONTEXT_H__
#define __BLADE_RHI_DIRECTX11_CONTEXT_H__

#include <d3d11.h>
#include <RHIContext.h>
#include <RHIDirectX11ShaderBase.h>
#include <RHIDirectX11TextureBase.h>

namespace BladeEngine
{
    namespace RHI
    {
        class DirectX11ContextBaseImpl : public IRHIContextBaseImpl
        {
        private:
            static const SIZE_T SHADER_RESOURCE_CACHE_NUM = 8;
            static const FLOAT BlendColor[4];
            static const UINT8 SamplerMask = 0;

        private:
            struct ContextStateCahce
            {
                ID3D11VertexShader* VS;
                ID3D11GeometryShader* GS;
                ID3D11HullShader* HS;
                ID3D11DomainShader* DS;
                ID3D11PixelShader* PS;

                ID3D11RasterizerState* RasterizerState;
                ID3D11BlendState* BlendState;
                ID3D11DepthStencilState* DepthStencilState;

                ID3D11DepthStencilView* DepthStencilView;
                ID3D11RenderTargetView* RenderTargetViews[8];

                ID3D11Buffer* UniformBuffers[SHADER_RESOURCE_CACHE_NUM];
                ID3D11ShaderResourceView* Textures[SHADER_RESOURCE_CACHE_NUM];
                ID3D11SamplerState* Samplers[SHADER_RESOURCE_CACHE_NUM];
            };

        protected:
            ID3D11DeviceContext* m_Context;
            ContextStateCahce m_StateCahce;

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

            virtual void SetTexture(RHITextureBase* inTexture, ESHADER_TYPE inType, SIZE_T inSlot)
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

            virtual void SetRenderTarget(RHITextureBase* inTexture)
            {
                ID3D11RenderTargetView* renderTargetViewView = ((IDirectX11TextureInterface*)inTexture)->GetRenderTargetView();
                m_Context->OMSetRenderTargets(1, &renderTargetViewView, m_StateCahce.DepthStencilView);
            }

            virtual void Clear(const BColor& inColor)
            {
                if (renderTargetViewView != NULL)
                {
                    m_Context->ClearRenderTargetView(inColor._data);
                }
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