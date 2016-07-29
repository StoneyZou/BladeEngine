#ifndef __BLADE_RHI_DIRECTX11_UNIFORM_BUFFER_H__
#define __BLADE_RHI_DIRECTX11_UNIFORM_BUFFER_H__

#include <DirectX11Device.h>
#include <DirectX11Context.h>
#include <RHIBufferBase.h>
#include <RHIDirectX11ShaderBase.h>

namespace BladeEngine
{
    namespace RHI
    {
        class DirectX11UniformBuffer : public RHIUniformBuffer
        {
        private:
            SIZE_T m_PackSize;
            ID3D11Buffer* m_Buffer;

        public:
            DirectX11UniformBuffer(DirectX11Device* inDevice, ID3D11Buffer* inBuffer, SIZE_T inPackSize, const RHIUniformCreateInfo& inCreateInfo)
                : RHIUniformBuffer(inDevice, inCreateInfo),
                m_Buffer(inBuffer),
                m_PackSize(inPackSize)
            {
                m_Buffer->AddRef();
            }

            virtual ~DirectX11UniformBuffer()
            {
                m_Buffer->Release();
            }

        public:
            SIZE_T GetPackSize() const { return m_PackSize; }

            ID3D11Buffer* GetBuffer() { return m_Buffer; }

        protected:
            void* Lock(RHIContextBase* inContext, ERES_LOCK_TYPE inType, const SIZE_T inIndex)
            {
                BladeAssert(inType == ERES_LOCK_WRITE_DISCARD);
                BladeAssert((GetAccessMode() & ECPU_WRITE) != 0);
                BladeAssert((GetAccessMode() & EGPU_READ) != 0);

                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());
                return contextImpl->LockGpuResource(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, true);
            }

            void Unlock(RHIContextBase* inContext, const SIZE_T inIndex)
            {
                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());
                contextImpl->UnlockGpuResource(m_Buffer, 0);
            }
        };

        class DirectX11InputLayout : public RHIInputLayout
        {
        private:
            ID3D11InputLayout* m_Layout;

        public:
            DirectX11InputLayout(DirectX11Device* inDevice, ID3D11InputLayout* inLayout, 
                const RHIInputLayoutCreateInfo& inCreateInfo)
                : RHIInputLayout(inDevice, inCreateInfo), m_Layout(inLayout)
            {
                m_Layout->AddRef();
            }

            virtual ~DirectX11InputLayout()
            {
                if (m_Layout != NULL)
                {
                    m_Layout->Release();
                }
            }

            ID3D11InputLayout* GetLayout() { return m_Layout; }
        };

        class DirectX11VertexBuffer : public RHIVertexBuffer
        {
        private:
            ID3D11Buffer* m_Buffer;

        public:
            DirectX11VertexBuffer(DirectX11Device* inDevice, ID3D11Buffer* inBuffer,
                const RHIVertexBufferCreateInfo& inCreateInfo)
                : RHIVertexBuffer(inDevice, inCreateInfo), m_Buffer(inBuffer)
            {}

            virtual ~DirectX11VertexBuffer()
            {
                if (m_Buffer != NULL)
                {
                    m_Buffer->Release();
                }
            }

            ID3D11Buffer* GetBuffer() { return m_Buffer; }
        };
    }
}

#endif // !__BLADE_RHI_DIRECTX11_UNIFORM_BUFFER_H__