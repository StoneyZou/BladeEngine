#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <TypeDefine.h>
#include <RHIDevice.h>
#include <RHITextureBase.h>

namespace BladeEngine
{
    struct OpenGLWindowsContext
    {
        HWND    DummyWnd;
        HDC     DummyDC;
        HGLRC   Context;
        GLuint  FrameBuffer;

        bool MakeCurrentContext()
        {
            return wglMakeCurrent(DummyDC, Context) == TRUE;
        }
    };

    struct OpenGLWindowsDevice
    {
        OpenGLWindowsContext m_RenderingContext;
        OpenGLWindowsContext m_SharedContext;
    };

    OpenGLWindowsDevice* CreateDevice()
    {
        if (!InitOpenGLLibrary(3, 3))
        {
            return NULL;
        }

        OpenGLWindowsDevice* device = new OpenGLWindowsDevice();
        if (CreateDummyOpenGLContext(&device->m_RenderingContext, 3, 3))
        {
            return NULL;
        }

        if (CreateDummyOpenGLContext(&device->m_SharedContext, 3, 3))
        {
            return NULL;
        }

        return device;
    }

    void ReleaseOpenGLWindowsDevice(OpenGLWindowsDevice* inDevice)
    {
        ReleaseDummyOpenGLContext(&inDevice->m_RenderingContext);
        delete inDevice;
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
   
    bool CreateDummyOpenGLContext(OpenGLWindowsContext* inContext, int MajorVersion, int MinorVersion)
    {
        HWND dummyWnd = CreateDummyWindow();
        if (dummyWnd == NULL)
        {
            return NULL;
        }

        HDC hDc = GetDC(dummyWnd);
        if (hDc == NULL || InitPixelFormatForDC(hDc))
        {
            CloseWindow(dummyWnd);
            return NULL;
        }

        HGLRC hGLRC = CreateOpenGLContext(hDc, 3, 3);
        if (hGLRC != NULL)
        {
            CloseWindow(dummyWnd);
            return NULL;
        }

        glGenFramebuffers(1, &inContext->FrameBuffer);

        inContext->DummyWnd = dummyWnd;
        inContext->DummyDC = hDc;
        inContext->Context = hGLRC;
    }

    void ReleaseDummyOpenGLContext(OpenGLWindowsContext* inContext)
    {
        if (wglGetCurrentDC() == inContext->DummyDC || wglGetCurrentContext() == inContext->Context)
        {
            wglMakeCurrent(NULL, NULL);
        }

        if (inContext->Context != NULL) { wglDeleteContext(inContext->Context); }
        if (inContext->DummyDC != NULL) { ReleaseDC(inContext->DummyWnd, inContext->DummyDC); }
        if (inContext->DummyWnd != NULL) { CloseWindow(inContext->DummyWnd); }
        if (inContext->FrameBuffer != 0) { glDeleteFramebuffers(1, &inContext->FrameBuffer); }

        inContext->DummyWnd = NULL;
        inContext->DummyDC = NULL;
        inContext->Context = NULL;
        inContext->FrameBuffer = 0;
    }

    bool MakeCurrentOpenGLContext(OpenGLWindowsContext* inContext)
    {
        if (inContext == NULL)
        {
            return wglMakeCurrent(NULL, NULL) == TRUE;
        }
        return wglMakeCurrent(inContext->DummyDC, inContext->Context) == TRUE;
    }

    bool InitOpenGLLibrary(int MajorVersion, int MinorVersion)
    {
        static bool bOpenGLLibraryInitialized = false;
        if (bOpenGLLibraryInitialized)
        {
            return true;
        }

        bool result = false;
        HWND dummyWnd = CreateDummyWindow();
        if (dummyWnd != NULL)
        {
            HDC hDC = GetDC(dummyWnd);
            if (hDC != NULL && InitPixelFormatForDC(hDC))
            {
                HGLRC hGLRC = CreateOpenGLContext(hDC, MajorVersion, MinorVersion);
                if (hGLRC != NULL)
                {
                    GLenum result = glewInit();
                    if (result == GLEW_OK)
                    {
                        result = true;
                        bOpenGLLibraryInitialized = true;
                    }
                    wglDeleteContext(hGLRC);
                }
                ReleaseDC(dummyWnd, hDC);
            }
            CloseWindow(dummyWnd);
        }

        return result;
    }

    void CreateTexture2D(RHI::RHITextureCreateInfo info)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.Width, info.Height, 0, GL_RGBA, GL_FLOAT, info.Data);
        //glTextureRenderbufferEXT
    }

    bool GetRenderBufferImage(GLuint renderBuffer, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels)
    {
        glBindFramebuffer();
    }
}