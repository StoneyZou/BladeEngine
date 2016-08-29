#ifndef __BLADE_GENERAL_SYSTEM_API_H__
#define __BLADE_GENERAL_SYSTEM_API_H__

#include <MacroDefine.h>
#if defined WINDOWS_PLATFORM
    #include <WindowsAPI.h>
#endif

namespace BladeEngine
{
	class PlatformAPI
    #if defined WINDOWS_PLATFORM
		: public WindowsAPI
    #endif
    {
    };

    typedef PlatformAPI::ModuleHandle ModuleHandle;
    typedef PlatformAPI::FileHandle FileHandle;
}

#endif // !__BLADE_GENERAL_SYSTEM_API_H__