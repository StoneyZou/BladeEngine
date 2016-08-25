#ifndef __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__
#define __BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__

#include <WindowsPCH.h>
#include <TArray.h>
#include <EnumDefine.h>
#include <WindowsWindow.h>

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

    class WindowsAPI
    {
    public:
        typedef HMODULE ModuleHandle;
        typedef HANDLE FileHandle;
        typedef HWND WindowHandle;

    public:
        static ModuleHandle LoadBaseModule(const TCHAR* inFileHandleName)
        {
            return ::LoadLibrary(inFileHandleName);
        }

        static void FreeBaseModule(ModuleHandle inHModule)
        {
            if (!CheckModuleHandleValid(inHModule))
            {
                return;
            }
            ::FreeLibrary(inHModule);
        }

        static bool CheckModuleHandleValid(ModuleHandle inHModule)
        {
            return inHModule != 0;
        }

        static void* GetProcAddress(ModuleHandle inHModule, const TCHAR* inFuncName)
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
            length = WideCharToMultiByte(CP_ACP, 0, inSrc, wcslen(inSrc), tempStr.TypePtr(), tempStr.Size(), NULL, false);
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
            length = MultiByteToWideChar(CP_ACP, 0, inSrc, strlen(inSrc), tempStr.TypePtr(), tempStr.Size());
            if (length == 0)
            {
                return L"";
            }

            return tempStr.TypePtr();
        }

        static const FileHandle OpenFile(const TCHAR* inFileName, EFILE_ACCESS_MODE inAccessMode, EFILE_SHARE_MODE inShareMode, EFILE_OPEN_MODE inOpenMode)
        {
            DWORD accessMode =
                ((inAccessMode & EFILE_READ) != 0 ? GENERIC_READ : 0) |
                ((inAccessMode & EFILE_WRITE) != 0 ? GENERIC_WRITE : 0);

            DWORD shareMode =
                ((inShareMode & EFILE_SHARE_READ) != 0 ? FILE_SHARE_READ : 0) |
                ((inShareMode & EFILE_SHARE_WRITE) != 0 ? FILE_SHARE_WRITE : 0) |
                ((inShareMode & EFILE_SHARE_DELETE) != 0 ? FILE_SHARE_DELETE : 0);

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

        static void CloseFile(FileHandle inFileHandle)
        {
            CloseHandle(inFileHandle);
        }

        static bool CheckFileHandleValid(FileHandle inFileHandle)
        {
            return inFileHandle != 0;
        }

        static int32 ReadFile(FileHandle inFileHandle, byte* inBuf, uint32 inBufSize)
        {
            DWORD readSize = 0;
            if (!::ReadFile(inFileHandle, inBuf, inBufSize, &readSize, NULL))
            {
                return -1;
            }
            return readSize;
        }

        static int32 WriteFile(FileHandle inFileHandle, const byte* inBuf, uint32 inBufSize)
        {
            DWORD writeSize = 0;
            if (!::WriteFile(inFileHandle, inBuf, inBufSize, &writeSize, NULL))
            {
                return -1;
            }
            return writeSize;
        }

        static int64 SeekFile(FileHandle inFileHandle, uint64 inOffset, ESEEK_POS inSeekPos)
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

            low32Bits = SetFilePointer(inFileHandle, low32Bits, &high32Bits, seekPos);

            uint64 result = low32Bits + ((uint64)high32Bits << 32);
            return result;
        }
        
        static uint64 GetFileSize(FileHandle inFileHandle)
        {
            DWORD low32Bits = 0;
            DWORD high32Bits = 0;

            low32Bits =::GetFileSize(inFileHandle, &high32Bits);

            uint64 result = low32Bits + ((uint64)high32Bits << 32);
            return result;
        }

        static WindowsWindowRef CreatePlatformWindow(const TCHAR* inWindowName, uint32 inLeft, uint32 inTop, uint32 inWidth, uint32 inHeight)
        {
            // ��ȡ��ǰ���̵�ʵ�����
            HINSTANCE hInstance = GetModuleHandle(0);

            WNDCLASS wndClass = { 0 };
            wndClass.cbClsExtra = 0;                                            // ��class��չ
            wndClass.cbWndExtra = 0;                                            // �޴�����չ
            wndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	    // ȡ�ð�ɫ��ˢ
            wndClass.hCursor = LoadCursor(nullptr, IDI_APPLICATION);            // ȡӦ�ó���Ĭ�Ϲ��
            wndClass.hIcon = LoadIcon(nullptr, IDC_ICON);                       // ȡĬ��ͼ��
            wndClass.hInstance = hInstance;
            wndClass.lpfnWndProc = WindowsWindow::WindowProc;
            wndClass.lpszClassName = "D3D11RenderWindow";
            wndClass.lpszMenuName = "";
            wndClass.style = 0;                                                 // Ĭ�ϸ�ʽ
            
            // ע�ᴰ����
            RegisterClass(&wndClass);

            HWND hWnd = CreateWindow("D3D11RenderWindow", inWindowName, WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                inLeft, inTop, inWidth, inHeight, nullptr, nullptr, hInstance, 0);

            if (hWnd == 0)
            {
                return WindowsWindowRef();
            }

            return WindowsWindowRef(new WindowsWindow(inWindowName, inLeft, inTop, inWidth, inHeight, hWnd));
        }

        static void ClosePlatformWindow(WindowHandle inWindowHandle)
        {
            CloseWindow(inWindowHandle);
        }
    };
}

#endif // !__BLADE_PLATFORM_WINDOWS_SYSTEM_API_H__