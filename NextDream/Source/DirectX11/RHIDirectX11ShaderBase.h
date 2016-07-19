#ifndef __BLADE_RHI_DIRECTX11_SHADER_BASE_H__
#define __BLADE_RHI_DIRECTX11_SHADER_BASE_H__

#include <d3d11.h>
#include <MemUtil.h>
#include <RHIShaderBase.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHIDirectX11ShaderState : public RHIShaderState
        {
        private:
            ID3D11RasterizerState*      m_RasterizerState;
            ID3D11BlendState*           m_BlendState;
            ID3D11DepthStencilState*    m_DepthStencilState;

        public:
            RHIDirectX11ShaderState(ID3D11RasterizerState* inRasterizerState, ID3D11BlendState* inBlendState,
                ID3D11DepthStencilState* inDepthStencilState, const RHIShaderStateCreateInfo& inCreateInfo) 
                : RHIShaderState(inCreateInfo),
                m_RasterizerState(inRasterizerState),
                m_BlendState(inBlendState),
                m_DepthStencilState(inDepthStencilState)
            {
                m_RasterizerState->AddRef();
                m_BlendState->AddRef();
                m_DepthStencilState->AddRef();
            }

            ~RHIDirectX11ShaderState()
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

        class RHIDirectX11VertexShader : public RHIVertexShader
        {
        private:
            ID3D11VertexShader* m_VertexShader;

        public:
            RHIDirectX11VertexShader(ID3D11VertexShader* inShader, const RHIVertexShaderCreateInfo& inCreateInfo) : 
                RHIVertexShader(inCreateInfo),
                m_VertexShader(inShader)
            {
                m_VertexShader->AddRef();
            }

            ID3D11VertexShader* GetShader()
            {
                return m_VertexShader;
            }
        };

        class RHIDirectX11HullShader : public RHIHullShader 
        {
        private:
            ID3D11HullShader* m_HullShader;

        public:
            RHIDirectX11HullShader(ID3D11HullShader* inShader, const RHIShaderCreateInfo& inCreateInfo) :
                RHIHullShader(inCreateInfo),
                m_HullShader(inShader)
            {
                m_HullShader->AddRef();
            }

            ~RHIDirectX11HullShader()
            {
                uint64 refCount = m_HullShader->Release();
                BladeAssert(refCount == 0);
            }

            ID3D11HullShader* GetShader()
            {
                return m_HullShader;
            }
        };

        class RHIDirectX11DomainShader : public RHIDomainShader
        {
        private:
            ID3D11DomainShader* m_DomainShader;

        public:
            RHIDirectX11DomainShader(ID3D11DomainShader* inShader, const RHIShaderCreateInfo& inCreateInfo) :
                RHIDomainShader(inCreateInfo),
                m_DomainShader(inShader)
            {
                m_DomainShader->AddRef();
            }

            ~RHIDirectX11DomainShader()
            {
                uint64 refCount = m_DomainShader->Release();
                BladeAssert(refCount == 0);
            }

            ID3D11DomainShader* GetShader()
            {
                return m_DomainShader;
            }
        };

        class RHIDirectX11GeometryShader : public RHIGeometryShader
        {
        private:
            ID3D11GeometryShader* m_GeometryShader;

        public:
            RHIDirectX11GeometryShader(ID3D11GeometryShader* inShader, const RHIShaderCreateInfo& inCreateInfo) :
                RHIGeometryShader(inCreateInfo),
                m_GeometryShader(inShader)
            {
                m_GeometryShader->AddRef();
            }

            ~RHIDirectX11GeometryShader()
            {
                uint64 refCount = m_GeometryShader->Release();
                BladeAssert(refCount == 0);
            }

            ID3D11GeometryShader* GetShader()
            {
                return m_GeometryShader;
            }
        };

        class RHIDirectX11PixelShader : public RHIPixelShader
        {
        private:
            ID3D11PixelShader* m_PixelShader;

        public:
            RHIDirectX11PixelShader(ID3D11PixelShader* inShader, const RHIShaderCreateInfo& inCreateInfo) :
                RHIPixelShader(inCreateInfo),
                m_PixelShader(inShader)
            {
                m_PixelShader->AddRef();
            }

            ~RHIDirectX11PixelShader()
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