#include <Windows.h>
#include <TypeDefine.h>
#include <GL/glew.h>
#include <GL/wglew.h>

namespace BladeEngine
{
    struct OpenGLWindowsContext
    {
        HWND    DummyWnd;
        HDC     DummyDC;
        HGLRC   OpenGLContext;

        bool MakeCurrentContext()
        {
            return wglMakeCurrent(DummyDC, OpenGLContext) == TRUE;
        }

        void ReleaseContext()
        {
            if( wglGetCurrentDC() == 
            if (DummyWnd != NULL) { CloseWindow(DummyWnd); }
            if (DummyDC != NULL) { CloseWindow(DummyDC); }
            if (DummyDC != NULL) { CloseWindow(DummyDC); }
        }
    };

    struct OpenGLWindowsDevice
    {
        OpenGLWindowsContext m_RenderingContext;
    };

    OpenGLWindowsDevice* CreateOpenGLWindowsDevice()
    {
        HWND dummyWnd = CreateDummyWindow();
        if( dummyWnd == NULL)
        {
            return NULL;
        }

        HDC hDc = GetDC(dummyWnd);
        if (hDc == NULL)
        {
            return NULL;
        }

        if (InitPixelFormatForDC(hDc))
        {
            return NULL;
        }

        HGLRC hGLRC = CreateOpenGLContext(hDc, 3, 3);
        if (hGLRC != NULL)
        {
            return NULL;
        }


    }

    HWND CreateDummyWindow()
    {
        const TCHAR* DummyWindow = TEXT("__DummyWindow");
        static bool bCreateDummyWindowClass = false;

        if (!bCreateDummyWindowClass)
        {
            WNDCLASS wndClass = { 0 };
            wndClass.style = CS_OWNDC;
            wndClass.lpfnWndProc = ::DefWindowProc;
            wndClass.cbClsExtra = 0;
            wndClass.cbWndExtra = 0;
            wndClass.hInstance = NULL;
            wndClass.hIcon = NULL;
            wndClass.hCursor = NULL;
            wndClass.hbrBackground = (HBRUSH)(COLOR_MENUTEXT);
            wndClass.lpszMenuName = NULL;
            wndClass.lpszClassName = DummyWindow;

            if (::RegisterClass(&wndClass))
            {
                return NULL;
            }
        }

        HWND hwnd = ::CreateWindow(DummyWindow, NULL, WS_POPUP, 0, 0, 1, 1, NULL, NULL, NULL, NULL);
        return hwnd;
    }

    static bool InitPixelFormatForDC(HDC hDC)
    {
        // Pixel format descriptor for the context.
        PIXELFORMATDESCRIPTOR PixelFormatDesc;
        PixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        PixelFormatDesc.nVersion = 1;
        PixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        PixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
        PixelFormatDesc.cColorBits = 32;
        PixelFormatDesc.cDepthBits = 0;
        PixelFormatDesc.cStencilBits = 0;
        PixelFormatDesc.iLayerType = PFD_MAIN_PLANE;

        // Set the pixel format and create the context.
        int32 PixelFormat = ChoosePixelFormat(hDC, &PixelFormatDesc);
        if (PixelFormat == 0 || !SetPixelFormat(hDC, PixelFormat, &PixelFormatDesc))
        {
            return false;
        }
        return true;
    }

    HGLRC CreateOpenGLContext(HDC hDc, int MajorVersion, int MinorVersion)
    {
        int AttribList[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, MajorVersion,
            WGL_CONTEXT_MINOR_VERSION_ARB, MinorVersion,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        return wglCreateContextAttribsARB(hDc, NULL, AttribList);
    }
}