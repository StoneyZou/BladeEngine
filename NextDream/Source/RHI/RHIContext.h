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

            virtual void SetShaderResource(RHITextureBase* inTexture, ESHADER_TYPE inType, SIZE_T inSlot) = 0;

            virtual void Flush() = 0;

            //virtual bool SetUniformBuffer(RHIShaderState* inRHIShader) = 0;
        };

        class RHIContextBase : public IReferencable
        {
        private:
            bool m_IsDeferred;
            IRHIContextBaseImpl* m_Impl;
            
        private:
            TArray<RHIResourceRef> m_ResourcesInContext;

        protected:
            RHIContextBase(IRHIContextBaseImpl* inImpl, bool inIsDeffered) : m_Impl(inImpl), m_IsDeferred(inIsDeffered)
            {
            }

        public:
            ~RHIContextBase() 
            {
                if (m_Impl != NULL)
                {
                    delete m_Impl;
                }
            }

        public:
            IRHIContextBaseImpl* GetImpl() { return m_Impl; }
            const IRHIContextBaseImpl* GetImpl() const { return m_Impl; }

        public:
            bool IsDeferred() const { return m_IsDeferred; }

        public:
            void Flush()
            {
                m_ResourcesInContext.Clear();
                m_Impl->Flush();
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

            void SetShaderResource(RHITextureBaseRef inRHITexture, ESHADER_TYPE inType, SIZE_T inSlot)
            {
                m_ResourcesInContext.Add(inRHITexture);
                m_Impl->SetShaderResource(inRHITexture.GetReferencePtr(), inType, inSlot);
            }

            void* Lock(IResourceLockableRef& inResource, ERES_LOCK_TYPE inType, const SIZE_T inIndex = 0)
            {
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

        class RHIImmediateContext : public RHIContextBase 
        {
        public:
            RHIImmediateContext(IRHIContextBaseImpl* inImpl) : RHIContextBase(inImpl, false)
            {}
        };

        class RHIDeferredContext : public RHIContextBase 
        {
        public:
            RHIDeferredContext(IRHIContextBaseImpl* inImpl) : RHIContextBase(inImpl, true)
            {}
        };
    }
}

#endif // !__BLADE_RHI_RHI_CONTEXT_H__
