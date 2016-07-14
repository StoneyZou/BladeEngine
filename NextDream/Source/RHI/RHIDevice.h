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
            NotThreadSafeRefCount m_RefCount;

        public:
            ~RHIResource() 
            {}

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

        class RHITextureBase;
        typedef RefCountObject<RHITextureBase> RHITextureBaseRef;

        class RHIVertexBuffer;
        typedef RefCountObject<RHIVertexBuffer> RHIVertexBufferRef;

        class RHIIndexBuffer;
        typedef RefCountObject<RHIIndexBuffer> RHIIndexBufferRef;

        /** 
        * @Desc                 Structure contains all infomations to create a 2d texture
        * @param EnableWrite    if value is true, it mean the texture can be writen by cpu, but can't use as render target 
        */
        struct RHITexture2DCreateInfo
        {
        public:
            uint32 Width;
            uint32 Height;
            ERHI_PIXEL_FORMAT Format;
            ECPU_GPU_ACCESS_MODE AccessMode;
            uint32 DataSize;
            void* Data;
        };

        struct RHIShaderCreateInfo
        {
        public:

            TArray<BString> Defines;
            TArray<uint32> DefinesHash;
            TArray<SIZE_T> CodeOffsetWithDefines;
            TArray<SIZE_T> CodeSizeWithDefines;
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