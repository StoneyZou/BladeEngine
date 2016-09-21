#include <Windows.h>
#include <WindowsAPI.h>
#include <WindowsPCH.h>
#include <TArray.h>
#include <StringUtil.h>
#include <WindowsWindow.h>

namespace BladeEngine
{
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

    WindowsAPI::HModule WindowsAPI::LoadBaseModule(const TCHAR* inHFileName)
    {
        return (WindowsAPI::HModule)::LoadLibrary(inHFileName);
    }

    void WindowsAPI::FreeBaseModule(WindowsAPI::HModule inHModule)
    {
        if (!CheckHModuleValid(inHModule))
        {
            return;
        }
        ::FreeLibrary((::HMODULE)inHModule);
    }

    bool WindowsAPI::CheckHModuleValid(WindowsAPI::HModule inHModule)
    {
        return inHModule != NULL;
    }

    void* WindowsAPI::GetProcAddress(HModule inHModule, const TCHAR* inFuncName)
    {
        return ::GetProcAddress((::HMODULE)inHModule, inFuncName);
    }

    const ANSICHAR* WindowsAPI::WideCahrToAnsiChar(const WIDECHAR* inSrc)
    {
        TArray<TCHAR> tempStr;

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

    const WIDECHAR* WindowsAPI::AnsiCahrToWideChar(const ANSICHAR* inSrc)
    {
        TArray<WIDECHAR> tempStr;

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

    const WindowsAPI::HFile WindowsAPI::OpenFile(const TCHAR* inFileName, EFILE_ACCESS_MODE inAccessMode, EFILE_SHARE_MODE inShareMode, EFILE_OPEN_MODE inOpenMode)
    {
        return OpenFileA(inFileName, inAccessMode, inShareMode, inOpenMode);
    }

    const WindowsAPI::HFile WindowsAPI::OpenFileA(const ANSICHAR* inFileName, EFILE_ACCESS_MODE inAccessMode, EFILE_SHARE_MODE inShareMode, EFILE_OPEN_MODE inOpenMode)
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

        return (WindowsAPI::HFile)CreateFileA(inFileName, accessMode, shareMode, NULL, openMode, FILE_ATTRIBUTE_NORMAL, 0);
    }

    const WindowsAPI::HFile WindowsAPI::OpenFileW(const WIDECHAR* inFileName, EFILE_ACCESS_MODE inAccessMode, EFILE_SHARE_MODE inShareMode, EFILE_OPEN_MODE inOpenMode)
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

        return (WindowsAPI::HFile)CreateFileW(inFileName, accessMode, shareMode, NULL, openMode, FILE_ATTRIBUTE_NORMAL, 0);
    }

    void WindowsAPI::CloseFile(HFile inHFile)
    {
        CloseHandle(inHFile);
    }

    bool WindowsAPI::CheckHFileValid(HFile inHFile)
    {
        return inHFile != 0;
    }

    SIZE_T WindowsAPI::ReadFile(HFile inHFile, byte* inBuf, uint32 inBufSize)
    {
        DWORD readSize = 0;
        if (!::ReadFile(inHFile, inBuf, inBufSize, &readSize, NULL))
        {
            return -1;
        }
        return readSize;
    }

    SIZE_T WindowsAPI::WriteFile(HFile inHFile, const byte* inBuf, uint32 inBufSize)
    {
        DWORD writeSize = 0;
        if (!::WriteFile(inHFile, inBuf, inBufSize, &writeSize, NULL))
        {
            return -1;
        }
        return writeSize;
    }

    SIZE_T WindowsAPI::SeekFile(HFile inHFile, SIZE_T inOffset, ESEEK_POS inSeekPos)
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

        low32Bits = SetFilePointer(inHFile, low32Bits, &high32Bits, seekPos);

        SIZE_T result = low32Bits;// +((SIZE_T)high32Bits << 32);
        return result;
    }

    SIZE_T WindowsAPI::GetFileSize(HFile inHFile)
    {
        DWORD low32Bits = 0;
        DWORD high32Bits = 0;

        low32Bits = ::GetFileSize(inHFile, &high32Bits);

        SIZE_T result = low32Bits;// +((SIZE_T)high32Bits << 32);
        return result;
    }

    PlatformWindowRef WindowsAPI::CreatePlatformWindow(const TCHAR* inWindowName, uint32 inLeft, uint32 inTop, uint32 inWidth, uint32 inHeight)
    {
        // 获取当前进程的实例句柄
        HINSTANCE hInstance = ::GetModuleHandle(0);

        WNDCLASS wndClass = { 0 };
        wndClass.cbClsExtra = 0;                                            // 无class扩展
        wndClass.cbWndExtra = 0;                                            // 无窗口扩展
        wndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	    // 取得白色画刷
        wndClass.hCursor = LoadCursor(nullptr, IDI_APPLICATION);            // 取应用程序默认光标
        wndClass.hIcon = LoadIcon(nullptr, IDC_ICON);                       // 取默认图标
        wndClass.hInstance = hInstance;
        wndClass.lpfnWndProc = WindowsWindow::WindowProc;
        wndClass.lpszClassName = "D3D11RenderWindow";
        wndClass.lpszMenuName = "";
        wndClass.style = 0;                                                 // 默认格式

                                                                            // 注册窗口类
        RegisterClass(&wndClass);

        HWND hWnd = CreateWindow("D3D11RenderWindow", inWindowName, WS_VISIBLE | WS_OVERLAPPEDWINDOW,
            inLeft, inTop, inWidth, inHeight, nullptr, nullptr, hInstance, 0);

        if (hWnd == 0)
        {
            return WindowsWindowRef();
        }

        return WindowsWindowRef(new WindowsWindow(inWindowName, inLeft, inTop, inWidth, inHeight, hWnd));
    }

    uint32 WindowsAPI::PrintToConsole(const TCHAR* inBuffer)
    {
        return PrintToConsole(inBuffer, StringUtil::Strlen(inBuffer));
    }

    uint32 WindowsAPI::PrintToConsole(const TCHAR* inBuffer, SIZE_T inBufferSize)
    {
        bool hasAllocConsole = false;
        HANDLE stdOutHandle = NULL;

        if (!hasAllocConsole)
        {
            hasAllocConsole = (AllocConsole() == TRUE);
        }

        if (hasAllocConsole && stdOutHandle == NULL)
        {
            stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        DWORD writeSize = 0;
        if (!WriteConsole(stdOutHandle, inBuffer, inBufferSize, &writeSize, NULL))
        {
            return -1;
        }
        return writeSize;
    }
}