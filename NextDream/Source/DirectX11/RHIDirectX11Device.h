#ifndef __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
#define __BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__

#include "RHIDevice.h"
#include "RHIDirectXEnumMapping.h"

namespace BladeEngine
{
    namespace RHI
    {
        #include <d3d11.h>

        typedef RefCountObject<ID3D11Device> ID3D11DeviceRef;

        class RHIDirectX11Device : public IRHIDevice
        {
        private:
            ID3D11DeviceRef m_pDevice;

        public:
            virtual RHITextureBaseRef CreateTexture2D(const RHITexture2DCreateInfo& inCreateInfo)
            {
                D3D11_TEXTURE2D_DESC tDesc = { 0 };
                //tDesc.BindFlags
                tDesc.Width = inCreateInfo.Width;
                tDesc.Height = inCreateInfo.Height;
                tDesc.Usage = inCreateInfo.Writable ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
                tDesc.CPUAccessFlags = inCreateInfo.Writable ? D3D11_CPU_ACCESS_WRITE : 0;
                tDesc.SampleDesc.Count = 1;
                tDesc.SampleDesc.Quality = 0;
                tDesc.ArraySize = 1;
                tDesc.MipLevels = 0;
                tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (inCreateInfo.AsRenderTarget ? D3D11_BIND_RENDER_TARGET : 0);
                tDesc.MiscFlags = 0;
                tDesc.Format = RHIDirectXEnumMapping::Get(inCreateInfo.Format);
                


                m_pDevice->CreateTexture2D(&tDesc, inCreateInfo.Data, )
            }

            virtual RHIVertexShaderRef CreateVextexShader(const RHIShaderCreateInfo) = 0;

            virtual RHIPixelShaderRef CreatePixelShader(const RHIShaderCreateInfo) = 0;

            virtual RHIHullShaderRef CreateHullShader(const RHIShaderCreateInfo) = 0;

            virtual RHIDomainShaderRef CreateDomainShader(const RHIShaderCreateInfo) = 0;

            virtual RHIGeometryShaderRef CreateGeometryShader(const RHIShaderCreateInfo) = 0;

            virtual RHIVertexBufferRef CreateVertexBuffer(const RHIVertexBufferCreateInfo) = 0;

            virtual RHIIndexBufferRef CreateIndexBuffer(const RHIIndexBufferCreateInfo) = 0;
        };
    }
}

#endif // !__BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
