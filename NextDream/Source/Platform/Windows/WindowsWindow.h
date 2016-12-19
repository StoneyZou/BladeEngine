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
        bool m_isClosed;
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

        static LRESULT WINAPI WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
        {
            for (uint32 i = 0; i < m_WindowsList.Size(); ++i)
            {
                WindowsWindow* window = static_cast<WindowsWindow*>(m_WindowsList[i]);
                if (window != NULL && window->GetWindowHandle() == hWnd)
                {
                    return window->ProcessMsg(Msg, wParam, lParam);
                }
            }
            return ::DefWindowProc(hWnd, Msg, wParam, lParam);
        }

    private:
        LRESULT ProcessMsg(UINT Msg, WPARAM wParam, LPARAM lParam)
        {
            switch(Msg)
            {
            case WM_CLOSE:
                break;
            case WM_DESTROY:
                m_isClosed = true;
                PostQuitMessage(0);
                break;
            }
            LRESULT hr = ::DefWindowProc(m_windowHandle, Msg, wParam, lParam);
            return hr;
        }

    public:
        WindowsWindow(const TCHAR* inWindowName, uint32 inLeft, uint32 inTop, uint32 inWidth, uint32 inHeight, HWND inWindowHandle)
            : PlatformWindow(inWindowName, inLeft, inTop, inWidth, inHeight, false),
            m_windowHandle(inWindowHandle),
            m_isClosed(false)
        {
        }

    public:
        ~WindowsWindow()
        {
            UnregisterClass(TEXT("D3D11RenderWindow"), GetModuleHandle(0));
            CloseWindow(m_windowHandle);
            DestroyWindow(m_windowHandle);
        }

        void Update()
        {
            MSG msg = { 0 };
            while (PeekMessage(&msg, m_windowHandle, 0, 0, PM_REMOVE))
            {
                if (!TranslateAccelerator(msg.hwnd, 0, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }

        bool IsClosed() const { return m_isClosed;  }

    public:
        HWND GetWindowHandle() { return m_windowHandle; }
    };
}

#endif // !__BLADE_WINDOWS_WINDOW_H__