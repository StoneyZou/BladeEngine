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
        class RHIShaderResourceTable : public RHIResource
        {
        public:
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
                SIZE_T PackSize;
                SIZE_T Offset;
            };

            struct ResourceBindDesc
            {
                SIZE_T Slot;
                SIZE_T Count;
                ESHADER_RESOURCE_TYPE Type;
            };

            typedef TMap<BString, int32> BStringToIntMap;

        private:
            SIZE_T m_TotalSize;
            byte* m_TotalData;

            TArray<UniformBufferDesc> m_UniformBufferArray;

            TArray<AttributionDesc>  m_AttributionDescArr;
            BStringToIntMap m_AttributionDescMap;

            TArray<ResourceBindDesc> m_ResourceDescArr;
            BStringToIntMap m_ResourceDescMap;

        public:
            SIZE_T AddUniformBufferDesc(const BString& inBufferName, SIZE_T inSlot, SIZE_T inPackSize)
            {
                UniformBufferDesc desc;
                desc.Slot = inSlot;
                desc.PackSize = inPackSize;

                m_UniformBufferArray.Add(desc);
            }

            void BuildUniformBuffers()
            {
                BladeAssert(m_TotalSize == 0);
                BladeAssert(m_TotalData == NULL);

                m_TotalSize = 0;
                for(SIZE_T i = 0; i < m_UniformBufferArray.Size(); ++i)
                {
                    m_TotalSize += m_UniformBufferArray[i].PackSize;
                }
                
                m_TotalData = (byte*)Malloc::Alloc(m_TotalSize);
                MemUtil::Memset(m_TotalData, 0, m_TotalSize);
            }

            void AddAttributionDesc(const BString& inAttrName, SIZE_T inUniformIndex, SIZE_T inOffset, SIZE_T inSize, ESHADER_ATTRIB_TYPE inType, void* inDefault)
            {
                BladeAssert(inUniformIndex >= 0 && inUniformIndex < m_UniformBufferArray.Size());

                AttributionDesc desc;
                desc.UnifromIndex = inUniformIndex;
                desc.Offset = inOffset;
                desc.Size = inSize;
                desc.Type = inType;

                m_AttributionDescMap.Insert(inAttrName, m_AttributionDescArr.Size());
                m_AttributionDescArr.Add(desc);

                const UniformBufferDesc& uniformBufferDesc = m_UniformBufferArray[inUniformIndex];
                BladeAssert(inSize >= 0 && inSize< (m_TotalSize - uniformBufferDesc.Offset - desc.Offset));

                MemUtil::Memcopy(
                    m_TotalData + uniformBufferDesc.Offset + desc.Offset,
                    inSize,
                    inDefault,
                    inSize);
            }

            void AddResourceBindDesc(const BString& inResourceName, SIZE_T inSlot, SIZE_T inCount, ESHADER_RESOURCE_TYPE inType)
            {
                ResourceBindDesc desc;
                desc.Slot = inSlot;
                desc.Count = inCount;
                desc.Type = inType;

                m_ResourceDescMap.Insert(inResourceName, m_ResourceDescArr.Size());
                m_ResourceDescArr.Add(desc);
            }

            bool GetResourceBindDesc(const BString& inResName, ResourceBindDesc* outDesc) const
            {
                BStringToIntMap::Iterator ite = m_ResourceDescMap.Find(inResName);
                if (ite == m_ResourceDescMap.End())
                {
                    return false;
                }

                if(outDesc != NULL)
                {
                    *outDesc = m_ResourceDescArr[ite.Value()];
                }
            }

            bool GetAttributionDesc(const BString& inAttrName, AttributionDesc* outDesc) const
            {
                BStringToIntMap::Iterator ite = m_AttributionDescMap.Find(inAttrName);
                if (ite == m_AttributionDescMap.End())
                {
                    return false;
                }

                if (outDesc != NULL)
                {
                    *outDesc = m_AttributionDescArr[ite.Value()];
                }
            }
        };

        class RHIShaderState : public RHIResource
        {
        private:
            RHIShaderStateCreateInfo m_CreateInfo;
            
        public:
            RHIShaderState(IRHIDevice* inDevice, const RHIShaderStateCreateInfo& inCreateInfo) : RHIResource(inDevice, EONLY_GPU_READ),
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

        class RHIShaderBase : public RHIResource
        {
        public:
            RHIShaderBase(IRHIDevice* inDevice, const RHIShaderCreateInfo& inCreareInifo) : RHIResource(inDevice, EONLY_GPU_READ), m_ResourceTable(inCreareInifo.ResourceTable)
            {}

            const RHIShaderResourceTableRef& GetResourceTable() const { return m_ResourceTable; }

        private:
            RHIShaderResourceTableRef m_ResourceTable;
        };

        class RHIVertexShader : public RHIShaderBase 
        {
        public:
            struct InputElementDesc
            {
                uint32 Index;
                uint32 Slot;
                ESHADER_SEMANTIC_TYPE Semantic;
            };

        public:
            typedef TArray<InputElementDesc> InputTable;

        private:
            InputTable m_InputTable;

        public:
            RHIVertexShader(IRHIDevice* inDevice, const RHIShaderCreateInfo& inCreateInfo) : RHIShaderBase(inDevice, inCreateInfo)
            {}

            void AddInputBindDesc(const BString& inResourceName, ESHADER_SEMANTIC_TYPE inSemantic, uint32 inSlot, SIZE_T inIndex)
            {
                InputElementDesc desc;
                desc.Semantic = inSemantic;
                desc.Slot = inSlot;
                desc.Index = inIndex;

                m_InputTable.Add(desc);
            }

            const InputTable& GetInputTable() const { return m_InputTable; }
        };

        class RHIHullShader : public RHIShaderBase 
        {
        public:
            RHIHullShader(IRHIDevice* inDevice, const RHIShaderCreateInfo& inCreateInfo) : RHIShaderBase(inDevice, inCreateInfo)
            {}
        };

        class RHIDomainShader : public RHIShaderBase
        {
        public:
            RHIDomainShader(IRHIDevice* inDevice, const RHIShaderCreateInfo& inCreateInfo) : RHIShaderBase(inDevice, inCreateInfo)
            {}
        };

        class RHIGeometryShader : public RHIShaderBase
        {
        public:
            RHIGeometryShader(IRHIDevice* inDevice, const RHIShaderCreateInfo& inCreateInfo) : RHIShaderBase(inDevice, inCreateInfo)
            {}
        };

        class RHIPixelShader : public RHIShaderBase
        {
        public:
            RHIPixelShader(IRHIDevice* inDevice, const RHIShaderCreateInfo& inCreateInfo) : RHIShaderBase(inDevice, inCreateInfo)
            {}
        };

    }
}

#endif // !__BLADE_RHI_SHADER_BASE_H__