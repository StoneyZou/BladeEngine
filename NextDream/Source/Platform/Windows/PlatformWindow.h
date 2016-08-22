#ifndef __BLADE_PLATFORM_WINDOW_H__
#define __BLADE_PLATFORM_WINDOW_H__

#include <TypeDefine.h>
#include <Utility.h>
#include <BString.h>

namespace BladeEngine
{
	class PlatformWindow;
	typedef RefCountObject<PlatformWindow> PlatformWindowRef;

    class PlatformWindow : public INoncopyable, public IReferencable
    {
	public:
		struct EventListener
		{
			virtual void OnWindowClose(PlatformWindow* inWindow) {};
		};

	private:
		static TArray<PlatformWindow*> m_WindowsList;

	private:
        TArray<EventListener*> m_ListenerList;

    protected:
        bool m_bFullScreen : 1;

        uint32 m_width;
        uint32 m_height;
		BString m_windowName;

    protected:
        PlatformWindow(const BString& inWindowName, uint32 inWidth, uint32 inHeight, bool inFullScreen) 
            : m_width(inWidth), m_height(inHeight), m_windowName(inWindowName)
            m_bFullScreen(inFullScreen)
        {
			m_WindowsList.Add(this);
		}

	public:
		virtual ~PlatformWindow()
		{
			m_WindowsList.Remove(this);
		}

    public:
        uint32 GetWidth() const { return m_width; };
        uint32 GetHeight() const { return m_height; }
        bool IsFullScreen() const { return m_bFullScreen; }
    }; 
}

#endif // !__BLADE_PLATFORM_WINDOW_H__

