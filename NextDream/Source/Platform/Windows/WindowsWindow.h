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
        static WindowsWindowRef Create(const TCHAR* inWindowName, uint32 inWidth, uint32 inHeight)
        {
            // ��ȡ��ǰ���̵�ʵ�����
            HINSTANCE hInstance = GetModuleHandle(0);

            WNDCLASS wndClass = { 0 };
            wndClass.cbClsExtra = 0;	// ��class��չ
            wndClass.cbWndExtra = 0;	// �޴�����չ
            wndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));		// ȡ�ð�ɫ��ˢ
            wndClass.hCursor = LoadCursor(nullptr, IDI_APPLICATION);			// ȡӦ�ó���Ĭ�Ϲ��
            wndClass.hIcon = LoadIcon(nullptr, IDC_ICON);						// ȡĬ��ͼ��
            wndClass.hInstance = hInstance;
            wndClass.lpfnWndProc = ::DefWindowProc;
            wndClass.lpszClassName = "D3D11RenderWindow";
            wndClass.lpszMenuName = "";
            wndClass.style = 0;		// Ĭ�ϸ�ʽ
                                    // ע�ᴰ����
            RegisterClass(&wndClass);

            HWND hWnd = CreateWindow("D3D11RenderWindow", inWindowName, WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                50, 50, inWidth, inHeight, nullptr, nullptr, hInstance, 0);

            if (hWnd == 0)
            {
                return WindowsWindowRef();
            }

            return WindowsWindowRef(new WindowsWindow(hWnd, inWidth, inHeight));
        }

    public:
        WindowsWindow(HWND inWindowHandle, uint32 inWidth, uint32 inHeight)
            : PlatformWindow("", inWidth, inHeight, false),
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