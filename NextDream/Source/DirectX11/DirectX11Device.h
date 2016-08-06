#ifndef __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
#define __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__

#include <d3d11.h>
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
		typedef HRESULT (__stdcall *PFN_CREATE_DXGI_FACTORY1)(REFIID riid, void **ppFactory);

    private:
        PFN_D3D11_CREATE_DEVICE m_FuncD3D11CreateDevice;
		PFN_CREATE_DXGI_FACTORY1 m_FuncCreateDXGIFactory1;

        HModule m_Module;
		IDXGIFactory1* m_DXGIFactory1;
        TArray<BString> m_AdapterNames;
        TArray<IDXGIAdapter1*> m_Adapters;

    public:
        DirectX11RHIModule() : m_DXGIFactory1(NULL)
        {
        }

    public:
        virtual bool Load(const BString& inFileName )
        {
            m_Module = SystemAPI::LoadBaseModule(inFileName);
            if (!SystemAPI::CheckModuleHandleValid(m_Module))
            {
                //log
                return false;
            }

            m_FuncD3D11CreateDevice = (PFN_D3D11_CREATE_DEVICE)SystemAPI::GetProcAddress(module, "D3D11CreateDevice");
            if (m_FuncD3D11CreateDevice != NULL)
            {
                //log
                return false;
            }

            m_FuncCreateDXGIFactory1 = (PFN_CREATE_DXGI_FACTORY1)SystemAPI::GetProcAddress(module, "CreateDXGIFactory1"); 
            if (m_FuncCreateDXGIFactory1 != NULL)
            {
                //log
                return false;
            }
        }
        
        virtual bool StartUp()
        {
            HRESULT hr = m_FuncCreateDXGIFactory1(IID_IDXGIFactory1, (void**)&m_DXGIFactory1);
            ComPtrGuard(m_DXGIFactory1);

            if (FAILED(hr))
            {
                return false;
            }

            uint32 index = 0;
            IDXGIAdapter1* adapter1 = NULL;

            while (FAILED(m_DXGIFactory1->EnumAdapters1(index, &adapter1)))
            {
                ComPtrGuard(adapter1);
                if(adapter1 == NULL)
                {
                    //
                    return false;
                }

                DXGI_ADAPTER_DESC1 desc; 
                hr = adapter1->GetDesc1(&desc);
                if (FAILED(hr))
                {
                    continue;
                }

                m_AdapterNames.Add(StringUtil::WideCahrToTChar(desc.Description));
            }

            m_DXGIFactory1->AddRef();
            return true;
        }

        virtual void ShutDown()
        {
            if (m_DXGIFactory1 != NULL) { m_DXGIFactory1->Release(); }
            if (m_Device != NULL) { delete m_Device; }

            m_Adapters.Clear();
            m_AdapterNames.Clear();

            m_Device = NULL;
            m_DXGIFactory1 = NULL;
        }

        virtual void Unload()
        {
            SystemAPI::FreeBaseModule(m_Module);

            m_FuncD3D11CreateDevice = NULL;
            m_FuncCreateDXGIFactory1 = NULL;
            m_Module = 0;
        }

        virtual bool InitDevice()
        {
            const BString& defaultAdapterName = Framework::GlobalConfig::GetInstance().m_DefaultAdapterName;
            
            int32 selectAdapterIndex = -1;
            for(uint32 i = 0; i < m_AdapterNames.GetLength(); ++i)
            {
                if (m_AdapterNames[i] == defaultAdapterName)
                {
                    selectAdapterIndex = (int32)i;
                    break;
                }
            }

            if( selectAdapterIndex < 0 )
            {
                //
                return false;
            }

            IDXGIAdapter1* selectAdapter = NULL;
            HRESULT hr = m_DXGIFactory1->EnumAdapters1(selectAdapterIndex, &selectAdapter);
            ComPtrGuard(selectAdapter);

            if (FAILED(hr))
            {
                selectAdapterIndex = -1;
            }

            const D3D_FEATURE_LEVEL featureLevels[] = {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1,
            };

            D3D_FEATURE_LEVEL feature;
            ID3D11Device* device = NULL;
            ID3D11DeviceContext* context = NULL;

            hr = m_FuncD3D11CreateDevice(
                selectAdapterIndex < 0 ? NULL : m_Adapters[selectAdapterIndex],
                D3D_DRIVER_TYPE_HARDWARE,
                0,
                0,
                featureLevels,
                countof(featureLevels),
                D3D11_SDK_VERSION,
                &device,
                &feature,
                &context
            );
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
    };
}

#endif // !__BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
