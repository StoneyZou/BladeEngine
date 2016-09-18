#ifndef __BLADE_RHI_DIRECTX11_SHADER_BASE_H__
#define __BLADE_RHI_DIRECTX11_SHADER_BASE_H__

#include <DirectX11Device.h>
#include <MemUtil.h>
#include <RHIShaderBase.h>

namespace BladeEngine
{
    namespace RHI
    {
        class DirectX11ShaderState : public RHIShaderState
        {
        private:
            ID3D11RasterizerState*      m_RasterizerState;
            ID3D11BlendState*           m_BlendState;
            ID3D11DepthStencilState*    m_DepthStencilState;

        public:
            DirectX11ShaderState(DirectX11Device* inDevice, ID3D11RasterizerState* inRasterizerState, ID3D11BlendState* inBlendState,
                ID3D11DepthStencilState* inDepthStencilState, const RHIShaderStateCreateInfo& inCreateInfo) 
                : RHIShaderState(inDevice, inCreateInfo),
                m_RasterizerState(inRasterizerState),
                m_BlendState(inBlendState),
                m_DepthStencilState(inDepthStencilState)
            {
                m_RasterizerState->AddRef();
                m_BlendState->AddRef();
                m_DepthStencilState->AddRef();
            }

            ~DirectX11ShaderState()
            {
                uint64 rasterizerStateRefCount = m_RasterizerState->Release();
                BladeAssert(rasterizerStateRefCount == 0);

                uint64 blendStateRefCount = m_BlendState->Release();
                BladeAssert(blendStateRefCount == 0);

                uint64 depthStencilStateRefCount = m_DepthStencilState->Release();
                BladeAssert(depthStencilStateRefCount == 0);
            }

            ID3D11RasterizerState* GetRasterizerState()
            {
                return m_RasterizerState;
            }

            ID3D11BlendState* GetBlendState()
            {
                return m_BlendState;
            }

            ID3D11DepthStencilState* GetDepthStencilState()
            {
                return m_DepthStencilState;
            }
        };

        class DirectX11VertexShader : public RHIVertexShader
        {
        private:/*
            byte* m_Data;
            SIZE_T m_DataSize;*/

            ID3D11VertexShader* m_VertexShader;

        public:
            DirectX11VertexShader(DirectX11Device* inDevice, ID3D11VertexShader* inShader) :
                RHIVertexShader(inDevice),
                m_VertexShader(inShader)
            {
               /* m_DataSize = inCreateInfo.DataSize;
                m_Data = (byte*)Malloc::Alloc(m_DataSize);
                MemUtil::Memcopy(m_Data, m_DataSize, inCreateInfo.Data, inCreateInfo.DataSize);*/

                m_VertexShader->AddRef();
            }

            ID3D11VertexShader* GetShader() { return m_VertexShader; }

            /*const byte* GetData() const { return m_Data; }
            SIZE_T GetDataSize() const { return m_DataSize; }*/
        };

        class DirectX11HullShader : public RHIHullShader 
        {
        private:
            ID3D11HullShader* m_HullShader;

        public:
            DirectX11HullShader(DirectX11Device* inDevice, ID3D11HullShader* inShader, const RHIShaderCreateInfo& inCreateInfo) :
                RHIHullShader(inDevice, inCreateInfo),
                m_HullShader(inShader)
            {
                m_HullShader->AddRef();
            }

            ~DirectX11HullShader()
            {
                uint64 refCount = m_HullShader->Release();
                BladeAssert(refCount == 0);
            }

            ID3D11HullShader* GetShader()
            {
                return m_HullShader;
            }
        };

        class DirectX11DomainShader : public RHIDomainShader
        {
        private:
            ID3D11DomainShader* m_DomainShader;

        public:
            DirectX11DomainShader(DirectX11Device* inDevice, ID3D11DomainShader* inShader, const RHIShaderCreateInfo& inCreateInfo) :
                RHIDomainShader(inDevice, inCreateInfo),
                m_DomainShader(inShader)
            {
                m_DomainShader->AddRef();
            }

            ~DirectX11DomainShader()
            {
                uint64 refCount = m_DomainShader->Release();
                BladeAssert(refCount == 0);
            }

            ID3D11DomainShader* GetShader()
            {
                return m_DomainShader;
            }
        };

        class DirectX11GeometryShader : public RHIGeometryShader
        {
        private:
            ID3D11GeometryShader* m_GeometryShader;

        public:
            DirectX11GeometryShader(DirectX11Device* inDevice, ID3D11GeometryShader* inShader) :
                RHIGeometryShader(inDevice),
                m_GeometryShader(inShader)
            {
                m_GeometryShader->AddRef();
            }

            ~DirectX11GeometryShader()
            {
                uint64 refCount = m_GeometryShader->Release();
                BladeAssert(refCount == 0);
            }

            ID3D11GeometryShader* GetShader()
            {
                return m_GeometryShader;
            }
        };

        class DirectX11PixelShader : public RHIPixelShader
        {
        private:
            ID3D11PixelShader* m_PixelShader;

        public:
            DirectX11PixelShader(DirectX11Device* inDevice, ID3D11PixelShader* inShader) :
                RHIPixelShader(inDevice),
                m_PixelShader(inShader)
            {
                m_PixelShader->AddRef();
            }

            ~DirectX11PixelShader()
            {
                uint64 refCount = m_PixelShader->Release();
                BladeAssert(refCount == 0);
            }

            ID3D11PixelShader* GetShader()
            {
                return m_PixelShader;
            }
        };
    }
}

#endif // !__BLADE_RHI_DIRECTX11_SHADER_BASE_H__