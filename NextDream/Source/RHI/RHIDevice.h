#ifndef __BLADE_RHI_RHI_DEVICE_H__
#define __BLADE_RHI_RHI_DEVICE_H__

#include <TMap.h>
#include <BString.h>
#include <Utility.h>
#include <BModule.h>
#include <RHIEnum.h>
#include <TArray.h>

namespace BladeEngine
{
    namespace RHI
    {
        #define MAX_NUM_RENDER_TARGET 8

        class IRHIDevice;

        class RHIResource : public INoncopyable
        {
        private:
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
			void InitRHI();
			void ReleaseRHI();

        public:
            ECPU_GPU_ACCESS_MODE GetAccessMode() const { return m_AccessMode; }
            bool CanCpuRead()  const    { return ((m_AccessMode & ECPU_READ) != 0); }
            bool CanCpuWrite() const    { return ((m_AccessMode & ECPU_WRITE) != 0); }
            bool CanGpuRead()  const    { return ((m_AccessMode & EGPU_READ) != 0); }
            bool CanGpuWrite() const    { return ((m_AccessMode & EGPU_WRITE) != 0); }

        public:
            int32 AddRef() const { return m_RefCount.AddRef(); }
            int32 Release() const { if (m_RefCount.GetRefCount() == 1) { delete this; } return m_RefCount.Release(); };
            int32 GetRefCount() const { return m_RefCount.GetRefCount(); }
            int32 IsUnique() const { return m_RefCount.GetRefCount() == 1; }
        };

        class RHIContextBase;
        typedef RefCountObject<RHIContextBase> RHIContextBaseRef;

        class RHIImmediateContext;
        typedef RefCountObject<RHIImmediateContext> RHIImmediateContextRef;

        class RHIDeferredContext;
        typedef RefCountObject<RHIDeferredContext> RHIDeferredContextRef;

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

        class RHITexture2D;
        typedef RefCountObject<RHITexture2D> RHITexture2DRef;

        class RHISwapChain;
        typedef RefCountObject<RHISwapChain> RHISwapChainRef;

        class RHIVertexBuffer;
        typedef RefCountObject<RHIVertexBuffer> RHIVertexBufferRef;

        class RHIIndexBuffer;
        typedef RefCountObject<RHIIndexBuffer> RHIIndexBufferRef;

        class RHIShaderResourceTable;
        typedef RefCountObject<RHIShaderResourceTable> RHIShaderResourceTableRef;

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

        class RHIInputLayout;
        typedef RefCountObject<RHIInputLayout> RHIInputLayoutRef;

        class RHIShaderState;
        typedef RefCountObject<RHIShaderState> RHIShaderStateRef;

        class RHIShaderResourceTable;
        typedef RefCountObject<RHIShaderResourceTable> RHIShaderResourceTableRef;

        struct RHIVertexBufferCreateInfo
        {
            bool CanCpuWrite;
            uint32 VertexNum;
            uint32 DataSize;
            const void* Data;
        };

        struct RHIIndexBufferCreateInfo
        {
            uint32 VertexNumber;
            uint32 DataSize;
            void* Data;
        };

        struct RHIUniformCreateInfo
        {
            uint32 DataSize;
            void* Data;
        };

        struct RHIInputElementDesc
        {
            RHIVertexBuffer* Buffer;
            ESHADER_SEMANTIC_TYPE Semantic;
            uint32 Index;
        };

        struct RHIShaderCreateInfo
        {
            BString Name;
            RHIShaderResourceTableRef ResourceTable;
            uint32 DataSize;
            const void* Data;
        };

        struct RHIShaderRasterizerDesc
        {
            RHIShaderRasterizerDesc() :
                FillMode(EMESH_FILL_SOLID),
                CullMode(EFACE_CULL_BACK),
                FrontCounterClockwise(false),
                DepthBias(0)
            {}

            EMESH_FILL_MODE FillMode;
            EFACE_CULL_MODE CullMode;
            bool FrontCounterClockwise;
            uint32 DepthBias;
            float DepthBiasClamp;
            float SlopeScaledDepthBias;
            bool DepthClipEnable;
            bool ScissorEnable;
            bool MultisampleEnable;
            bool AntialiasedLineEnable;
        };

        struct RHIRenderTargetBlendDesc
        {
            bool BlendEnable;
            uint8 RenderTargetWriteMask;

            EBLEND_ARG SrcBlend;
            EBLEND_ARG DestBlend;
            EBLEND_FUNC BlendOp;

            EBLEND_ARG SrcBlendAlpha;
            EBLEND_ARG DestBlendAlpha;
            EBLEND_FUNC BlendOpAlpha;
        };

        struct RHIShaderBlendDesc
        {
            bool AlphaTest;
            bool IndependentBlendEnable;
            RHIRenderTargetBlendDesc RenderTarget[MAX_NUM_RENDER_TARGET];
        };

        struct RHIShaderDepthStencilDesc
        {
            bool DepthEnable;
            bool StencilEnable;
            uint8 StencilReadMask;
            uint8 StencilWriteMask;

            ECOMPARISON_FUNC DepthFunc;

            EDEPTH_STENCIL_WRITE_FUNC FrontFaceSFailFunc;
            EDEPTH_STENCIL_WRITE_FUNC FrontFaceSPassDFailFunc;
            EDEPTH_STENCIL_WRITE_FUNC FrontFaceSPassDPassFunc;
            ECOMPARISON_FUNC FrontFaceStencilFunc;

