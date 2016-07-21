#ifndef __BLADE_RHI_DIRECTX11_UNIFORM_BUFFER_H__
#define __BLADE_RHI_DIRECTX11_UNIFORM_BUFFER_H__

#include <DirectX11Device.h>
#include <DirectX11Context.h>
#include <RHIUniformBuffer.h>

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
                BladeAssert(GetAccessMode() & ECPU_WRITE != 0);
                BladeAssert(GetAccessMode() & EGPU_READ != 0);

                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());
                return contextImpl->LockGpuResource(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, true);
            }

            void Unlock(RHIContextBase* inContext, const SIZE_T inIndex)
            {
                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());
                contextImpl->UnlockGpuResource(m_Buffer, 0);
            }
        };
    }
}

#endif // !__BLADE_RHI_DIRECTX11_UNIFORM_BUFFER_H__