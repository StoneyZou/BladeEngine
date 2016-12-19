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
        SIZE_T m_CapacityInByte;
        TCHAR* m_Buffer;

    public:
        BString() : m_Length(0), m_CapacityInByte(0), m_Buffer(NULL)
        {}

        ~BString()
        {
            Malloc::Free(m_Buffer);
        }

        /**
        * @param inStr      An array of character encoding with ansi/unicode
        */
        BString(const TCHAR* inStr) : m_Length(0), m_CapacityInByte(0), m_Buffer(NULL)
        {
            SIZE_T newLength = StringUtil::Strlen(inStr);
            if (newLength != 0)
            {
                m_Length = newLength;
                m_CapacityInByte = (newLength + 1) * sizeof(TCHAR);
                m_Buffer = (TCHAR*)Malloc::Alloc(m_CapacityInByte);
                StringUtil::Strcpy(m_Buffer, m_CapacityInByte, inStr);
            }
        }

        BString(const TCHAR* inStr, const SIZE_T inLen) : m_Length(inLen), m_CapacityInByte(inLen), m_Buffer(NULL)
        {
            if (m_Length != 0)
            {
                m_CapacityInByte = (m_Length + 1) * sizeof(TCHAR);
                m_Buffer = (TCHAR*)Malloc::Alloc(m_CapacityInByte);
                StringUtil::Strncpy(m_Buffer, m_CapacityInByte, inStr, inLen);
                m_Buffer[inLen] = 0;
            }
        }

        BString(const BString& rl) : m_Length(rl.m_Length), m_CapacityInByte(0), m_Buffer(NULL)
        {
            if (m_Length != 0)
            {
                m_CapacityInByte = (m_Length + 1) * sizeof(TCHAR);
                m_Buffer = (TCHAR*)Malloc::Alloc(m_CapacityInByte);
                StringUtil::Strcpy(m_Buffer, m_CapacityInByte, rl.m_Buffer);
            }
        }

        operator const TCHAR*() const { return m_Buffer != NULL ? m_Buffer : NIL; }

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
            if (m_CapacityInByte < sizeInByte)
            {
                m_CapacityInByte = sizeInByte;
                m_Buffer = (TCHAR*)Malloc::Realloc(m_Buffer, m_CapacityInByte);
            }    

            m_Length = rl.m_Length;
            StringUtil::Strcpy(m_Buffer, m_CapacityInByte, rl.m_Buffer);

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
            if (m_CapacityInByte < sizeInByte)
            {
                m_CapacityInByte = sizeInByte;
                m_Buffer = (TCHAR*)Malloc::Realloc(m_Buffer, m_CapacityInByte);
            }
            
            m_Length = newLength;
            StringUtil::Strcpy(m_Buffer, m_CapacityInByte, inStr);

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

            StringUtil::Strncpy(newStr.m_Buffer, newStr.m_CapacityInByte, m_Buffer, m_Length);
            StringUtil::Strncpy(newStr.m_Buffer + m_Length, newStr.m_CapacityInByte - (m_Length + 1) * sizeof(TCHAR), rh.m_Buffer, rh.m_Length);
            newStr.m_Buffer[m_Length + rh.m_Length] = 0;

            newStr.m_Length = m_Length + rh.m_Length;
            return newStr;
        }

    public:
        void Reserve(uint32 inNewCapacityInTChar)
        {
            ReserveInByte((inNewCapacityInTChar + 1) * sizeof(TCHAR));
        }

        void ReserveInByte(uint32 inNewCapacityInByte)
        {
            if (inNewCapacityInByte == 0)
            {
                Malloc::Free(m_Buffer);
                m_Buffer = NULL;
                m_CapacityInByte = 0;
                return;
            }

            if (m_CapacityInByte < inNewCapacityInByte)
            {
                TCHAR* newBuffer = m_Buffer == NULL ? (TCHAR*)Malloc::Alloc(inNewCapacityInByte) :
                    (TCHAR*)Malloc::Realloc(m_Buffer, inNewCapacityInByte);

                if (newBuffer != m_Buffer && NULL != m_Buffer)
                {
                    StringUtil::Strncpy(newBuffer, inNewCapacityInByte, m_Buffer, m_Length);
                    Malloc::Free(m_Buffer);
                }

                m_Buffer = newBuffer;
                m_CapacityInByte = inNewCapacityInByte;
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

            byte* buffer = (byte*)inStr.m_Buffer;
            SIZE_T curSizeInByte = (inStr.m_Length + 1) * sizeof(TCHAR);
            SIZE_T preReadSizeInByte = 0;

            while (!inReader.TestStrLen(&preReadSizeInByte))
            {
                if (curSizeInByte + preReadSizeInByte > inStr.m_CapacityInByte)
                {
                    inStr.ReserveInByte(curSizeInByte + preReadSizeInByte);
                }

                inReader.Read(buffer + curSizeInByte, preReadSizeInByte);
                curSizeInByte = curSizeInByte + preReadSizeInByte;
            }

            if (curSizeInByte % sizeof(TCHAR) != 0)
            {
                inReader.MarkFailed();
            }

            return inReader;
        }
    };
}

#endif // !__BLADE_CONTAINS_STRING_H__
