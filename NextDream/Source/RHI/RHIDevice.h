#ifndef __BLADE_RHI_RHI_DEVICE_H__
#define __BLADE_RHI_RHI_DEVICE_H__

#include <Utility.h>
#include <RHIEnum.h>
#include <TArray.h>
#include <String.h>

namespace BladeEngine
{
    namespace RHI
    {
        #define MAX_SHADER_DEFINE_NUM 32

        class RHIShaderBase
        {
        private:
            struct AttributionDesc
            {
                SIZE_T Offset;
                SIZE_T Size;
                ESHADER_ATTRIB_TYPE Type;
            };

        private:
            
            TArray<AttributionDesc> m_AttributionDesc

        public:

        };

        template<ENUM_SHADER_TYPE ShaderType>
        class RHIShader : RHIShaderBase
        {};

        typedef RHIShader<VERTEX_SHADER> RHIVertexShader;
        typedef RHIShader<HULL_SHADER> RHIHullShader;
        typedef RHIShader<DOMAIM_SHADER> RHIDomainShader;
        typedef RHIShader<GEOMETRY_SHADER> RHIGeometryShader;
        typedef RHIShader<PIXEL_SHADER> RHIPixelShader;

        class RHITextureBase;
        typedef RefCountObject<RHITextureBase> RHITextureBaseRef;

        class RHIShaderBase;
        typedef RefCountObject<RHIShaderBase> RHIShaderBaseRef;

        class RHIVertexBuffer;
        typedef RefCountObject<RHIVertexBuffer> RHIVertexBufferRef;

        class RHIIndexBuffer;
        typedef RefCountObject<RHIIndexBuffer> RHIIndexBufferRef;

        typedef RefCountObject<RHIVertexShader> RHIVertexShaderRef;
        typedef RefCountObject<RHIHullShader> RHIHullShaderRef;
        typedef RefCountObject<RHIDomainShader> RHIDomainShaderRef;
        typedef RefCountObject<RHIGeometryShader> RHIGeometryShaderRef;
        typedef RefCountObject<RHIPixelShader> RHIPixelShaderRef;

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
            bool Writable; 
            bool AsRenderTarget;
            uint32 DataSize;
            void* Data;
        };

        struct RHIShaderCreateInfo
        {
        public:
            TArray<String> Defines;
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