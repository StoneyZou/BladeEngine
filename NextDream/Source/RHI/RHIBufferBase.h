#ifndef __BLADE_RHI_UNIFORM_BUFFER_H__
#define __BLADE_RHI_UNIFORM_BUFFER_H__

#include <RHIDevice.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHIUniformBuffer : public RHIResource, public IResourceLockable
        {
        private:
            SIZE_T m_Size;

        public:
            RHIUniformBuffer(IRHIDevice* inDevice, const RHIUniformCreateInfo& inCreateInfo) 
                : RHIResource(inDevice, EGPU_READ_CPU_WRITE),
                m_Size(inCreateInfo.DataSize)
            {}
        };

        class RHIInputLayout : public RHIResource
        {
        private:
            RHIVertexBuffer* m_BindBuffer;
            RHIVertexShader* m_BindShader;

        public:
            RHIInputLayout(IRHIDevice* inDevice, const RHIInputLayoutCreateInfo& inCreateInfo)
                : RHIResource(inDevice, EONLY_GPU_READ),
                m_BindBuffer(inCreateInfo.BindBuffer),
                m_BindShader(inCreateInfo.BindShader)
            {}
        };

        class RHIVertexBuffer : public RHIResource, public IResourceLockable
        {
        public: 
            struct Declaration
            {
                ESHADER_SEMANTIC_TYPE Semantic;
                uint32 Index;
                EDATA_FORMAT Format;
                uint32 Offset;
                EINPUT_CLASSIFICATION InputType;
                uint32 InstanceDataStepRate;
            };

        public:
            typedef TArray<Declaration> DeclarationTable;

        private:
            SIZE_T m_VertexNum;
            SIZE_T m_Size;
            DeclarationTable m_DeclarationTable;

        public:
            RHIVertexBuffer(IRHIDevice* inDevice, const RHIVertexBufferCreateInfo& inCreateInfo)
                : RHIResource(inDevice, inCreateInfo.Writable ? EGPU_READ_CPU_WRITE : EONLY_GPU_READ),
                m_Size(inCreateInfo.DataSize)
            {}

            void AddVertexDeclaration(ESHADER_SEMANTIC_TYPE inSemantic, SIZE_T inIndex, EDATA_FORMAT Format, SIZE_T inOffset)
            {
                m_DeclarationTable.Add(Declaration{ inSemantic, inIndex, Format, inOffset });
            }

            const DeclarationTable& GetDeclaration() const { return m_DeclarationTable; }
        };
    }
}

#endif // !__BLADE_RHI_UNIFORM_BUFFER_H__