#ifndef __BLADE_PLATFORM_WINDOW_H__
#define __BLADE_PLATFORM_WINDOW_H__

#include <TypeDefine.h>
#include <Utility.h>

namespace BladeEngine
{
    struct IPlatformWindowEventListener
    {
        virtual void OnWindowClose(PlatformWindow* inWindow) {};
    };

    class PlatformWindow : public INoncopyable, public IReferencable
    {
    public:
        TArray<IPlatformWindowEventListener*> m_ListenerList;

    protected:
        bool m_bFullScreen : 1;

        uint32 m_width;
        uint32 m_height;

    public:
        PlatformWindow(uint32 inWidth, uint32 inHeight, bool inFullScreen) 
            : m_width(inWidth), m_height(inHeight),
            m_bFullScreen(inFullScreen)
        {}



    public:
        uint32 GetWidth() const { return m_width; };
        uint32 GetHeight() const { return m_height; }
        bool IsFullScreen() const { return m_bFullScreen; }
    }; 
}

#endif // !__BLADE_PLATFORM_WINDOW_H__

