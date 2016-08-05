#ifndef __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__
#define __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__

#include <WinBase.h>
#include <BString.h>

namespace BladeEngine
{
    namespace Windows
    {
        class WindowsSystemAPI
        {
        public:
            typedef HMODULE HModule;

        public:
            static HModule LoadBaseModule(const BString& inFileName)
            {
                return ::LoadLibrary(inFileName);
            }

            static void FreeBaseModule(HModule inHModule)
            {
                if (inHModule == 0)
                {
                    return;
                }
                ::FreeLibrary(inHModule);
            }

            static bool CheckModuleHandleValid(HModule inHModule)
            {
                return inHModule == 0;
            }

            static void* GetProcAddress(HModule inHModule, const BString& inFuncName)
            {
                return ::GetProcAddress(inHModule, inFuncName);
            }
        };
    }
}

#endif // !__BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__