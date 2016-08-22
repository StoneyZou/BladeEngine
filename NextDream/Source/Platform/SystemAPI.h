#ifndef __BLADE_GENERAL_SYSTEM_API_H__
#define __BLADE_GENERAL_SYSTEM_API_H__

#define WINDOWS_PLATFORM 1

#if defined WINDOWS_PLATFORM
    #include <WindowsAPI.h>
#endif

namespace BladeEngine
{
	class SystemAPI
    #if defined WINDOWS_PLATFORM
		: public WindowsAPI
    #endif
    {
    };

    typedef SystemAPI::ModuleHandle ModuleHandle;
    typedef SystemAPI::FileHandle FileHandle;
}

#endif // !__BLADE_GENERAL_SYSTEM_API_H__