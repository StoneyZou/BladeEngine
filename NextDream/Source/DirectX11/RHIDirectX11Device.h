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
                D3D11_TEXTURE2D_DESC tTextureDesc = { 0 };
                //tDesc.BindFlags
                tTextureDesc.Width = inCreateInfo.Width;
                tTextureDesc.Height = inCreateInfo.Height;
                tTextureDesc.Usage = inCreateInfo.Writable ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
                tTextureDesc.CPUAccessFlags = inCreateInfo.Writable ? D3D11_CPU_ACCESS_WRITE : 0;
                tTextureDesc.SampleDesc.Count = 1;
                tTextureDesc.SampleDesc.Quality = 0;
                tTextureDesc.ArraySize = 1;
                tTextureDesc.MipLevels = 0;
                tTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (inCreateInfo.AsRenderTarget ? D3D11_BIND_RENDER_TARGET : 0);
                tTextureDesc.MiscFlags = 0;
                tTextureDesc.Format = RHIDirectXEnumMapping::Get(inCreateInfo.Format);
                
                D3D11_SUBRESOURCE_DATA tTextureData = { 0 };
                tTextureData.pSysMem = inCreateInfo.Data;
                tTextureData.SysMemPitch = 0;
                tTextureData.SysMemSlicePitch = 0;
                
                ID3D11Texture2D* pD3D11Texture2D = NULL;
                HRESULT hr = m_pDevice->CreateTexture2D(&tTextureDesc, &tTextureData, &pD3D11Texture2D);

                if (FAILED(hr))
                {
                    //Logger::Log()
                    return NULL;
                }
                
                D3D11_SHADER_RESOURCE_VIEW_DESC tShaderResViewDesc;
                tShaderResViewDesc.Format = tTextureDesc.Format;
                tShaderResViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
                tShaderResViewDesc.Texture2D.MipLevels = 0;
                tShaderResViewDesc.Texture2D.MostDetailedMip = 0;
                
                ID3D11ShaderResourceView* pD3D11ShaderResourceView = NULL;
                hr = m_pDevice->CreateShaderResourceView(pD3D11Texture2D, &tShaderResViewDesc, &pD3D11ShaderResourceView);

                if (FAILED(hr))
                {
                    //Logger::Log()
                    return NULL;
                }
            }

            virtual RHIVertexShaderRef CreateVextexShader(const RHIShaderCreateInfo& inCreateInfo)
            {
                BladeAssert(inCreateInfo.Defines.Length() <= MAX_SHADER_DEFINE_NUM);
                BladeAssert(inCreateInfo.CodeOffsetWithDefines.Length() == inCreateInfo.CodeSizeWithDefines.Length());

                ID3D11VertexShader* pD3D11VertexShader = NULL;
                HRESULT hr = S_OK;

                for (int i = 0; i < inCreateInfo.CodeOffsetWithDefines.Length(); ++i)
                {
                    inCreateInfo.Data;
                    SIZE_T codeOffest = inCreateInfo.CodeOffsetWithDefines[i];
                    SIZE_T codeSize = inCreateInfo.CodeSizeWithDefines[i];

                    hr = m_pDevice->CreateVertexShader(((const char*)inCreateInfo.Data + codeOffest) , codeSize, NULL, &pD3D11VertexShader);
                    if (FAILED(hr))
                    {
                        //Logger::Log()
                        return NULL;
                    }
                }
            }

            virtual RHIPixelShaderRef CreatePixelShader(const RHIShaderCreateInfo& inCreateInfo))
            {
                BladeAssert(inCreateInfo.Defines.Length() <= MAX_SHADER_DEFINE_NUM);
                BladeAssert(inCreateInfo.CodeOffsetWithDefines.Length() == inCreateInfo.CodeSizeWithDefines.Length());

                ID3D11PixelShader* pD3D11PixelShader = NULL;
                HRESULT hr = S_OK;

                for (int i = 0; i < inCreateInfo.CodeOffsetWithDefines.Length(); ++i)
                {
                    inCreateInfo.Data;
                    SIZE_T codeOffest = inCreateInfo.CodeOffsetWithDefines[i];
                    SIZE_T codeSize = inCreateInfo.CodeSizeWithDefines[i];

                    hr = m_pDevice->CreatePixelShader(((const char*)inCreateInfo.Data + codeOffest), codeSize, NULL, &pD3D11PixelShader);
                    if (FAILED(hr))
                    {
                        //Logger::Log()
                        return NULL;
                    }
                }
            }


            virtual RHIHullShaderRef CreateHullShader(const RHIShaderCreateInfo) = 0;

            virtual RHIDomainShaderRef CreateDomainShader(const RHIShaderCreateInfo) = 0;

            virtual RHIGeometryShaderRef CreateGeometryShader(const RHIShaderCreateInfo) = 0;

            virtual RHIVertexBufferRef CreateVertexBuffer(const RHIVertexBufferCreateInfo) = 0;

            virtual RHIIndexBufferRef CreateIndexBuffer(const RHIIndexBufferCreateInfo) = 0;
        };
    }
}

#endif // !__BLADE_RHI_RHI_DIRECTX_11_DEVICE_H__
