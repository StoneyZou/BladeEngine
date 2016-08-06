#ifndef __BLADE_GENERAL_SYSTEM_API_H__
#define __BLADE_GENERAL_SYSTEM_API_H__

#define WINDOWS_PLATFORM 1

#if defined WINDOWS_PLATFORM
    #include <WindowsSystemAPI.h>
#endif

namespace BladeEngine
{
    #if defined WINDOWS_PLATFORM
    class SystemAPI : public WindowsSystemAPI
    #endif
    {
    };

    typedef SystemAPI::HModule HModule;
}

#endif // !__BLADE_GENERAL_SYSTEM_API_H__