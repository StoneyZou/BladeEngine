#ifndef __BLADE_CORE_ARCHIVE_BASE_H__
#define __BLADE_CORE_ARCHIVE_BASE_H__

#include <TypeDefine.h>
#include <EnumDefine.h>

namespace BladeEngine
{
    struct InputAnsiStr
    {
        typedef ANSICHAR CharType;
        InputAnsiStr(ANSICHAR*& inStrBuf, SIZE_T inMaxStrLen) : StrBuf(inStrBuf), MaxStrLen(inMaxStrLen)
        {}

        ANSICHAR*& StrBuf;
        SIZE_T MaxStrLen;
    };

    struct InputWideStr
    {
        typedef WIDECHAR CharType;
        InputWideStr(WIDECHAR*& inStrBuf, SIZE_T inMaxStrLen) : StrBuf(inStrBuf), MaxStrLen(inMaxStrLen)
        {}

        WIDECHAR*& StrBuf;
        SIZE_T MaxStrLen;
    };

    struct OutputStr
    {
        typedef TCHAR CharType;
        OutputStr(const TCHAR* inStrBuf, SIZE_T inStrLen) : StrBuf(inStrBuf), StrLen(inStrLen)
        {}

        const TCHAR* StrBuf;
        SIZE_T StrLen;
    };

    struct OutputAnsiStr
    {
        typedef ANSICHAR CharType;
        OutputAnsiStr(const ANSICHAR* inStrBuf, SIZE_T inStrLen) : StrBuf(inStrBuf), StrLen(inStrLen)
        {}

        const ANSICHAR*& StrBuf;
        SIZE_T StrLen;
    };

    struct OutputWideStr
    {
        typedef WIDECHAR CharType;
        OutputWideStr(const WIDECHAR* inStrBuf, SIZE_T inStrLen) : StrBuf(inStrBuf), StrLen(inStrLen)
        {}

        const WIDECHAR* StrBuf;
        SIZE_T StrLen;
    };

    struct IReader
    {
    public:
        enum _EOF { IS_EOF, };
        enum _FAILED { IS_FAILED, };

    protected:
        bool m_IsFailed;

    public:
        IReader() : m_IsFailed(false) {}
        virtual ~IReader() {};

        virtual bool IsEOF() const = 0;
        bool IsFailed() const { return m_IsFailed; }

        operator bool() const { return !IsFailed() && !IsEOF(); };

        bool operator == (_EOF _flag) const { return IsEOF(); }
        bool operator == (_FAILED _flag) const { return IsFailed(); }

        virtual SIZE_T Seek(ESEEK_POS inPos, SIZE_T inOffset) = 0;
        virtual SIZE_T Read(byte* inBuffer, SIZE_T inBufferSize) = 0;

        virtual IReader& operator >> (uint8& outoutValue) = 0;
        virtual IReader& operator >> (uint16& outoutValue) = 0;
        virtual IReader& operator >> (uint32& outoutValue) = 0;
        virtual IReader& operator >> (uint64& outoutValue) = 0;
        virtual IReader& operator >> (int8& outValue) = 0;
        virtual IReader& operator >> (int16& outValue) = 0;
        virtual IReader& operator >> (int32& outValue) = 0;
        virtual IReader& operator >> (int64& outValue) = 0;
        virtual IReader& operator >> (float& outValue) = 0;
        virtual IReader& operator >> (double& outValue) = 0;
        virtual IReader& operator >> (InputAnsiStr& outStr) = 0;
        virtual IReader& operator >> (InputWideStr& outStr) = 0;
    };

    struct IWriter
    {
    public:
        enum _EOF { IS_EOF, };
        enum _FAILED { IS_FAILED, };

    protected:
        bool m_IsFailed;

    public:
        IWriter() : m_IsFailed(false) {}
        virtual ~IWriter() {};

        virtual bool IsEOF() const = 0;
        bool IsFailed() const { return m_IsFailed; }

        operator bool() const { return !IsFailed() && !IsEOF(); };

        bool operator == (_EOF _flag) const { return IsEOF(); }
        bool operator == (_FAILED _flag) const { return IsFailed(); }

        virtual SIZE_T Seek(ESEEK_POS inRelativePos, SIZE_T inOffset) = 0;
        virtual SIZE_T Write(const byte* inBuffer, SIZE_T inBufferSize) = 0;

        virtual IWriter& operator << (uint8 outValue) = 0;
        virtual IWriter& operator << (uint16 outValue) = 0;
        virtual IWriter& operator << (uint32 outValue) = 0;
        virtual IWriter& operator << (uint64 outValue) = 0;
        virtual IWriter& operator << (int8 outValue) = 0;
        virtual IWriter& operator << (int16 outValue) = 0;
        virtual IWriter& operator << (int32 outValue) = 0;
        virtual IWriter& operator << (int64 outValue) = 0;
        virtual IWriter& operator << (float outValue) = 0;
        virtual IWriter& operator << (double outValue) = 0;
        virtual IWriter& operator << (OutputAnsiStr& outStr) = 0;
        virtual IWriter& operator << (OutputWideStr& outStr) = 0;
    };
}

#endif // !__BLADE_CORE_ARCHIVE_BASE_H__
