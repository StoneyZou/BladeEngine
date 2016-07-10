#ifndef __BLADE_CONTAINS_STRING_H__
#define __BLADE_CONTAINS_STRING_H__

#include <CharDefine.h>
#include <TypeDefine.h>
#include <StringUtil.h>
#include <Memory.h>

namespace BladeEngine
{
    class String
    {
    private:
        SIZE_T m_Length;
        TCHAR* m_Buffer;

    public:
        /**
        * @param inStr      An array of character encoding with ansi/unicode
        */
        String(const TCHAR* inStr)
        {
            m_Length = StringUtil::Strlen(inStr);

            m_Buffer = (TCHAR*)SystemMalloc::GetInstance().Alloc((m_Length + 1) * sizeof(TCHAR));
            StringUtil::Strcpy(m_Buffer, m_Length, inStr);
        }

        String(const TCHAR* inStr, const SIZE_T inLen) : m_Length(inLen)
        {
            m_Buffer = (TCHAR*)SystemMalloc::GetInstance().Alloc((m_Length + 1) * sizeof(TCHAR));
            StringUtil::Strcpy(m_Buffer, m_Length, inStr);
        }

        String(const String& rl) : m_Length(rl.m_Length)
        {
            m_Buffer = (TCHAR*)SystemMalloc::GetInstance().Alloc((m_Length + 1) * sizeof(TCHAR));
            StringUtil::Strcpy(m_Buffer, m_Length, rl.m_Buffer);
        }
       
        SIZE_T GetLength() const
        {
            return m_Length;
        }

        TCHAR& operator[] (const SIZE_T index)
        {
            if (index < 0 || index >= m_Length)
            {
                return m_Buffer[0];
            }
            return m_Buffer[index];
        }

        const TCHAR& operator[] (const SIZE_T index) const
        {
            if (index < 0 || index >= m_Length)
            {
                return m_Buffer[0];
            }
            return m_Buffer[index];
        }

        String& operator = (const String& rl)
        {
            if (m_Length < rl.m_Length)
            {
                SystemMalloc::GetInstance().Realloc(m_Buffer, (rl.m_Length + 1) * sizeof(TCHAR));
            }    
            StringUtil::Strcpy(m_Buffer, rl.m_Buffer, m_Length);
        }

        String& operator = (const TCHAR* inStr)
        {
            SIZE_T newLength = StringUtil::Strlen(inStr);
            if (m_Length < newLength)
            {
                SystemMalloc::GetInstance().Realloc(m_Buffer, (newLength + 1) * sizeof(TCHAR));
            }
            
            m_Length = newLength;
            StringUtil::Strcpy(m_Buffer, inStr, m_Length);
        }

        bool operator == (const String& rl) const
        {
            return m_Length == rl.m_Length && ( StringUtil::Strcmp(m_Buffer, rl.m_Buffer, m_Length) == 0 );
        }

        bool operator != (const String& rl) const
        {
            return !(*this == rl);
        }


    };
}

#endif // !__BLADE_CONTAINS_STRING_H__
