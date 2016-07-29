#ifndef __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
#define __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__

#include <d3d11.h>
#include "RHIDevice.h"
#include <TMap.h>
#include <BMath.h>

namespace BladeEngine
{
    namespace RHI
    {
        #define MAX_NUM_D3D11_RENDER_TARGET 8

        typedef RefCountObject<ID3D11Device> ID3D11DeviceRef;
        typedef RefCountObject<ID3D11DeviceContext> ID3D11ContextRef;

        class ComPtrGuard
        {
        private:
            IUnknown* m_Ptr;

        public:
            ComPtrGuard(IUnknown* inPtr) : m_Ptr(m_Ptr)
            {
            }

            ~ComPtrGuard()
            {
                if (m_Ptr != NULL)
                {
                    m_Ptr->Release();
                }
            }
        };

        #define D3D11PtrGuard(ptr) ComPtrGuard ptr##Guard(ptr)

        class DirectX11UniformBuffer;

        class DirectX11Device : public IRHIDevice
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

            typedef TMap<D3D11_RASTERIZER_DESC, ID3D11RasterizerState*, MemCompareFunc<RHIShaderRasterizerDesc>> RasterizerStateMap;
            typedef TMap<D3D11_BLEND_DESC, ID3D11BlendState*, MemCompareFunc<RHIShaderBlendDesc>> BlendStateMap;
            typedef TMap<D3D11_DEPTH_STENCIL_DESC, ID3D11DepthStencilState*, MemCompareFunc<RHIShaderDepthStencilDesc>> DepthStencilStateMap;
            typedef TMap<D3D11_SAMPLER_DESC, ID3D11SamplerState*, MemCompareFunc<RHITexuteSamplerInfo>> SamplerStateMap;
            typedef TArray<DirectX11UniformBuffer*> UniformBufferList;

        private:
            ID3D11DeviceRef m_pDevice;
            ID3D11ContextRef m_pContext;

            RasterizerStateMap m_RasterizerStateMap;
            BlendStateMap m_BlendStateMap;
            DepthStencilStateMap m_DepthStencilStateMap;
            SamplerStateMap m_SamplerStateMap;
            UniformBufferList m_UniformBufferList;

            TArray<D3D11_INPUT_ELEMENT_DESC> m_TempInputElementDescs;

        public:
            virtual RHITextureBaseRef CreateTexture2D(const RHITextureCreateInfo& inCreateInfo) = 0;

            virtual RHIVertexShaderRef CreateVextexShader(const RHIShaderCreateInfo& inCreateInfo) = 0;

            virtual RHIPixelShaderRef CreatePixelShader(const RHIShaderCreateInfo& inCreateInfo) = 0;

            virtual RHIHullShaderRef CreateHullShader(const RHIShaderCreateInfo) = 0;

            virtual RHIDomainShaderRef CreateDomainShader(const RHIShaderCreateInfo) = 0;

            virtual RHIGeometryShaderRef CreateGeometryShader(const RHIShaderCreateInfo) = 0;

            virtual RHIVertexBufferRef CreateVertexBuffer(const RHIVertexBufferCreateInfo) = 0;

            virtual RHIIndexBufferRef CreateIndexBuffer(const RHIIndexBufferCreateInfo) = 0;

            virtual RHIShaderStateRef CreateShaderState(const RHIShaderStateCreateInfo& inCreateInfo);

            virtual RHIUniformBufferRef CreateUniformBuffer(const RHIUniformCreateInfo& inCreateInfo);

            virtual RHIInputLayoutRef CreateInputLayout(const RHIInputLayoutCreateInfo& inCreateInfo);
        };
    }
}

#endif // !__BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
