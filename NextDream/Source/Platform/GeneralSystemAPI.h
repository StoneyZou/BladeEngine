#ifndef __BLADE_GENERAL_SYSTEM_API_H__
#define __BLADE_GENERAL_SYSTEM_API_H__

#define WINDOWS_PLATFORM 1

#if defined WINDOWS_PLATFORM
    #include <WindowsAPI.h>
#endif

namespace BladeEngine
{
    #if defined WINDOWS_PLATFORM
    class SystemAPI : public WindowsAPI
    #endif
    {
    };

    typedef SystemAPI::ModuleHandle ModuleHandle;
    typedef SystemAPI::FileHandle FileHandle;
    typedef SystemAPI::WindowHandle WindowHandle;
}

#endif // !__BLADE_GENERAL_SYSTEM_API_H__