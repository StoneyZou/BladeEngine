#ifndef __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
#define __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__

#include <d3d11.h>
#include <dxgi.h>
#include <RHIDevice.h>
#include <TMap.h>
#include <BMath.h>
#include <BModule.h>
#include <GeneralSystemAPI.h>
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
                    if (lh.GetLength() < rh.GetLength())
                    {
                        return 1;
                    }
                    else if(lh.GetLength() > rh.GetLength())
                    {
                        return -1;
                    }
                    return  MemUtil::Memcmp(lh.VoidPtr(), rh.VoidPtr(), lh.GetLength() * sizeof(Type));
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
            ID3D11Device* m_pDevice;
            ID3D11DeviceContext* m_pImmediateContext;

            RasterizerStateMap m_RasterizerStateMap;
            BlendStateMap m_BlendStateMap;
            DepthStencilStateMap m_DepthStencilStateMap;
            SamplerStateMap m_SamplerStateMap;
            UniformBufferList m_UniformBufferList;

            TArray<D3D11_INPUT_ELEMENT_DESC> m_TempInputElementDescs;

        public:
            DirectX11Device(ID3D11Device* inDevice, ID3D11DeviceContext* inContext) : m_pDevice(inDevice), m_pImmediateContext(inContext)
            {
                m_pDevice->AddRef();
                m_pImmediateContext->AddRef();
            }

            virtual ~DirectX11Device()
            {
                m_pDevice->Release();
                m_pImmediateContext->Release();
            }

        public:
            virtual RHITextureBaseRef CreateTexture2D(const RHITextureCreateInfo& inCreateInfo);

            virtual RHIVertexShaderRef CreateVextexShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIPixelShaderRef CreatePixelShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIHullShaderRef CreateHullShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIDomainShaderRef CreateDomainShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIGeometryShaderRef CreateGeometryShader(const RHIShaderCreateInfo& inCreateInfo);

            virtual RHIVertexBufferRef CreateVertexBuffer(const RHIVertexBufferCreateInfo& inCreateInfo);

            virtual RHIShaderStateRef CreateShaderState(const RHIShaderStateCreateInfo& inCreateInfo);

            virtual RHIUniformBufferRef CreateUniformBuffer(const RHIUniformCreateInfo& inCreateInfo);

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
        virtual bool Load(const BString& inFileName)
        {
            m_Module = SystemAPI::LoadBaseModule(inFileName);
            if (!SystemAPI::CheckModuleHandleValid(m_Module))
            {
                //log
                return false;
            }

            m_FuncD3D11CreateDevice = (PFN_D3D11_CREATE_DEVICE)SystemAPI::GetProcAddress(m_Module, "D3D11CreateDevice");
            if (m_FuncD3D11CreateDevice == NULL)
            {
                //log
                return false;
            }

            return true;
        }
        
        virtual bool StartUp()
        {
            HRESULT hr = CreateDXGIFactory(IID_IDXGIFactory, (void**)&m_DXGIFactory);
            ComPtrGuard(m_DXGIFactory);

            if (FAILED(hr))
            {
                return false;
            }

            uint32 index = 0;
            IDXGIAdapter* adapter = NULL;

            uint64 maxDedicatedVideoMemory = 0;
            uint64 maxDedicatedSystemMemory = 0;
            uint64 maxSharedSystemMemory = 0;
            
            m_bestAdapterIndex = 0;

            while (!FAILED(m_DXGIFactory->EnumAdapters(index, &adapter)))
            {
                ComPtrGuard(adapter);
                if(adapter == NULL)
                {
                    //
                    return false;
                }

                DXGI_ADAPTER_DESC desc; 
                hr = adapter->GetDesc(&desc);
                if (FAILED(hr))
                {
                    continue;
                }

                if (maxDedicatedVideoMemory < desc.DedicatedVideoMemory)
                {
                    m_bestAdapterIndex = index;
                }

                if (maxDedicatedVideoMemory == 0 && desc.DedicatedSystemMemory)
                {
                    m_bestAdapterIndex = index;
                }

                if (maxDedicatedVideoMemory == 0 && maxSharedSystemMemory == 0 && desc.SharedSystemMemory)
                {
                    m_bestAdapterIndex = index;
                }

                ++index;
                adapter->AddRef();
                m_Adapters.Add(adapter);
                m_AdapterNames.Add(StringUtil::WideCahrToTChar(desc.Description));
            }

            m_DXGIFactory->AddRef();
            return true;
        }

        virtual void ShutDown()
        {
            if (m_DXGIFactory != NULL) { m_DXGIFactory->Release(); }
            if (m_Device != NULL) { delete m_Device; }

            for(uint32 i = 0; i < m_Adapters.GetLength(); ++i)
            {
                if(m_Adapters[i] != NULL)
                {
                    m_Adapters[i]->Release();
                }
            }
            m_AdapterNames.Clear();

            m_Device = NULL;
            m_DXGIFactory = NULL;
        }

        virtual void Unload()
        {
            SystemAPI::FreeBaseModule(m_Module);

            m_FuncD3D11CreateDevice = NULL;
            m_Module = 0;
        }

        virtual bool InitDevice(uint32 inAdapterIndex)
        {
            if(inAdapterIndex < 0 || inAdapterIndex >= m_Adapters.GetLength())
            {
                //
                return false;
            }

            IDXGIAdapter* selectAdapter = m_Adapters[inAdapterIndex];

            D3D_FEATURE_LEVEL feature;
            ID3D11Device* device = NULL;
            ID3D11DeviceContext* context = NULL;

            const D3D_FEATURE_LEVEL featureLevel_11_1 = D3D_FEATURE_LEVEL_11_1;

            // test can create device with ferture 11_1
            HRESULT hr = m_FuncD3D11CreateDevice(
                selectAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, 0,
                &featureLevel_11_1, 1,
                D3D11_SDK_VERSION,
                &device, &feature, &context
            );

            if (hr == E_INVALIDARG)
            {
                const D3D_FEATURE_LEVEL featureLevels[] = {
                    D3D_FEATURE_LEVEL_11_0,
                    D3D_FEATURE_LEVEL_10_1,
                    D3D_FEATURE_LEVEL_10_0,
                    D3D_FEATURE_LEVEL_9_3,
                    D3D_FEATURE_LEVEL_9_2,
                    D3D_FEATURE_LEVEL_9_1,
                };
                // create device with lower ferture
                hr = m_FuncD3D11CreateDevice(
                    selectAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, 0,
                    featureLevels, countof(featureLevels),
                    D3D11_SDK_VERSION,
                    &device, &feature, &context
                );
            }

            ComPtrGuard(device);
            ComPtrGuard(context);

            if (FAILED(hr))
            {
                //
                return false;
            }

            m_Device = new RHI::DirectX11Device(device, context);
            return true;
        }

        //virtual void SwitchAdapterByIndex(uint32 inIndex) const = 0;
    };
}

#endif // !__BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
