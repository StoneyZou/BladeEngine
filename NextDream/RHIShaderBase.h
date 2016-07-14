#ifndef __BLADE_RHI_SHADER_BASE_H__
#define __BLADE_RHI_SHADER_BASE_H__

#include <RHIDevice.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHIShaderResourceTable;
        typedef RefCountObject<RHIShaderResourceTable> RHIShaderResourceTableRef;

        class RHIShaderResourceTable;
        typedef RefCountObject<RHIShaderResourceTable> RHIShaderResourceTableRef;

        typedef RefCountObject<RHIVertexShader> RHIVertexShaderRef;
        typedef RefCountObject<RHIHullShader> RHIHullShaderRef;
        typedef RefCountObject<RHIDomainShader> RHIDomainShaderRef;
        typedef RefCountObject<RHIGeometryShader> RHIGeometryShaderRef;
        typedef RefCountObject<RHIPixelShader> RHIPixelShaderRef;

        class RHIShaderResourceTable : public INoncopyable
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

        class RHIShaderInputTable : public INoncopyable
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

        class RHIShaderBase : public RHIResource
        {
        public:
            RHIShaderBase(const RHIShaderResourceTableRef& inResourceTable) : m_ResourceTable(inResourceTable)
            {}

        private:
            RHIShaderResourceTableRef m_ResourceTable;
        };

        class RHIVertexShader : public RHIShaderBase 
        {
        public:
            RHIVertexShader(
                const RHIShaderInputTable& inInputTable,
                const RHIShaderResourceTableRef& inResourceTable) : RHIShaderBase(inResourceTable), m_InputTable(inInputTable)
            {}

        private:
            RHIShaderResourceTableRef m_InputTable;
        };

        class RHIHullShader : public RHIShaderBase {};
        class RHIDomainShader : public RHIShaderBase {};
        class RHIGeometryShader : public RHIShaderBase {};
        class RHIPixelShader : public RHIShaderBase {};

    }
}

#endif // !__BLADE_RHI_SHADER_BASE_H__