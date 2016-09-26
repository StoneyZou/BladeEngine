#ifndef __BLADE_OPENGL_WINDOWS_H__
#define __BLADE_OPENGL_WINDOWS_H__

namespace BladeEngine
{
    struct OpenGLWindowsContext;
    struct OpenGLWindowsDevice;

    struct OpenGLWindowsDeviceUtil
    {
        // Device
        OpenGLWindowsDevice* CreateDevice();
        void ReleaseDevice(OpenGLWindowsDevice* inDevice);
    };
}

#endif // !__BLADE_OPENGL_WINDOWS_H__
