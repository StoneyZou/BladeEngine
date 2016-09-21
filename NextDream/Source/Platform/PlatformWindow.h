#ifndef __BLADE_PLATFORM_WINDOW_H__
#define __BLADE_PLATFORM_WINDOW_H__

#include <BString.h>
#include <Utility.h>
#include <TArray.h>

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

    public:
		static TArray<PlatformWindow*> m_WindowsList;

	private:
        TArray<EventListener*> m_ListenerList;

    protected:
        bool m_bFullScreen;

        uint32 m_Left;
        uint32 m_Top;
        uint32 m_width;
        uint32 m_height;
        //BString m_windowName;

    protected:
        PlatformWindow(const TCHAR* inWindowName, uint32 inLeft, uint32 inTop, uint32 inWidth, uint32 inHeight, bool inFullScreen)
            : m_Left(inLeft), m_Top(inTop), m_width(inWidth), m_height(inHeight),
            m_bFullScreen(inFullScreen)
        {
			m_WindowsList.Add(this);
		}

	public:
		virtual ~PlatformWindow()
		{
            for (uint32 i = 0; i < m_WindowsList.Size(); ++i)
            {
                if (m_WindowsList[i] == this)
                {
                    m_WindowsList.RemoveAt(i);
                    break;
                }
            }
		}

        virtual void Update() = 0;

        virtual bool IsClosed() const = 0;

    public:
        uint32 GetWidth() const { return m_width; };
        uint32 GetHeight() const { return m_height; }
        bool IsFullScreen() const { return m_bFullScreen; }
    }; 
}

#endif // !__BLADE_PLATFORM_WINDOW_H__

