#ifndef __BLADE_CONTAINS_STRING_H__
#define __BLADE_CONTAINS_STRING_H__

#include <CharDefine.h>
#include <TypeDefine.h>
#include <StringUtil.h>
#include <Memory.h>

namespace BladeEngine
{
    class BString
    {
    private:
        const TCHAR* s_NIL = "";

    private:
        SIZE_T m_Length;
        SIZE_T m_Capacity;
        TCHAR* m_Buffer;

    public:
        BString() : m_Length(0), m_Capacity(0), m_Buffer(NULL)
        {}

        /**
        * @param inStr      An array of character encoding with ansi/unicode
        */
        BString(const TCHAR* inStr)
        {
            m_Capacity = m_Length = StringUtil::Strlen(inStr);
            SIZE_T sizeInByte = (m_Capacity + 1) * sizeof(TCHAR);

            m_Buffer = (TCHAR*)Malloc::Alloc(sizeInByte);
            StringUtil::Strcpy(m_Buffer, sizeInByte, inStr);
        }

        BString(const TCHAR* inStr, const SIZE_T inLen) : m_Length(inLen), m_Capacity(inLen)
        {
            SIZE_T sizeInByte = (m_Capacity + 1) * sizeof(TCHAR);

            m_Buffer = (TCHAR*)Malloc::Alloc(sizeInByte);
            StringUtil::Strcpy(m_Buffer, sizeInByte, inStr);
        }

        BString(const BString& rl) : m_Length(rl.m_Length), m_Capacity(rl.m_Length)
        {
            SIZE_T sizeInByte = (m_Capacity + 1) * sizeof(TCHAR);

            m_Buffer = (TCHAR*)Malloc::Alloc(sizeInByte);
            StringUtil::Strcpy(m_Buffer, sizeInByte, rl.m_Buffer);
        }

        operator const TCHAR*() const { return m_Buffer; }
       
        SIZE_T GetLength() const
        {
            return m_Length;
        }

        int32 Compare(const BString& rh) const
        {
            return StringUtil::Strcmp(m_Buffer, rh.m_Buffer, m_Length < rh.m_Length ? m_Length : rh.m_Length);
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

        BString& operator = (const BString& rl)
        {
            SIZE_T sizeInByte = (rl.m_Length + 1) * sizeof(TCHAR);
            if (m_Capacity < sizeInByte)
            {
                m_Capacity = sizeInByte;
                m_Buffer = (TCHAR*)Malloc::Realloc(m_Buffer, sizeInByte);
            }    

            m_Length = rl.m_Length;
            StringUtil::Strcpy(m_Buffer, sizeInByte, rl.m_Buffer);

            return *this;
        }

        BString& operator = (const TCHAR* inStr)
        {
            SIZE_T newLength = StringUtil::Strlen(inStr);
            SIZE_T sizeInByte = (newLength + 1) * sizeof(TCHAR);

            if (m_Capacity < sizeInByte)
            {
                m_Capacity = sizeInByte;
                m_Buffer = (TCHAR*)Malloc::Realloc(m_Buffer, sizeInByte);
            }
            
            m_Length = newLength;
            StringUtil::Strcpy(m_Buffer, sizeInByte, inStr);

            return *this;
        }

        bool operator == (const BString& rh) const
        {
            return m_Length == rh.m_Length && ( StringUtil::Strcmp(m_Buffer, rh.m_Buffer, m_Length) == 0 );
        }

        bool operator != (const BString& rh) const
        {
            return !(*this == rh);
        }

        bool operator == (const TCHAR* other) const
        {
            return m_Length == StringUtil::Strlen(other) && (StringUtil::Strcmp(m_Buffer, other, m_Length) == 0);
        }

        bool operator != (const TCHAR* other) const
        {
            return !(*this == other);
        }
    };
}

#endif // !__BLADE_CONTAINS_STRING_H__
