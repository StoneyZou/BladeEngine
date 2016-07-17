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
                //
            }

        public:
        };
    }
}

#endif // !__BLADE_RHI_DIRECTX11_SHADER_BASE_H__