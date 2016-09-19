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
            BString DirectX11DllPath;
            BString DefaultAdapterName;

        private:
            GlobalConfig()
            {
                //DirectX11DllPath = "";
                DefaultAdapterName = "";
            }

        public:
            static GlobalConfig& GetInstance() { return m_Instance; }
        };

    }
}

#endif // !__BLADE_FRAMEWORK_GLOBAL_CONFIG_H__