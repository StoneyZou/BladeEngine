#ifndef __BLADE_RHI_SHADER_BASE_H__
#define __BLADE_RHI_SHADER_BASE_H__

#include <RHIEnum.h>
#include <TArray.h>
#include <TMap.h>
#include <BString.h>
#include <Utility.h>
#include <MemUtil.h>
#include <RHIDevice.h>
#include <BArchiveBase.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHIShaderResourceTable : public IReferencable
        {
        public:
            struct AttributionDesc
            {
                SIZE_T UnifromIndex;
                SIZE_T Offset;
                SIZE_T Size;
                ESHADER_ATTRIB_TYPE Type;
                SIZE_T Rows;
                SIZE_T Cols;
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
            ~RHIShaderResourceTable()
            {
                if(m_TotalData != NULL)
                {
                    Malloc::Free(m_TotalData);
                    m_TotalData = NULL;
                }

                m_UniformBufferArray.Clear();
                m_AttributionDescArr.Clear();
                m_AttributionDescMap.Clear();
                m_ResourceDescArr.Clear();
                m_ResourceDescMap.Clear();
            }

        public:
            SIZE_T AddUniformBufferDesc(const BString& inBufferName, SIZE_T inSlot, SIZE_T inPackSize)
            {
                UniformBufferDesc desc;
                desc.Slot = inSlot;
                desc.PackSize = inPackSize;

                m_UniformBufferArray.Add(desc);
                return m_UniformBufferArray.Size() - 1;
            }

            void BuildUniformBuffers()
            {
                BladeAssert(m_TotalSize == 0);
                BladeAssert(m_TotalData == NULL);

                m_TotalSize = 0;
                for(SIZE_T i = 0; i < m_UniformBufferArray.Size(); ++i)
                {
                    m_UniformBufferArray[i].Offset = m_TotalSize;
                    m_TotalSize += m_UniformBufferArray[i].PackSize;
                }
                
                m_TotalData = (byte*)Malloc::Alloc(m_TotalSize);
                MemUtil::Memset(m_TotalData, 0, m_TotalSize);
            }

            void AddAttributionDesc(const BString& inAttrName, SIZE_T inUniformIndex, SIZE_T inOffset, SIZE_T inSize, 
                ESHADER_ATTRIB_TYPE inType, SIZE_T inRows, SIZE_T inCols, void* inDefault)
            {
                BladeAssert(inUniformIndex >= 0 && inUniformIndex < m_UniformBufferArray.Size());

                AttributionDesc desc;
                desc.UnifromIndex = inUniformIndex;
                desc.Offset = inOffset;
                desc.Size = inSize;
                desc.Type = inType;
                desc.Rows = inRows;
                desc.Cols = inCols;

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

        public:
            friend IWriter& operator << (IWriter& inWriter, const RHIShaderResourceTable& inResourceTable)
            {
                inWriter << inResourceTable.m_TotalSize;
                inWriter.Write(inResourceTable.m_TotalData, inResourceTable.m_TotalSize);

                inWriter << inResourceTable.m_UniformBufferArray.Size();
                for (uint32 i = 0; i < inResourceTable.m_UniformBufferArray.Size(); ++i)
                {
                    inWriter << inResourceTable.m_UniformBufferArray[i].Offset;
                    inWriter << inResourceTable.m_UniformBufferArray[i].PackSize;
                    inWriter << inResourceTable.m_UniformBufferArray[i].Slot;
                }

                inWriter << inResourceTable.m_AttributionDescArr.Size();
                for (uint32 i = 0; i < inResourceTable.m_AttributionDescArr.Size(); ++i)
                {
                    inWriter << inResourceTable.m_AttributionDescArr[i].UnifromIndex;
                    inWriter << inResourceTable.m_AttributionDescArr[i].Offset;
                    inWriter << inResourceTable.m_AttributionDescArr[i].Size;

                    inWriter << (SIZE_T)inResourceTable.m_AttributionDescArr[i].Type;

                    inWriter << inResourceTable.m_AttributionDescArr[i].Rows;
                    inWriter << inResourceTable.m_AttributionDescArr[i].Cols;
                }

                inWriter << inResourceTable.m_AttributionDescMap.Size();
                BStringToIntMap::Iterator attributionDescIte = inResourceTable.m_AttributionDescMap.Begin();
                while (attributionDescIte != inResourceTable.m_AttributionDescMap.End())
                {
                    inWriter << attributionDescIte.Key();
                    inWriter << attributionDescIte.Value();
                }

                inWriter << inResourceTable.m_ResourceDescArr.Size();
                for (uint32 i = 0; i < inResourceTable.m_ResourceDescArr.Size(); ++i)
                {
                    inWriter << inResourceTable.m_ResourceDescArr[i].Slot;
                    inWriter << inResourceTable.m_ResourceDescArr[i].Count;

                    inWriter << (SIZE_T)inResourceTable.m_ResourceDescArr[i].Type;
                }

                inWriter << inResourceTable.m_ResourceDescMap.Size();
                BStringToIntMap::Iterator resourceDescMapIte = inResourceTable.m_ResourceDescMap.Begin();
                while (resourceDescMapIte != inResourceTable.m_ResourceDescMap.End())
                {
                    inWriter << resourceDescMapIte.Key();
                    inWriter << resourceDescMapIte.Value();
                }

                return inWriter;
            }

            friend IReader& operator >> (IReader& inReader, RHIShaderResourceTable& inResourceTable)
            {
                inReader >> inResourceTable.m_TotalSize;

                inResourceTable.m_TotalData = (byte*)Malloc::Alloc(inResourceTable.m_TotalSize);
                inReader.Read(inResourceTable.m_TotalData, inResourceTable.m_TotalSize);

                SIZE_T uniformBufferArraySize = 0;
                inReader >> uniformBufferArraySize;
                for (uint32 i = 0; i < uniformBufferArraySize; ++i)
                {
                    UniformBufferDesc desc;
                    inReader >> desc.Offset;
                    inReader >> desc.PackSize;
                    inReader >> desc.Slot;

                    inResourceTable.m_UniformBufferArray.Add(desc);
                }

                SIZE_T attributionDescArrSize = 0;
                inReader >> attributionDescArrSize;
                for (uint32 i = 0; i < attributionDescArrSize; ++i)
                {
                    AttributionDesc desc;
                    inReader >> desc.UnifromIndex;
                    inReader >> desc.Offset;
                    inReader >> desc.Size;

                    SIZE_T type = 0;
                    inReader >> type;
                    desc.Type = (ESHADER_ATTRIB_TYPE)type;

                    inReader >> desc.Rows;
                    inReader >> desc.Cols;

                    inResourceTable.m_AttributionDescArr.Add(desc);
                }

                SIZE_T attributionDescMapSize = 0;
                inReader >> attributionDescMapSize;
                for (uint32 i = 0; i < attributionDescMapSize; ++i)
                {
                    BString key;
                    uint32 value;

                    inReader >> key;
                    inReader >> value;

                    inResourceTable.m_AttributionDescMap.Insert(key, value);
                }

                SIZE_T resourceDescArrSize = 0;
                inReader >> resourceDescArrSize;
                for (uint32 i = 0; i < resourceDescArrSize; ++i)
                {
                    ResourceBindDesc desc;
                    inReader >> desc.Slot;
                    inReader >> desc.Count;

                    SIZE_T type = 0;
                    inReader >> type;
                    desc.Type = (ESHADER_RESOURCE_TYPE)type;

                    inResourceTable.m_ResourceDescArr.Add(desc);
                }

                SIZE_T resourceDescMapSize = 0;
                inReader >> resourceDescMapSize;
                for (uint32 i = 0; i < resourceDescMapSize; ++i)
                {
                    BString key;
                    uint32 value;

                    inReader >> key;
                    inReader >> value;

                    inResourceTable.m_ResourceDescMap.Insert(key, value);
                }

                return inReader;
            }
        };

        class RHIShaderInputTable
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

        public:
            InputTable m_InputTable;
            
        public:
            void AddInputBindDesc(ESHADER_SEMANTIC_TYPE inSemantic, SIZE_T inIndex, uint32 inSlot)
            {
                InputElementDesc desc;
                desc.Semantic = inSemantic;
                desc.Slot = inSlot;
                desc.Index = inIndex;

                m_InputTable.Add(desc);
            }

        public:
            friend IWriter& operator << (IWriter& inWriter, const RHIShaderInputTable& inInpuTable)
            {
                inWriter << inInpuTable.m_InputTable.Size();
                for (uint32 i = 0; i < inInpuTable.m_InputTable.Size(); ++i)
                {
                    inWriter << inInpuTable.m_InputTable[i].Index;
                    inWriter << inInpuTable.m_InputTable[i].Slot;
                    inWriter << (SIZE_T)inInpuTable.m_InputTable[i].Semantic;
                }

                return inWriter;
            }

            friend IReader& operator >> (IReader& inReader, RHIShaderInputTable& inInpuTable)
            {
                SIZE_T inputTableSize = 0;
                inReader >> inputTableSize;
                for (uint32 i = 0; i < inputTableSize; ++i)
                {
                    InputElementDesc desc;
                    inReader >> desc.Index;
                    inReader >> desc.Slot;

                    SIZE_T semantic = 0;
                    inReader >> semantic;
                    desc.Semantic = (ESHADER_SEMANTIC_TYPE)semantic;

                    inInpuTable.m_InputTable.Add(desc);
                }
                
                return inReader;
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

        public:
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
        private:
            RHIShaderInputTable m_InputTable;

        public:
            RHIVertexShader(IRHIDevice* inDevice, const RHIShaderCreateInfo& inCreateInfo) : RHIShaderBase(inDevice, inCreateInfo)
            {}

            void AddInputBindDesc(ESHADER_SEMANTIC_TYPE inSemantic, uint32 inSlot, SIZE_T inIndex)
            {
                m_InputTable.AddInputBindDesc(inSemantic, inSlot, inIndex);
            }

            RHIShaderInputTable& InputTable() { return m_InputTable; }
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