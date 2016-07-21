#ifndef __BLADE_RHI_RHI_CONTEXT_H__
#define __BLADE_RHI_RHI_CONTEXT_H__

#include <RHIDevice.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHIContextBase;

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

            virtual void Flush();

            //virtual bool SetUniformBuffer(RHIShaderState* inRHIShader) = 0;
        };

        class RHIContextBase
        {
        private:
            bool m_IsDeferred;
            IRHIContextBaseImpl* m_Impl;
            
        private:
            TArray<RHIResourceRef> m_ResourcesInContext;

        protected:
            RHIContextBase(IRHIContextBaseImpl* inImpl, bool inIsDeffered) : m_Impl(inImpl), m_IsDeferred(inIsDeffered)
            {}

        public:
            IRHIContextBaseImpl* GetImpl() { return m_Impl; }
            const IRHIContextBaseImpl* GetImpl() const { return m_Impl; }

        public:
            bool IsDeferred() const { return m_IsDeferred; }

        public:
            void Flush()
            {
                m_ResourcesInContext.Clear();

            }
            
            void SetVertexShader(RHIVertexShaderRef& inRHIShader) 
            { 
                m_ResourcesInContext.Add(inRHIShader);  
                m_Impl->SetVertexShader(inRHIShader.GetReferencePtr()); 
            }

            void SetPixelShader(RHIPixelShaderRef& inRHIShader) 
            {
                m_ResourcesInContext.Add(inRHIShader);  
                m_Impl->SetPixelShader(inRHIShader.GetReferencePtr());
            }

            void SetGeometryShader(RHIGeometryShaderRef& inRHIShader) 
            {
                m_ResourcesInContext.Add(inRHIShader);  
                m_Impl->SetGeometryShader(inRHIShader.GetReferencePtr()); 
            }

            void SetHullShader(RHIHullShaderRef& inRHIShader) 
            {
                m_ResourcesInContext.Add(inRHIShader);  
                m_Impl->SetHullShader(inRHIShader.GetReferencePtr()); 
            }

            void SetDomainShader(RHIDomainShaderRef& inRHIShader) 
            {
                m_ResourcesInContext.Add(inRHIShader);  
                m_Impl->SetDomainShader(inRHIShader.GetReferencePtr()); 
            }

            void SetShaderState(RHIShaderStateRef& inRHIShaderState) 
            {
                m_ResourcesInContext.Add(inRHIShaderState);
                m_Impl->SetShaderState(inRHIShaderState.GetReferencePtr()); 
            }

            void SetTexture(RHITextureBaseRef inTex) 
            {
                m_ResourcesInContext.Add(inTex);
                m_Impl->SetTexture(inTex.GetReferencePtr());
            }

            void* Lock(IResourceLockableRef& inResource, ERES_LOCK_TYPE inType, const SIZE_T inIndex = 0)
            {
                if ((((RHIResourceRef)inResource)->GetAccessMode() & ECPU_READ) == 0)
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
