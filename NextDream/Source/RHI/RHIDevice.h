#ifndef __BLADE_RHI_RHI_DEVICE_H__
#define __BLADE_RHI_RHI_DEVICE_H__

#include <RHIEnum.h>
#include <TArray.h>
#include <TMap.h>
#include <BString.h>
#include <Utility.h>

namespace BladeEngine
{
    namespace RHI
    {
        #define MAX_NUM_RENDER_TARGET 8

        class IRHIDevice;
        class RHIContextBase;

        class RHIResource : public INoncopyable
        {
        private:
            bool m_IsRecycle;
            ECPU_GPU_ACCESS_MODE m_AccessMode;
            NotThreadSafeRefCount m_RefCount;

        protected:
            IRHIDevice* m_Device;

        public:
            RHIResource(IRHIDevice* inDevice, ECPU_GPU_ACCESS_MODE inAccessMode) 
                : m_AccessMode(inAccessMode), 
                m_Device(inDevice)
            {}

            virtual ~RHIResource() {}

        public:
            ECPU_GPU_ACCESS_MODE GetAccessMode() const { return m_AccessMode; }

        public:
            int32 AddRef() const { return m_RefCount.AddRef(); }
            int32 Release() const { delete this; };
            int32 GetRefCount() const { return m_RefCount.GetRefCount(); }
            int32 IsUnique() const { return m_RefCount.GetRefCount() == 1; }
        };

        class IResourceLockable
        {
            friend RHIContextBase;
        protected:
            virtual void* Lock(RHIContextBase* inContext, ERES_LOCK_TYPE inType, const SIZE_T inIndex) = 0;
            virtual void Unlock(RHIContextBase* inContext, const SIZE_T inIndex) = 0;
        };

        class IResourceCopyable
        {
            friend RHIContextBase;
        protected:
            virtual RHIResource* Copy(RHIContextBase* inContext, ECPU_GPU_ACCESS_MODE inMode) = 0;
        };

        class IResourceLockable;
        typedef RefCountObject<IResourceLockable> IResourceLockableRef;

        class IResourceCopyable;
        typedef RefCountObject<IResourceCopyable> IResourceCopyableRef;

        class RHIResource;
        typedef RefCountObject<RHIResource> RHIResourceRef;

        class RHITextureBase;
        typedef RefCountObject<RHITextureBase> RHITextureBaseRef;

        class RHIVertexBuffer;
        typedef RefCountObject<RHIVertexBuffer> RHIVertexBufferRef;

        class RHIIndexBuffer;
        typedef RefCountObject<RHIIndexBuffer> RHIIndexBufferRef;

        struct RHIShaderCreateInfo;

        class RHIVertexShader;
        typedef RefCountObject<RHIVertexShader> RHIVertexShaderRef;
        
        class RHIHullShader;
        typedef RefCountObject<RHIHullShader> RHIHullShaderRef;

        class RHIDomainShader;
        typedef RefCountObject<RHIDomainShader> RHIDomainShaderRef;

        class RHIGeometryShader;
        typedef RefCountObject<RHIGeometryShader> RHIGeometryShaderRef;

        class RHIPixelShader;
        typedef RefCountObject<RHIPixelShader> RHIPixelShaderRef;

        struct RHIShaderStateCreateInfo;

        class RHIShaderState;
        typedef RefCountObject<RHIShaderState> RHIShaderStateRef;

        class RHIUniformBuffer;
        typedef RefCountObject<RHIUniformBuffer> RHIUniformBufferRef;

        class RHISampler;
        typedef RefCountObject<RHIUniformBuffer> RHISamplerRef;

        struct RHITexture2DCreateInfo;

        struct RHIVertexBufferCreateInfo
        {
        public:
            uint32 VertexNumber;
            uint32 DataSize;
            void* Data;
        };

        struct RHIIndexBufferCreateInfo
        {
        public:
            uint32 VertexNumber;
            uint32 DataSize;
            void* Data;
        };

        struct RHIUniformCreateInfo
        {
            uint32 DataSize;
            void* Data;
        };

        class IRHIDevice
        {
        protected:
            TArray<RHIResource*> m_DeleteResourceList;

        public:
           virtual RHITextureBaseRef CreateTexture2D(const RHITexture2DCreateInfo& inCreateInfo) = 0;

           virtual RHIVertexShaderRef CreateVextexShader(const RHIShaderCreateInfo) = 0;

           virtual RHIPixelShaderRef CreatePixelShader(const RHIShaderCreateInfo) = 0;

           virtual RHIHullShaderRef CreateHullShader(const RHIShaderCreateInfo) = 0;

           virtual RHIDomainShaderRef CreateDomainShader(const RHIShaderCreateInfo) = 0;

           virtual RHIGeometryShaderRef CreateGeometryShader(const RHIShaderCreateInfo) = 0;

           virtual RHIVertexBufferRef CreateVertexBuffer(const RHIVertexBufferCreateInfo) = 0;

           virtual RHIIndexBufferRef CreateIndexBuffer(const RHIIndexBufferCreateInfo) = 0;

           virtual RHIShaderStateRef CreateShaderState(const RHIShaderStateCreateInfo&) = 0;

           virtual RHIUniformBufferRef CreateUniformBuffer(const RHIUniformCreateInfo&) = 0;

           virtual RHISamplerRef CreateSampler(const RHIUniformCreateInfo&) = 0;
        };
    }
}
#endif // !__BLADE_RHI_RHI_DEVICE_H__