#ifndef __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
#define __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__

#include <d3d11.h>
#include <RHIDevice.h>
#include <TMap.h>
#include <BMath.h>
#include <BModule.h>
#include <GeneralSystemAPI.h>
#include <shared_ptr>

namespace BladeEngine
{
    namespace RHI
    {
        #define MAX_NUM_D3D11_RENDER_TARGET 8

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
            virtual RHITextureBaseRef CreateTexture2D(const RHITextureCreateInfo& inCreateInfo);

            virtual RHITextureBaseRef CreateTexture2DAsDepthStencil(const RHITextureCreateInfo& inCreateInfo);

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

    class DirectX11Module : public FrameWork::IModule
    {
	private:
		typedef HRESULT (__stdcall *PFN_CREATE_DXGI_FACTORY1)(REFIID riid, void **ppFactory);

    private:
        PFN_D3D11_CREATE_DEVICE m_FuncD3D11CreateDevice;
		PFN_CREATE_DXGI_FACTORY1 m_FuncCreateDXGIFactory1;

		ID3D11

    public:
        virtual bool Load(const BString& inFileName )
        {
            HModule module = SystemAPI::LoadBaseModule(inFileName);
            if (!SystemAPI::CheckModuleHandleValid(module))
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
			m_FuncCreateDXGIFactory1
        }
    };
}

#endif // !__BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
