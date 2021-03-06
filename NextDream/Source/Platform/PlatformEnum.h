#ifndef __BLADE_PLATFORM_PLATFORM_ENUM_H__
#define __BLADE_PLATFORM_PLATFORM_ENUM_H__

namespace BladeEngine
{
    enum EFILE_ACCESS_MODE
    {
        EFILE_READ = 0x0001,
        EFILE_WRITE = 0x0002,
    };

    enum EFILE_SHARE_MODE
    {
        EFILE_SHARE_READ = 0x0001,
        EFILE_SHARE_WRITE = 0x0002,
        EFILE_SHARE_DELETE = 0x0004,
        EFILE_SHARE_READ_WRITE = EFILE_SHARE_READ | EFILE_SHARE_WRITE,
    };

    enum EFILE_OPEN_MODE
    {
        EFIlE_CREATE,
        EFIlE_CREATE_NEW,
        EFILE_OPEN_EXISTING,
    };
}

#endif // !__BLADE_PLATFORM_PLATFORM_ENUM_H__