            EDEPTH_STENCIL_WRITE_FUNC BackFaceSFailFunc;
            EDEPTH_STENCIL_WRITE_FUNC BackFaceSPassDFailFunc;
            EDEPTH_STENCIL_WRITE_FUNC BackFaceSPassDPassFunc;
            ECOMPARISON_FUNC BackFaceStencilFunc;
        };

        struct RHIShaderStateCreateInfo
        {
            RHIShaderRasterizerDesc     RasterizerDesc;
            RHIShaderBlendDesc          BlendDesc;
            RHIShaderDepthStencilDesc   DepthStencilDesc;
        };
        
        struct RHITexuteSamplerInfo
        {
            ETEXTURE_FILTER_MODE Filter;
            ETEXTURE_ADDRESS_MODE AddressU;
            ETEXTURE_ADDRESS_MODE AddressV;
            ETEXTURE_ADDRESS_MODE AddressW;
            float MipLODBias;
            uint32 MaxAnisotropy;
            ECOMPARISON_FUNC ComparisonFunc;
            float BorderColor[4];
            float MinLOD;
            float MaxLOD;
        };

        /**
        * @Desc Structure contains all infomations to create a texture
        */
        struct RHITextureCreateInfo
        {
            RHITexuteSamplerInfo Sampler;
            uint32 Width, Height, Depth;
            uint32 SampleCount, SampleQulity;
            uint32 Usage;
            EDATA_FORMAT BaseFormat;
            EDATA_FORMAT RenderTargetFormat;
            EDATA_FORMAT DepthStencilFormat;
            ECPU_GPU_ACCESS_MODE AccessMode;
            uint32 DataSize;
            void* Data;
        };

        struct RHITextureInitInfo
        {
            bool CanAsRenderTarget, CanAsDepthStencil;
            uint32 Width, Height;
            uint32 SampleCount, SampleQulity;
            uint32 Usage;
            EDATA_FORMAT BaseFormat;
            ECPU_GPU_ACCESS_MODE AccessMode;
        };

        struct RHISwapChainCreateInfo
        {
            RHITexuteSamplerInfo Sampler;
            uint32 BufferNum;
            uint32 SampleCount;
            uint32 SampleQulity;
            uint32 RefreshRateDenominator;
            uint32 RefreshRateNumerator;
            PlatformWindowRef Window;
        };

        struct RHISwapChainInitInfo
        {
            uint32 Width, Height;
            uint32 BufferNum;
            uint32 SampleCount;
            uint32 SampleQulity;
            uint32 RefreshRateDenominator;
            uint32 RefreshRateNumerator;
            PlatformWindowRef Window;
            RHITexture2DRef Texture;
        };

        class IRHIDevice : public IReferencable
        {
        protected:
            TArray<RHIResource*> m_DeleteResourceList;

        public:
           virtual RHITexture2DRef CreateTexture2D(const RHITextureCreateInfo& inCreateInfo) = 0;

           virtual RHIVertexShaderRef CreateVextexShader(const RHIShaderCreateInfo&) = 0;

           virtual RHIPixelShaderRef CreatePixelShader(const RHIShaderCreateInfo&) = 0;

           virtual RHIHullShaderRef CreateHullShader(const RHIShaderCreateInfo&) = 0;

           virtual RHIDomainShaderRef CreateDomainShader(const RHIShaderCreateInfo&) = 0;

           virtual RHIGeometryShaderRef CreateGeometryShader(const RHIShaderCreateInfo&) = 0;

           virtual RHIVertexBufferRef CreateVertexBuffer(const RHIVertexBufferCreateInfo&) = 0;

           virtual RHIShaderStateRef CreateShaderState(const RHIShaderStateCreateInfo&) = 0;

           virtual RHIUniformBufferRef CreateUniformBuffer(const RHIUniformCreateInfo&) = 0; 
           
           virtual RHISwapChainRef CreateSwapChain(PlatformWindow& inWindow);

        public:
            virtual RHIImmediateContextRef GetImmediateContext() = 0;

            virtual RHIDeferredContextRef CreateDeferredContext() = 0;
        };
    }

    struct IRHIModule : public Framework::IModule
    {
    protected:
        // 需要在子类赋值，放在这里只是为了避免多态
        RHI::IRHIDevice* m_Device;

        uint32 m_bestAdapterIndex;
        TArray<BString> m_AdapterNames;

    private:
        static const BString NullAdapterName;

    protected:
        IRHIModule(uint32 inSubType) : m_Device(NULL), IModule((uint32)EMODULE_RHI, inSubType)
        {}

    public:
        virtual bool InitDevice(uint32 inAdapterIndex) = 0;

    public:
        uint32 GetAdapterNum() const { return m_AdapterNames.Size(); }
        
        const BString& GetAdapterName(uint32 inIndex) const 
        { 
            if(inIndex < 0 || inIndex >= m_AdapterNames.Size())
            {
                return NullAdapterName;
            }
            return m_AdapterNames[inIndex]; 
        }

        //virtual void SwitchAdapterByIndex(uint32 inIndex) const = 0;

        uint32 GetBestAdapterIndex() const { return m_bestAdapterIndex; }

        RHI::IRHIDevice* GetDevice() { return m_Device; }
    };
}
#endif // !__BLADE_RHI_RHI_DEVICE_H__