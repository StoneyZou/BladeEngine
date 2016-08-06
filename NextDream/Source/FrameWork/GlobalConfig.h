#ifndef __BLADE_FRAMEWORK_GLOBAL_CONFIG_H__
#define __BLADE_FRAMEWORK_GLOBAL_CONFIG_H__

#include <TypeDefine.h>
#include <BString.h>

namespace BladeEngine
{
    namespace Framework
    {
        class GlobalConfig
        {
        private:
            static GlobalConfig m_Instance;

        public:
            BString m_DirectX11DllPath;
            BString m_DefaultAdapterName;

        private:
            GlobalConfig()
            {
                m_DirectX11DllPath = "";
                m_DefaultAdapterName = "";
            }

        public:
            static const GlobalConfig& GetInstance() { return m_Instance; }
        };

    }
}

#endif // !__BLADE_FRAMEWORK_GLOBAL_CONFIG_H__