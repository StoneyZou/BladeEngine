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
        #define MAX_SHADER_DEFINE_NUM 32

        class RHIResource : public INoncopyable
        {
        public:
            static TArray<RHIResource*> s_DeferedDeleteResources;

        private:
            ECPU_GPU_ACCESS_MODE m_AccessMode;
            NotThreadSafeRefCount m_RefCount;

        public:
            RHIResource(ECPU_GPU_ACCESS_MODE inAccessMode) : m_AccessMode(inAccessMode)
            {}

            virtual ~RHIResource() {}

        public:
            virtual void* GetPlatformSpecificPtr() = 0;
            const void* GetPlatformSpecificPtr() const { return GetPlatformSpecificPtr(); };

            ECPU_GPU_ACCESS_MODE GetAccessMode() const { return m_AccessMode; }

        public:
            int32 AddRef() { return m_RefCount.AddRef(); }
            int32 Release() 
            { 
                if (m_RefCount.Release() == 0) {
                    s_DeferedDeleteResources.Add(this);
                }
            }
            int32 GetRefCount() const { return m_RefCount.GetRefCount(); }
        };
        TArray<RHIResource*> RHIResource::s_DeferedDeleteResources;

        class RHIResource;
        typedef RefCountObject<RHIResource> RHIResourceRef;

        class RHITextureBase;
        typedef RefCountObject<RHITextureBase> RHITextureBaseRef;

        class RHIVertexBuffer;
        typedef RefCountObject<RHIVertexBuffer> RHIVertexBufferRef;

        class RHIIndexBuffer;
        typedef RefCountObject<RHIIndexBuffer> RHIIndexBufferRef;

        struct RHITexture2DCreateInfo;

        struct RHIShaderCreateInfo
        {
            BString Defines;
            uint32 DataSize;
            void* Data;
        };

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

        class IRHIDevice
        {
        public:
           virtual RHITextureBaseRef CreateTexture2D(const RHITexture2DCreateInfo& inCreateInfo) = 0;

           virtual void* Lock(RHIResourceRef& inResource, ELOCK_TYPE inType, const SIZE_T inIndex = 0) = 0;

           virtual void Unlock(RHIResourceRef& inResource, const SIZE_T inIndex = 0) = 0;

           virtual void Copy(RHIResourceRef& inDest, RHIResourceRef& inSrc) = 0;

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
#endif // !__BLADE_RHI_RHI_DEVICE_H__