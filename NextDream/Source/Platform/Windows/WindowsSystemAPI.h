#ifndef __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__
#define __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__

#include <Windows.h>
#include <TArray.h>
#include <EnumDefine.h>

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
    };

    enum EFILE_OPEN_MODE
    {
        EFIlE_CREATE,
        EFIlE_CREATE_NEW,
        EFILE_OPEN_EXISTING,
    };

    #define ComPtrGuard(ptr) _ComPtrGuard ptr##Guard(ptr)

    class WindowsSystemAPI
    {
    public:
        typedef HMODULE HModule;
        typedef HANDLE HFile;

    public:
        static HModule LoadBaseModule(const TCHAR* inFileName)
        {
            return ::LoadLibrary(inFileName);
        }

        static void FreeBaseModule(HModule inHModule)
        {
            if (!CheckModuleHandleValid(inHModule))
            {
                return;
            }
            ::FreeLibrary(inHModule);
        }

        static bool CheckModuleHandleValid(HModule inHModule)
        {
            return inHModule != 0;
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

        static const HFile OpenFile(const TCHAR* inFileName, EFILE_ACCESS_MODE inAccessMode, EFILE_SHARE_MODE inShareMode, EFILE_OPEN_MODE inOpenMode)
        {
            DWORD accessMode =
                (inAccessMode & EFILE_READ) != 0 ? GENERIC_READ : 0 |
                (inAccessMode & EFILE_WRITE) != 0 ? GENERIC_WRITE : 0;

            DWORD shareMode =
                (inShareMode & EFILE_SHARE_READ) != 0 ? FILE_SHARE_READ : 0 |
                (inShareMode & EFILE_SHARE_WRITE) != 0 ? FILE_SHARE_WRITE : 0 |
                (inShareMode & EFILE_SHARE_DELETE) != 0 ? FILE_SHARE_DELETE : 0;

            DWORD openMode = EFILE_OPEN_EXISTING;
            switch (inOpenMode)
            {
            case BladeEngine::EFIlE_CREATE:
                openMode = OPEN_ALWAYS;
                break;
            case BladeEngine::EFIlE_CREATE_NEW:
                openMode = CREATE_NEW;
                break;
            case BladeEngine::EFILE_OPEN_EXISTING:
                openMode = OPEN_EXISTING;
                break;
            }

            return CreateFile(inFileName, accessMode, shareMode, NULL, openMode, FILE_ATTRIBUTE_NORMAL, 0);
        }

        static void CloseFile(HFile inHFile)
        {
            CloseHandle(inHFile);
        }

        static bool CheckFileHandleValid(HFile inHFile)
        {
            return inHFile != 0;
        }

        static int32 ReadFile(HFile inFile, byte* inBuf, uint32 inBufSize)
        {
            DWORD readSize = 0;
            if (!::ReadFile(inFile, inBuf, inBufSize, &readSize, NULL))
            {
                return -1;
            }
            return readSize;
        }

        static int32 WriteFile(HFile inFile, const byte* inBuf, uint32 inBufSize)
        {
            DWORD writeSize = 0;
            if (!::WriteFile(inFile, inBuf, inBufSize, &writeSize, NULL))
            {
                return -1;
            }
            return writeSize;
        }

        static int64 SeekFile(HFile inFile, uint64 inOffset, ESEEK_POS inSeekPos)
        {
            DWORD seekPos = FILE_BEGIN;
            switch (inSeekPos)
            {
            case BladeEngine::ESEEK_POS_BEGIN:
                seekPos = FILE_BEGIN;
                break;
            case BladeEngine::ESEEK_POS_CUR:
                seekPos = FILE_CURRENT;
                break;
            case BladeEngine::ESEEK_POS_END:
                seekPos = FILE_END;
                break;
            default:
                break;
            }

            LONG low32Bits = *reinterpret_cast<uint32*>((byte*)&inOffset);
            LONG high32Bits = *reinterpret_cast<uint32*>(((byte*)&inOffset) + 32);

            low32Bits = SetFilePointer(inFile, low32Bits, &high32Bits, seekPos);

            uint64 result = low32Bits + ((uint64)high32Bits << 32);
            return result;
        }
        
        static int32 GetFileSize(HFile inFile)
        {
            DWORD low32Bits = 0;
            DWORD high32Bits = 0;

            low32Bits =::GetFileSize(inFile, &high32Bits);

            uint64 result = low32Bits + ((uint64)high32Bits << 32);
            return result;
        }
    };
}

#endif // !__BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__