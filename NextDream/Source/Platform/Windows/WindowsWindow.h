#ifndef __BLADE_WINDOWS_WINDOW_H__
#define __BLADE_WINDOWS_WINDOW_H__

#include <WindowsPCH.h>
#include <PlatformWindow.h>

namespace BladeEngine
{
    class WindowsWindow;
    typedef RefCountObject<WindowsWindow> WindowsWindowRef;

    class WindowsWindow : public PlatformWindow
    {
    private:
        HWND m_windowHandle;

    public:
        static WindowsWindowRef Create(const TCHAR* inWindowName, uint32 inLeft, uint32 inTop, uint32 inWidth, uint32 inHeight)
        {
            // 获取当前进程的实例句柄
            HINSTANCE hInstance = GetModuleHandle(0);

            WNDCLASS wndClass = { 0 };
            wndClass.cbClsExtra = 0;	// 无class扩展
            wndClass.cbWndExtra = 0;	// 无窗口扩展
            wndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));		// 取得白色画刷
            wndClass.hCursor = LoadCursor(nullptr, IDI_APPLICATION);			// 取应用程序默认光标
            wndClass.hIcon = LoadIcon(nullptr, IDC_ICON);						// 取默认图标
            wndClass.hInstance = hInstance;
            wndClass.lpfnWndProc = ::DefWindowProc;
            wndClass.lpszClassName = "D3D11RenderWindow";
            wndClass.lpszMenuName = "";
            wndClass.style = 0;		// 默认格式
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

    public:
        WindowsWindow(const TCHAR* inWindowName, uint32 inLeft, uint32 inTop, uint32 inWidth, uint32 inHeight, HWND inWindowHandle)
            : PlatformWindow(inWindowName, inLeft, inTop, inWidth, inHeight, false),
            m_windowHandle(inWindowHandle)
        {
        }

    public:
        ~WindowsWindow()
        {
            CloseWindow(m_windowHandle);
        }

    public:
        HWND GetWindowHandle() { return m_windowHandle; }
    };
}

#endif // !__BLADE_WINDOWS_WINDOW_H__