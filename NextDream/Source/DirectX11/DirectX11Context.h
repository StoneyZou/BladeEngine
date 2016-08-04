#ifndef __BLADE_RHI_DIRECTX11_CONTEXT_H__
#define __BLADE_RHI_DIRECTX11_CONTEXT_H__

#include <d3d11.h>
#include <RHIContext.h>
#include <DirectX11Device.h>

namespace BladeEngine
{
    namespace RHI
    {
        class DirectX11ContextBaseImpl : public IRHIContextBaseImpl
        {
        private:
            static const SIZE_T MAX_SHADER_RESOURCE_CACHE_NUM = 8;
            static const SIZE_T MAX_RENDER_TARGET_NUM = 8;
            static const FLOAT BlendColor[4];
            static const UINT8 SamplerMask = 0;

        private:
            const TCHAR* ShaderSemanticNames[6] =
            {
                TEXT("Unknown"),
                TEXT("Position"),
                TEXT("Normal"),
                TEXT("Color"),
                TEXT("Tangent"),
                TEXT("Texcoord"),
            };

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
            DirectX11ContextBaseImpl(ID3D11DeviceContext* inContext) : m_Context(inContext)
            {
                m_Context->AddRef();
            }

            ~DirectX11ContextBaseImpl()
            {
                m_Context->Release();
            }

        public:
            virtual void SetVertexShader(RHIVertexShader* inRHIShader);

            virtual void SetGeometryShader(RHIGeometryShader* inRHIShader);

            virtual void SetHullShader(RHIHullShader* inRHIShader);

            virtual void SetDomainShader(RHIDomainShader* inRHIShader);

            virtual void SetPixelShader(RHIPixelShader* inRHIShader);

            virtual void SetShaderState(RHIShaderState* inRHIShaderState);

            virtual void SetShaderResource(RHITextureBase* inTexture, ESHADER_TYPE inType, SIZE_T inSlot);

            virtual void SetRenderTarget(RHITextureBase* inTex, SIZE_T inIndex = 0);

            virtual void SetDepthStencil(RHITextureBase* inTex);

            virtual void ClearRenderTarget(const BColor& inColor);

            virtual void ClearDepthStencil(float inDepthValue = 0.0f, uint32 inStencilValue = 0);

            virtual void SetVertexBuufer(RHIVertexBuffer* inVertexBuffer, uint32 inStride, uint32 inOffset, uint32 inSlot = 0);

            virtual void DrawAuto();

            virtual void Draw(uint32 inVertexNum, uint32 inVertexOffset);

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

        class DirectX11ImmediateContextImpl : public DirectX11ContextBaseImpl
        {
        public:
            DirectX11ImmediateContextImpl(ID3D11DeviceContext* inContext) : DirectX11ContextBaseImpl(inContext)
            {
            }

        public:
            virtual void Flush()
            {
                // no thing, becase command is finish Immediatly;
            }
        };

        class DirectX11DeferredContextImpl : public DirectX11ContextBaseImpl
        {
        public:
            DirectX11DeferredContextImpl(ID3D11DeviceContext* inContext) : DirectX11ContextBaseImpl(inContext)
            {
            }

        public:
            virtual void Flush()
            {
                m_Context->Flush();
            }
        };
    }
}

#endif // !__BLADE_RHI_DIRECTX11_CONTEXT_H__