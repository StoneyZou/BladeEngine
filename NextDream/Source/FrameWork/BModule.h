#ifndef __BLADE_FRAMEWORK_BMODULE_H__
#define __BLADE_FRAMEWORK_BMODULE_H__

#include <TypeDefine.h>
#include <BString.h>

namespace BladeEngine
{
    enum EMODULE_TYPE
    {
        EMODULE_RHI,
        EMODULE_SHADER_COMPILE,
    };

    enum EMODULE_RHI_SUBTYPE
    {
        EMODULE_RHI_DIRECTX11,
    };

    namespace Framework
    {
        class IModule
        {
        private:
            uint32 m_Type;
            uint32 m_SubType;

        public:
            IModule(uint32 inType, uint32 inSubType) : m_Type(inType), m_SubType(inSubType)
            {}

        public:
            uint32 GetModuleType() const { return m_Type; }
            uint32 GetModuleSubType() const { return m_SubType; }

        public:
            virtual bool Load(const BString& inFileName) = 0;
            virtual bool StartUp() = 0;
            virtual void ShutDown() = 0;
            virtual void Unload() = 0;
        };
    }
}
#endif // !__BLADE_FRAMEWORK_BMODULE_H__