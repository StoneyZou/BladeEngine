#ifndef __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__
#define __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__

#include <Windows.h>
#include <TArray.h>

namespace BladeEngine
{
    #ifndef TEXT
        #ifdef _UNICODE
            #define TEXT(str) L##str
        #else
            #define TEXT(str) str
        #endif
    #endif // !TEXT

    class _ComPtrGuard
    {
    private:
        IUnknown* m_Ptr;

    public:
        _ComPtrGuard(IUnknown* inPtr) : m_Ptr(m_Ptr)
        {
        }

        ~_ComPtrGuard()
        {
            if (m_Ptr != NULL)
            {
                m_Ptr->Release();
            }
        }
    };

    #define ComPtrGuard(ptr) _ComPtrGuard ptr##Guard(ptr)

    class WindowsSystemAPI
    {
    public:
        typedef HMODULE HModule;

    public:
        static HModule LoadBaseModule(const TCHAR* inFileName)
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

        static void* GetProcAddress(HModule inHModule, const TCHAR* inFuncName)
        {
            return ::GetProcAddress(inHModule, inFuncName);
        }

        static const ANSICHAR* WideCahrToAnsiChar(const WIDECHAR* inSrc)
        {
            static TArray<TCHAR> tempStr;

            int32 length = WideCharToMultiByte(CP_ACP, 0, inSrc, wcslen(inSrc), NULL, 0, NULL, false);
            if (length == 0)
            {
                return "";
            }

            tempStr.Resize(length + 1);
            length = WideCharToMultiByte(CP_ACP, 0, inSrc, wcslen(inSrc), tempStr.TypePtr(), tempStr.GetLength(), NULL, false);
            if (length == 0)
            {
                return "";
            }

            return tempStr.TypePtr();
        }

        static const WIDECHAR* AnsiCahrToWideChar(const ANSICHAR* inSrc)
        {
            static TArray<WIDECHAR> tempStr;

            int32 length = MultiByteToWideChar(CP_ACP, 0, inSrc, strlen(inSrc), NULL, 0);
            if (length == 0)
            {
                return L"";
            }

            tempStr.Resize(length + 1);
            length = MultiByteToWideChar(CP_ACP, 0, inSrc, strlen(inSrc), tempStr.TypePtr(), tempStr.GetLength());
            if (length == 0)
            {
                return L"";
            }

            return tempStr.TypePtr();
        }
    };
}

#endif // !__BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__