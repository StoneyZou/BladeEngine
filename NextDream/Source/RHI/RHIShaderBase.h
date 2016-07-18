#ifndef __BLADE_RHI_SHADER_BASE_H__
#define __BLADE_RHI_SHADER_BASE_H__

#include <RHIEnum.h>
#include <TArray.h>
#include <TMap.h>
#include <BString.h>
#include <Utility.h>
#include <MemUtil.h>
#include <RHIDevice.h>

namespace BladeEngine
{
    namespace RHI
    {
        #define MAX_RENDER_TARGET 8

        class RHIShaderResourceTable : public INoncopyable, public IReferencable
        {
        private:
            struct AttributionDesc
            {
                SIZE_T UnifromIndex;
                SIZE_T Offset;
                SIZE_T Size;
                ESHADER_ATTRIB_TYPE Type;
            };

            struct UniformBufferDesc
            {
                SIZE_T Slot;
                SIZE_T Size;
                void* DefaultData;
            };

            struct ResourceBindDesc
            {
                SIZE_T Slot;
                SIZE_T Count;
                ESHADER_RESOURCE_TYPE Type;
            };

        private:
            TArray<UniformBufferDesc> m_UniformBuffer;

            TArray<AttributionDesc> m_AttributionDescArr;
            TMap<BString, int32> m_AttributionDescMap;

            TArray<ResourceBindDesc> m_ResourceDescArr;
            TMap<BString, int32> m_ResourceDescMap;

        public:
            SIZE_T AddUniformBufferDesc(const BString& inBufferName, SIZE_T inSlot, SIZE_T inSize)
            {
                UniformBufferDesc desc;
                desc.Slot = inSlot;
                desc.Size = inSize;
                desc.DefaultData = SystemMalloc::GetInstance().Alloc(inSize);

                m_UniformBuffer.Add(desc);
            }

            void AddAttributionDesc(const BString& inAttrName, SIZE_T inUniformIndex, SIZE_T inOffset, SIZE_T inSize, ESHADER_ATTRIB_TYPE inType, void* inDefault)
            {
                AttributionDesc desc;
                desc.UnifromIndex = inUniformIndex;
                desc.Offset = inOffset;
                desc.Size = inSize;
                desc.Type = inType;

                SIZE_T index = m_AttributionDescArr.Length();
                m_AttributionDescArr.Add(desc);

                m_AttributionDescMap.Insert(inAttrName, index);
            }

            void AddResourceBindDesc(const BString& inResourceName, SIZE_T inSlot, SIZE_T inCount, ESHADER_RESOURCE_TYPE inType)
            {
                ResourceBindDesc desc;
                desc.Slot = inSlot;
                desc.Count = inCount;
                desc.Type = inType;

                SIZE_T index = m_ResourceDescArr.Length();
                m_ResourceDescArr.Add(desc);

                m_ResourceDescMap.Insert(inResourceName, index);
            }
        };

        class RHIShaderInputTable : public INoncopyable, public IReferencable
        {
        private:
            struct InputElementDesc
            {
                SIZE_T Index;
                SIZE_T Rigster;
                ESHADER_SEMANTIC_TYPE Semantic;
            };

            void AddResourceBindDesc(const BString& inResourceName, ESHADER_SEMANTIC_TYPE inSemantic, SIZE_T inRigster, SIZE_T inIndex)
            {
                InputElementDesc desc;
                desc.Semantic = inSemantic;
                desc.Rigster = inRigster;
                desc.Index = inIndex;

                m_InputElementDescArr.Add(desc);
            }

        private:
            TArray<InputElementDesc> m_InputElementDescArr;
        };


        class RHIShaderResourceTable;
        typedef RefCountObject<RHIShaderResourceTable> RHIShaderResourceTableRef;

        class RHIShaderInputTable;
        typedef RefCountObject<RHIShaderInputTable> RHIShaderInputTableRef;

        struct RHIShaderCreateInfo
        {
            BString Name;
            RHIShaderResourceTableRef ResourceTable;
            uint32 DataSize;
            void* Data;
        };

        struct RHIVertexShaderCreateInfo : public RHIShaderCreateInfo
        {
            RHIShaderInputTableRef InputTable;
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

