#ifndef __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
#define __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__

#include <d3d11.h>
#include <dxgi.h>
#include <RHIDevice.h>
#include <TMap.h>
#include <BMath.h>
#include <BModule.h>
#include <PlatformAPI.h>
#include <GlobalConfig.h>

namespace BladeEngine
{
    namespace RHI
    {
        class DirectX11UniformBuffer;

        struct IDirectX11TextureInterface
        {
            virtual ID3D11RenderTargetView* GetRenderTargetView() = 0;
            virtual ID3D11DepthStencilView* GetDepthStencilView() = 0;
            virtual ID3D11ShaderResourceView* GetShaderResourceView() = 0;
        };

        class DirectX11Device : public IRHIDevice
        {
        private:
            template<typename Type>
            struct MemCompareFunc
            {
            public:
                static int32 Compare(const Type& lh, const Type& rh)
                {
                    return MemUtil::Memcmp(&lh, &rh, sizeof(Type));
                }
            };

            template<typename Type>
            struct ArrayMemCompareFunc
            {
            public:
                static int32 Compare(const TArray<Type>& lh, const TArray<Type>& rh)
                {
                    if (lh.Size() < rh.Size())
                    {
                        return 1;
                    }
                    else if(lh.Size() > rh.Size())
                    {
                        return -1;
                    }
                    return  MemUtil::Memcmp(lh.VoidPtr(), rh.VoidPtr(), lh.Size() * sizeof(Type));
                }
            };

            typedef TMap<D3D11_RASTERIZER_DESC, ID3D11RasterizerState*, MemCompareFunc<D3D11_RASTERIZER_DESC>> RasterizerStateMap;
            typedef TMap<D3D11_BLEND_DESC, ID3D11BlendState*, MemCompareFunc<D3D11_BLEND_DESC>> BlendStateMap;
            typedef TMap<D3D11_DEPTH_STENCIL_DESC, ID3D11DepthStencilState*, MemCompareFunc<D3D11_DEPTH_STENCIL_DESC>> DepthStencilStateMap;
            typedef TMap<D3D11_SAMPLER_DESC, ID3D11SamplerState*, MemCompareFunc<D3D11_SAMPLER_DESC>> SamplerStateMap;
            typedef TArray<DirectX11UniformBuffer*> UniformBufferList;
            typedef TArray<D3D11_INPUT_ELEMENT_DESC> InputElementDescArray;
            typedef TMap<InputElementDescArray, ID3D11InputLayout*, ArrayMemCompareFunc<InputElementDescArray>> InputLayoutMap;

        private:
			IDXGIFactory* m_pDXGIFactory;
            ID3D11Device* m_pDevice;
            ID3D11DeviceContext* m_pImmediateContext;

            RasterizerStateMap m_RasterizerStateMap;
            BlendStateMap m_BlendStateMap;
            DepthStencilStateMap m_DepthStencilStateMap;
            SamplerStateMap m_SamplerStateMap;
            UniformBufferList m_UniformBufferList;

            TArray<D3D11_INPUT_ELEMENT_DESC> m_TempInputElementDescs;
            TArray<RHISwapChainRef> m_SwapChainList;

        public:
            DirectX11Device(IDXGIFactory* inFactory, ID3D11Device* inDevice, ID3D11DeviceContext* inContext) 
				: m_pDXGIFactory(inFactory), m_pDevice(inDevice), m_pImmediateContext(inContext)
            {
                if (m_pDXGIFactory != NULL)         { m_pDXGIFactory->AddRef(); }
                if (m_pDevice != NULL)              { m_pDevice->AddRef(); }
                if (m_pImmediateContext != NULL)    { m_pImmediateContext->AddRef(); }
            }

            virtual ~DirectX11Device()
            {
                if (m_pDXGIFactory != NULL)         { m_pDXGIFactory->Release(); }
                if (m_pDevice != NULL)              { m_pDevice->Release(); }
                if (m_pImmediateContext != NULL)    { m_pImmediateContext->Release(); }
            }

        public:
            virtual RHITexture2DRef CreateTexture2D(const RHITextureCreateInfo& inCreateInfo);

            virtual RHIVertexShaderRef CreateVextexShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIPixelShaderRef CreatePixelShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIHullShaderRef CreateHullShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIDomainShaderRef CreateDomainShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIGeometryShaderRef CreateGeometryShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIVertexBufferRef CreateVertexBuffer(const RHIVertexBufferCreateInfo& inCreateInfo);

            virtual RHIShaderStateRef CreateShaderState(const RHIShaderStateCreateInfo& inCreateInfo);

            virtual RHIUniformBufferRef CreateUniformBuffer(const RHIUniformCreateInfo& inCreateInfo);

            virtual RHISwapChainRef CreateSwapChain(const RHISwapChainCreateInfo& inCreateInfo);

            virtual RHISwapChainRef GetSwapChain(PlatformWindowRef inWindow);

        public:
            virtual RHIImmediateContextRef GetImmediateContext();

            virtual RHIDeferredContextRef CreateDeferredContext();
        };
    }

    class DirectX11RHIModule : public IRHIModule
    {
    private:
        PFN_D3D11_CREATE_DEVICE m_FuncD3D11CreateDevice;

        HModule m_Module;
		IDXGIFactory* m_DXGIFactory;
        TArray<IDXGIAdapter*> m_Adapters;

    public:
        DirectX11RHIModule() : m_DXGIFactory(NULL), IRHIModule(EMODULE_RHI_DIRECTX11)
        {
        }

    public:
        virtual bool Load(const BString& inFileName);
        
        virtual bool StartUp();

        virtual void ShutDown();

        virtual void Unload();

        virtual bool InitDevice(uint32 inAdapterIndex);

        //virtual void SwitchAdapterByIndex(uint32 inIndex) const = 0;
    };
}

#endif // !__BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
