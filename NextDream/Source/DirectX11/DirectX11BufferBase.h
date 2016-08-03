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

        class DirectX11VertexBuffer : public RHIVertexBuffer
        {
        private:
            ID3D11Buffer* m_Buffer;
            ID3D11Buffer* m_ShadowBuffer;
            ID3D11Buffer* m_LockingBuffer;

        public:
            DirectX11VertexBuffer(DirectX11Device* inDevice, ID3D11Buffer* inBuffer,
                const RHIVertexBufferCreateInfo& inInitInfo)
                : RHIVertexBuffer(inDevice, inInitInfo), 
                m_Buffer(inBuffer)
            {
                BladeAssert(m_Buffer != NULL);
                m_Buffer->AddRef();
            }

            virtual ~DirectX11VertexBuffer()
            {
                if (m_Buffer != NULL) { m_Buffer->Release(); }
            }

        public:
            virtual void* Lock(RHIContextBase* inContext, ERES_LOCK_TYPE inType, const SIZE_T inIndex)
            {
                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());

                bool cantLock = (CanCpuRead() && (inType & ERES_LOCK_READ) != 0) ||
                    (CanCpuWrite() && (inType & ERES_LOCK_WRITE) != 0);

                D3D11_MAP mapType = D3D11_MAP_READ_WRITE;
                mapType = (inType == ERES_LOCK_ONLY_READ ? D3D11_MAP_READ : mapType);
                mapType = (inType == ERES_LOCK_ONLY_WRITE ? D3D11_MAP_WRITE : mapType);
                mapType = (inType == ERES_LOCK_READ_WRITE ? D3D11_MAP_READ_WRITE : mapType);
                mapType = (inType == ERES_LOCK_WRITE_DISCARD ? D3D11_MAP_WRITE_DISCARD : mapType);

#if _DEBUG
                if (!CanGpuWrite() && inType == ERES_LOCK_ONLY_WRITE)
                {
                    //log can create texture use EGPU_READ_CPU_WRITE
                }
#endif

                if (mapType == D3D11_MAP_WRITE_DISCARD && !CanCpuWrite())
                {
                    //log
                    return NULL;
                }

                if (cantLock)
                {
                    m_LockingBuffer = m_Buffer;
                    return contextImpl->LockGpuResource(m_Buffer, inIndex, mapType, true);
                }

                if (m_ShadowBuffer != NULL)
                {
                    ID3D11Device* device = contextImpl->GetDevice();
                    if (device == NULL)
                    {
                        return NULL;
                    }

                    D3D11_BUFFER_DESC desc;
                    m_Buffer->GetDesc(&desc);
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
                    desc.Usage = D3D11_USAGE_STAGING;

                    HRESULT hr = device->CreateBuffer(&desc, NULL, &m_ShadowBuffer);
                    if (FAILED(hr))
                    {
                        //log
                    }
                }

                contextImpl->CopyGpuResource(m_Buffer, m_ShadowBuffer);

                m_LockingBuffer = m_ShadowBuffer;
                return contextImpl->LockGpuResource(m_ShadowBuffer, 0, mapType, true);
            }

            virtual void Unlock(RHIContextBase* inContext, const SIZE_T inIndex)
            {
                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());
                contextImpl->UnlockGpuResource(m_LockingBuffer, 0);
                m_LockingBuffer = NULL;
            }

        public:
            ID3D11Buffer* GetBuffer() { return m_Buffer; }
        };
    }
}

#endif // !__BLADE_RHI_DIRECTX11_UNIFORM_BUFFER_H__