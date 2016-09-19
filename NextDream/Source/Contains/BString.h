#ifndef __BLADE_CONTAINS_STRING_H__
#define __BLADE_CONTAINS_STRING_H__

#include <CharDefine.h>
#include <BArchiveBase.h>
#include <StringUtil.h>
#include <Memory.h>

namespace BladeEngine
{
    class BString
    {
        friend struct IReader;
        friend struct IWritter;

    private:
        const TCHAR* NIL = TEXT("");

    private:
        SIZE_T m_Length;
        SIZE_T m_Capacity;
        TCHAR* m_Buffer;

    public:
        BString() : m_Length(0), m_Capacity(0), m_Buffer(NULL)
        {}

        ~BString()
        {
            Malloc::Free(m_Buffer);
        }

        /**
        * @param inStr      An array of character encoding with ansi/unicode
        */
        BString(const TCHAR* inStr) : m_Length(0), m_Capacity(0), m_Buffer(NULL)
        {
            SIZE_T newLength = StringUtil::Strlen(inStr);
            if (newLength != 0)
            {
                m_Capacity = m_Length = newLength;
                SIZE_T sizeInByte = (m_Capacity + 1) * sizeof(TCHAR);
                m_Buffer = (TCHAR*)Malloc::Alloc(sizeInByte);
                StringUtil::Strcpy(m_Buffer, sizeInByte, inStr);
            }
        }

        BString(const TCHAR* inStr, const SIZE_T inLen) : m_Length(inLen), m_Capacity(inLen), m_Buffer(NULL)
        {
            if (m_Length != 0)
            {
                SIZE_T sizeInByte = (m_Capacity + 1) * sizeof(TCHAR);
                m_Buffer = (TCHAR*)Malloc::Alloc(sizeInByte);
                StringUtil::Strncpy(m_Buffer, sizeInByte, inStr, inLen);
            }
        }

        BString(const BString& rl) : m_Length(rl.m_Length), m_Capacity(rl.m_Length), m_Buffer(NULL)
        {
            if (m_Length != 0)
            {
                SIZE_T sizeInByte = (m_Capacity + 1) * sizeof(TCHAR);
                m_Buffer = (TCHAR*)Malloc::Alloc(sizeInByte);
                StringUtil::Strcpy(m_Buffer, sizeInByte, rl.m_Buffer);
            }
        }

        operator const TCHAR*() const { return m_Buffer == NULL ? m_Buffer : NIL; }

    public:
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
            if (rl.m_Length == 0)
            {
                m_Length = 0;
                m_Buffer = NULL;
                return *this;
            }

            SIZE_T sizeInByte = (rl.m_Length + 1) * sizeof(TCHAR);
            if (m_Capacity < rl.m_Length)
            {
                m_Capacity = rl.m_Length;
                m_Buffer = (TCHAR*)Malloc::Realloc(m_Buffer, sizeInByte);
            }    

            m_Length = rl.m_Length;
            StringUtil::Strcpy(m_Buffer, sizeInByte, rl.m_Buffer);

            return *this;
        }

        BString& operator = (const TCHAR* inStr)
        {
            SIZE_T newLength = StringUtil::Strlen(inStr);
            if (newLength == 0)
            {
                m_Length = 0;
                m_Buffer = NULL;
                return *this;
            }

            SIZE_T sizeInByte = (newLength + 1) * sizeof(TCHAR);
            if (m_Capacity < newLength)
            {
                m_Capacity = newLength;
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

        BString operator + (const BString& rh) const
        {
            BString newStr;
            if (m_Length + rh.m_Length == 0)
            {
                return *this;
            }

            newStr.Reserve(m_Length + rh.m_Length);

            StringUtil::Strncpy(newStr.m_Buffer, (newStr.m_Capacity + 1) * sizeof(TCHAR), m_Buffer, m_Length);
            StringUtil::Strncpy(newStr.m_Buffer + m_Length, (newStr.m_Capacity - m_Length + 1) * sizeof(TCHAR), rh.m_Buffer, rh.m_Length);

            newStr.m_Length = m_Length + rh.m_Length;
            return newStr;
        }

    public:
        void Reserve(uint32 inNewCapacity)
        {
            if (inNewCapacity == 0)
            {
                m_Buffer = NULL;
                return;
            }

            SIZE_T newSizeInByte = (inNewCapacity + 1) * sizeof(TCHAR);
            if (m_Capacity < inNewCapacity)
            {
                TCHAR* newBuffer = m_Buffer == NULL ? (TCHAR*)Malloc::Alloc(newSizeInByte) :
                    (TCHAR*)Malloc::Realloc(m_Buffer, newSizeInByte);

                if (newBuffer != m_Buffer && NULL != m_Buffer)
                {
                    StringUtil::Strncpy(newBuffer, newSizeInByte, m_Buffer, m_Length);
                    Malloc::Free(m_Buffer);
                }
                m_Buffer = newBuffer;
                m_Capacity = inNewCapacity;
            }
        }

    public:
        int32 Compare(const BString& rh) const
        {
            return StringUtil::Strcmp(m_Buffer, rh.m_Buffer, m_Length < rh.m_Length ? m_Length : rh.m_Length);
        }

        int32 IndexOf(TCHAR ch) const
        {
            for (int32 i = m_Length - 1; i >= 0; --i)
            {
                if (m_Buffer[i] == ch)
                {
                    return i;
                }
            }
            return -1;
        }

        BString SubStr(uint32 inStart, uint32 inEnd) const
        {
            return BString(m_Buffer + inStart, inEnd - inStart + 1);
        }

    public:
        SIZE_T GetLength() const
        {
            return m_Length;
        }

    public:
        friend IWriter& operator << (IWriter& inWriter, const BString& inStr)
        {
            inWriter.Write((const byte*)inStr.m_Buffer, inStr.m_Length);
            return inWriter;
        }

        friend IReader& operator >> (IReader& inReader, BString& inStr)
        {
            if (!inReader)
            {
                return inReader;
            }

            /*byte* buffer = (byte*)inStr.m_Buffer;
            SIZE_T sizeInByte = inStr.m_Length * sizeof(TCHAR);

            SIZE_T preReadSizeInByte = 0;
            while (!inReader.TestStrLen(&preReadSizeInByte))
            {
                if (sizeInByte + preReadSizeInByte > inStr.m_Capacity * sizeof(TCHAR))
                {
                    inStr.Reserve(length + preReadLen);
                }

                inReader.Read(buffer + sizeInByte - 1, preReadLen);
                sizeInByte = sizeInByte + preReadSizeInByte;
            }

            if (length % sizeof(TCHAR) != 0)
            {
                inReader.MarkFailed();
            }*/

            return inReader;
        }
    };
}

#endif // !__BLADE_CONTAINS_STRING_H__