            ECOLOR_BLEND_FACTOR SrcBlend;
            ECOLOR_BLEND_FACTOR DestBlend;
            ECOLOR_BLEND_FUNC BlendOp;

            ECOLOR_BLEND_FACTOR SrcBlendAlpha;
            ECOLOR_BLEND_FACTOR DestBlendAlpha;
            ECOLOR_BLEND_FUNC BlendOpAlpha;
        };

        struct RHIShaderBlendDesc
        {
            bool AlphaTest;
            bool IndependentBlendEnable;
            RHIRenderTargetBlendDesc RenderTarget[MAX_RENDER_TARGET];
        };

        struct RHIShaderDepthStencilDesc
        {
            bool DepthEnable;
            uint8 DepthWriteMask;

            bool StencilEnable;
            uint8 StencilReadMask;
            uint8 StencilWriteMask;

            EDEPTH_STENCIL_WRITE_FUNC FrontFaceSFailFunc;
            EDEPTH_STENCIL_WRITE_FUNC FrontFaceSPassDFailFunc;
            EDEPTH_STENCIL_WRITE_FUNC FrontFaceSPassDPassFunc;
            EDEPTH_STENCIL_COMPARISON_FUNC FrontFaceDepthFunc;
            EDEPTH_STENCIL_COMPARISON_FUNC FrontFaceStencilFunc;

            EDEPTH_STENCIL_WRITE_FUNC BackFaceStencilFailFunc;
            EDEPTH_STENCIL_WRITE_FUNC BackFaceStencilDepthFailFunc;
            EDEPTH_STENCIL_WRITE_FUNC BackFaceStencilPassFunc;
            EDEPTH_STENCIL_COMPARISON_FUNC FrontFaceDepthFunc;
            EDEPTH_STENCIL_COMPARISON_FUNC FrontFaceStencilFunc;
        };

        struct RHIShaderStateCreateInfo
        {
            RHIShaderRasterizerDesc     RasterizerDesc;
            RHIShaderBlendDesc          BlendDesc;
            RHIShaderDepthStencilDesc   DepthStencilDesc;
        };

        class RHIShaderState : public RHIResource
        {
        private:
            RHIShaderStateCreateInfo m_CreateInfo;
            
        public:
            RHIShaderState(const RHIShaderStateCreateInfo& inCreateInfo) : RHIResource(EONLY_GPU_READ),
                m_CreateInfo(inCreateInfo)
            {}

        public:
            bool operator != (const RHIShaderState& rh) const
            {
                return !(*this == rh);
            }

            bool operator == (const RHIShaderState& rh) const
            {
                if (this == &rh)
                    return true;

                return MemUtil::Memcmp(&m_CreateInfo, &rh.m_CreateInfo, sizeof(m_CreateInfo)) == 0 ;
            }

            int32 Compare(const RHIShaderState& rh) const
            {
                return MemUtil::Memcmp(&m_CreateInfo, &rh.m_CreateInfo, sizeof(m_CreateInfo));
            }
        };

        typedef RefCountObject<RHIShaderState> RHIShaderStateRef;

        class RHIShaderBase : public RHIResource
        {
        public:
            RHIShaderBase(const RHIShaderCreateInfo& inCreareInifo) : RHIResource(EONLY_GPU_READ), m_ResourceTable(inCreareInifo.ResourceTable)
            {}

        private:
            RHIShaderResourceTableRef m_ResourceTable;
        };

        class RHIVertexShader : public RHIShaderBase 
        {
        public:
            RHIVertexShader(
                const RHIVertexShaderCreateInfo& inCreateInfo) : RHIShaderBase(inCreateInfo), m_InputTable(inCreateInfo.InputTable)
            {}

        private:
            RHIShaderInputTableRef m_InputTable;
        };

        class RHIHullShader : public RHIShaderBase {};
        class RHIDomainShader : public RHIShaderBase {};
        class RHIGeometryShader : public RHIShaderBase {};
        class RHIPixelShader : public RHIShaderBase {};

    }
}

#endif // !__BLADE_RHI_SHADER_BASE_H__