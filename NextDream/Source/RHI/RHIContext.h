#ifndef __BLADE_RHI_RHI_CONTEXT_H__
#define __BLADE_RHI_RHI_CONTEXT_H__

#include <RHIDevice.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHIContextBase;

        class IResourceLockable
        {
            friend RHIContextBase;
        protected:
            virtual void* Lock(RHIContextBase* inParam, ERES_LOCK_TYPE inType, const SIZE_T inIndex) = 0;
            virtual void Unlock(RHIContextBase* inParam, const SIZE_T inIndex) = 0;
        };

        class IResourceCopyable
        {
            friend RHIContextBase;
        protected:
            virtual RHIResource* Copy(void* inParam, ECPU_GPU_ACCESS_MODE inMode) = 0;
        };

        class IRHIContextBaseImpl
        {
        public:
            ~IRHIContextBaseImpl() {}

        public:
            virtual void SetVertexShader(RHIVertexShader* inRHIShader) = 0;

            virtual void SetPixelShader(RHIPixelShader* inRHIShader) = 0;

            virtual void SetGeometryShader(RHIGeometryShader* inRHIShader) = 0;

            virtual void SetHullShader(RHIHullShader* inRHIShader) = 0;

            virtual void SetDomainShader(RHIDomainShader* inRHIShader) = 0;

            virtual void SetShaderState(RHIShaderState* inRHIShaderState) = 0;

            virtual void SetTexture(RHITextureBase* inTex) = 0;

            //virtual bool SetUniformBuffer(RHIShaderState* inRHIShader) = 0;
        };

        class RHIContextBase
        {
        private:
            bool m_IsDeferred;
            IRHIContextBaseImpl* m_Impl;

        protected:
            RHIContextBase(IRHIContextBaseImpl* inImpl, bool inIsDeffered) : m_Impl(inImpl), m_IsDeferred(inIsDeffered)
            {}

        public:
            IRHIContextBaseImpl* GetImpl() { return m_Impl; }
            const IRHIContextBaseImpl* GetImpl() const { return m_Impl; }

        public:
            bool IsDeferred() const { return m_IsDeferred; }

        public:
            void SetVertexShader(RHIVertexShader* inRHIShader) { m_Impl->SetVertexShader(inRHIShader); }

            void SetPixelShader(RHIPixelShader* inRHIShader) { m_Impl->SetPixelShader(inRHIShader); }

            void SetGeometryShader(RHIGeometryShader* inRHIShader) { m_Impl->SetGeometryShader(inRHIShader); }

            void SetHullShader(RHIHullShader* inRHIShader) { m_Impl->SetHullShader(inRHIShader); }

            void SetDomainShader(RHIDomainShader* inRHIShader) { m_Impl->SetDomainShader(inRHIShader); }

            void SetShaderState(RHIShaderState* inRHIShaderState) { m_Impl->SetShaderState(inRHIShaderState); }

            void SetTexture(RHITextureBase* inTex) { m_Impl->SetTexture(inTex); }

            void* Lock(IResourceLockableRef& inResource, ERES_LOCK_TYPE inType, const SIZE_T inIndex = 0)
            {
                if (((RHIResourceRef)inResource)->GetAccessMode() & ECPU_READ == 0)
                {
                    //log
                    return NULL;
                }

                return inResource->Lock(this, inType, inIndex);
            }

            void Unlock(IResourceLockableRef& inResource, const SIZE_T inIndex = 0)
            {
                return inResource->Unlock(this, inIndex);
            }

            RHIResourceRef Copy(IResourceCopyableRef& inResource, ECPU_GPU_ACCESS_MODE inMode)
            {
                return inResource->Copy(this, inMode);
            }
        };

        class RHIImmediateContext : public RHIContextBase {};

        class RHIDeferredContext : public RHIContextBase {};
    }
}

#endif // !__BLADE_RHI_RHI_CONTEXT_H__
