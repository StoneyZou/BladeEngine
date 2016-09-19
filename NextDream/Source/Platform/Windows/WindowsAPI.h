#ifndef __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__
#define __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__

#include <TypeDefine.h>
#include <EnumDefine.h>
#include <PlatformWindow.h>
#include <PlatformEnum.h>

namespace BladeEngine
{
    #define DeclareHandle(name) struct name##__{int unused;}; typedef struct name##__ *name;

    class WindowsAPI
    {
    public:
        DeclareHandle(HModule);
        DeclareHandle(HFile);

    public:
        static HModule LoadBaseModule(const TCHAR* inFileHandleName);
        static void FreeBaseModule(HModule inHModule);
        static bool CheckHModuleValid(HModule inHModule);
        static void* GetProcAddress(HModule inHModule, const TCHAR* inFuncName);

        static const ANSICHAR* WideCahrToAnsiChar(const WIDECHAR* inSrc);
        static const WIDECHAR* AnsiCahrToWideChar(const ANSICHAR* inSrc);

        static const HFile OpenFile(const TCHAR* inFileName, EFILE_ACCESS_MODE inAccessMode, EFILE_SHARE_MODE inShareMode, EFILE_OPEN_MODE inOpenMode);
        static const HFile OpenFileA(const ANSICHAR* inFileName, EFILE_ACCESS_MODE inAccessMode, EFILE_SHARE_MODE inShareMode, EFILE_OPEN_MODE inOpenMode);
        static const HFile OpenFileW(const WIDECHAR* inFileName, EFILE_ACCESS_MODE inAccessMode, EFILE_SHARE_MODE inShareMode, EFILE_OPEN_MODE inOpenMode);
        static void CloseFile(HFile inFileHandle);
        static bool CheckHFileValid(HFile inFileHandle);
        static SIZE_T ReadFile(HFile inFileHandle, byte* inBuf, uint32 inBufSize);
        static SIZE_T WriteFile(HFile inFileHandle, const byte* inBuf, uint32 inBufSize);
        static SIZE_T SeekFile(HFile inFileHandle, SIZE_T inOffset, ESEEK_POS inSeekPos);
        static SIZE_T GetFileSize(HFile inFileHandle);

        static PlatformWindowRef CreatePlatformWindow(const TCHAR* inWindowName, uint32 inLeft, uint32 inTop, uint32 inWidth, uint32 inHeight);

        static uint32 PrintToConsole(const TCHAR* inBuffer);
        static uint32 PrintToConsole(const TCHAR* inBuffer, SIZE_T inBufferSize);
    };
}

#endif // !__BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__