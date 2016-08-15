#ifndef __BLADE_SERIALZATION_ARCHIVE_H__
#define __BLADE_SERIALZATION_ARCHIVE_H__

#include <TypeDefine.h>
#include <MemUtil.h>
#include <BString.h>
#include <EnumDefine.h>
#include <GeneralSystemAPI.h>

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

    struct OutputAnsiStr
    {
        typedef ANSICHAR CharType;
        OutputAnsiStr(const ANSICHAR*& inStrBuf, SIZE_T inStrLen) : StrBuf(inStrBuf), StrLen(inStrLen)
        {}

        const ANSICHAR*& StrBuf;
        SIZE_T StrLen;
    };

    struct OutputWideStr
    {
        typedef WIDECHAR CharType;
        OutputWideStr(const WIDECHAR*& inStrBuf, SIZE_T inStrLen) : StrBuf(inStrBuf), StrLen(inStrLen)
        {}

        const WIDECHAR*& StrBuf;
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
        
        virtual int32 Seek(ESEEK_POS inPos, uint32 inOffset) = 0;
        virtual int32 Read(byte* inBuffer, SIZE_T inBufferSize) = 0;

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

        virtual int32 Seek(ESEEK_POS inRelativePos, uint32 inOffset) = 0;
        virtual int32 Write(const byte* inBuffer, SIZE_T inBufferSize) = 0;

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

    class MemoryReader : public IReader
    {
    private:
        const byte* m_Buffer;
        SIZE_T m_BufferSize;
        SIZE_T m_CurPos;

    public:
        MemoryReader(const byte* inBuffer, SIZE_T inBufferSize)
            : IReader(),
            m_Buffer(inBuffer),
            m_BufferSize(inBufferSize),
            m_CurPos(0)
        {}

        virtual bool IsEOF() const { return m_CurPos == m_BufferSize; }

    private:
        bool SetCurPos(SIZE_T inPos)
        {
            if (inPos > m_BufferSize && inPos < 0)
                m_IsFailed = true;

            m_CurPos = inPos;
            return m_IsFailed;
        }

    public:
        virtual int32 Seek(ESEEK_POS inRelativePos, uint32 inOffset)
        {
            uint32 pos = 0;
            switch (inRelativePos)
            {
            case BladeEngine::ESEEK_POS_BEGIN:
                pos = 0;
                break;
            case BladeEngine::ESEEK_POS_CUR:
                pos = m_CurPos;
                break;
            case BladeEngine::ESEEK_POS_END:
                pos = m_BufferSize;
                break;
            default:
                break;
            }

            SetCurPos(pos + inOffset);
            return m_CurPos;
        }

        virtual int32 Read(byte* inBuffer, SIZE_T inBufferSize)
        {
            if (m_IsFailed)
                return -1;

            if (inBufferSize == 0)
                return 0;

            if (inBufferSize > m_BufferSize - m_CurPos)
            {
                m_IsFailed = true;
                return -1;
            }

            MemUtil::Memcopy(inBuffer, inBufferSize, m_Buffer + m_CurPos, inBufferSize);
            SetCurPos(m_CurPos + inBufferSize);

            return m_BufferSize;
        }

        virtual IReader& operator >> (uint8& outValue)
        {
            Read((byte*)&outValue, sizeof(uint8));
            return *this;
        }

        virtual IReader& operator >> (uint16& outValue)
        {
            Read((byte*)&outValue, sizeof(uint16));
            return *this;
        }

        virtual IReader& operator >> (uint32& outValue)
        {
            Read((byte*)&outValue, sizeof(uint32));
            return *this;
        }

        virtual IReader& operator >> (uint64& outValue)
        {
            Read((byte*)&outValue, sizeof(uint64));
            return *this;
        }

        virtual IReader& operator >> (int8& outValue)
        {
            Read((byte*)&outValue, sizeof(int8));
            return *this;
        }

        virtual IReader& operator >> (int16& outValue)
        {
            Read((byte*)&outValue, sizeof(int16));
            return *this;
        }

        virtual IReader& operator >> (int32& outValue)
        {
            Read((byte*)&outValue, sizeof(int32));
            return *this;
        }

        virtual IReader& operator >> (int64& outValue)
        {
            Read((byte*)&outValue, sizeof(int64));
            return *this;
        }

        virtual IReader& operator >> (float& outValue)
        {
            Read((byte*)&outValue, sizeof(float));
            return *this;
        }

        virtual IReader& operator >> (double& outValue)
        {
            Read((byte*)&outValue, sizeof(double));
            return *this;
        }

        virtual IReader& operator >> (InputAnsiStr& outStr)
        {
            Read((byte*)outStr.StrBuf, sizeof(InputAnsiStr::CharType) * outStr.MaxStrLen);
            return *this;
        }

        virtual IReader& operator >> (InputWideStr& outStr)
        {
            Read((byte*)outStr.StrBuf, sizeof(InputWideStr::CharType) * outStr.MaxStrLen);
            return *this;
        }
    };

    class FileReader : public IReader
    {
    private:
        HFile m_HFile;
        uint64 m_FileSize;

        uint64 m_PreReadPos;
        uint64 m_PreReadBufSize;
        TArray<byte*> m_PreReadBuf;

        uint64 m_CurLocalPos;
        uint64 m_CurFilePos;

    public:
        FileReader(HFile inHFile, uint32 inReadBufSize) 
            : IReader(), 
            m_HFile(inHFile),
            m_FileSize(0), 
            m_PreReadPos(0),
            m_PreReadBufSize(inReadBufSize),
            m_PreReadBuf(inReadBufSize),
            m_CurLocalPos(0),
            m_CurFilePos(0)
        {
            m_FileSize = SystemAPI::GetFileSize(m_HFile);
        }

        virtual ~FileReader() 
        {
        };

        virtual bool IsEOF() const { return m_CurLocalPos == m_FileSize; }

    private:
        bool SetCurPos(SIZE_T inPos)
        {
            if (m_IsFailed || (inPos > m_FileSize && inPos < 0))
            {
                m_IsFailed = true;
                return m_IsFailed;
            }

            // 读取指针超出了读取缓存，重新调整文件读取指针并清空读取缓存区
            if (inPos < m_PreReadPos && inPos >= m_PreReadPos + m_PreReadBuf.GetLength())
            {
                SeekFile(m_HFile, inPos);
                m_PreReadPos = 0;
                m_PreReadBuf.Clear();
            }
            m_CurLocalPos = inPos;

            return m_IsFailed;
        }

        int32 ReadFile(HFile inFile, byte* inBuf, uint32 inBufSize)
        {
            int32 realSize = SystemAPI::ReadFile(m_HFile, inBuf, inBufSize);
            if (realSize == -1)
            {
                m_IsFailed = true;
            }

            m_CurFilePos += realSize;
            return realSize;
        }

        int32 SeekFile(HFile inFile, uint32 inPos)
        {
            m_CurFilePos = inPos;
            return (int32)SystemAPI::SeekFile(m_HFile, inPos, ESEEK_POS_BEGIN);
        }

    public:
        virtual int32 Seek(ESEEK_POS inRelativePos, uint32 inOffset)
        {
            uint32 pos = 0;
            switch (inRelativePos)
            {
            case BladeEngine::ESEEK_POS_BEGIN:
                pos = 0;
                break;
            case BladeEngine::ESEEK_POS_CUR:
                pos = m_CurLocalPos;
                break;
            case BladeEngine::ESEEK_POS_END:
                pos = m_FileSize;
                break;
            default:
                break;
            }
            return SetCurPos(pos + inOffset) ? m_CurLocalPos : -1;
        }

        virtual int32 Read(byte* inBuffer, SIZE_T inBufferSize)
        {
            if (m_IsFailed)
                return -1;

            if (inBufferSize == 0)
                return 0;

            if (inBufferSize < m_FileSize - m_CurLocalPos)
            {
                m_IsFailed = true;
                return -1;
            }

            // 可以从读取缓存区读取数据的情况
            uint32 preReadSize = m_PreReadBuf.GetLength();
            uint32 preReadPartSize = 0;
            int32 realSize = 0;
            
            if (m_CurLocalPos >= m_PreReadPos && m_CurLocalPos < m_PreReadPos + preReadSize)
            {
                if (inBufferSize < m_PreReadPos + preReadSize - m_CurLocalPos)
                {
                    MemUtil::Memcopy(inBuffer, inBufferSize, m_PreReadBuf.TypePtr() + m_PreReadPos - m_CurLocalPos, inBufferSize);
                    return SetCurPos(m_CurLocalPos + inBufferSize) ? inBufferSize : -1;
                }
                else
                {
                    preReadPartSize = m_PreReadPos + preReadSize - m_CurLocalPos;
                    MemUtil::Memcopy(inBuffer, preReadPartSize, m_PreReadBuf.TypePtr() + m_PreReadPos - preReadPartSize, preReadPartSize);
                    
                    inBuffer += preReadPartSize;
                    inBufferSize -= preReadPartSize;

                    // 上一次读取缓存到达文件末端，直接设置读取指针到文件末端
                    if (m_PreReadPos + preReadSize >= m_FileSize)
                    {
                        return SetCurPos(m_FileSize) ? preReadPartSize : -1;
                    }
                }
            }

            // 本次读取超过读取缓存区的一半大小或者直接读取到文件结尾，直接从文件中读取不经过读取缓存
            if (inBufferSize > (m_PreReadBufSize >> 1) || inBufferSize > m_FileSize - m_CurLocalPos)
            {
                realSize = ReadFile(m_HFile, inBuffer, inBufferSize);
                return SetCurPos(m_CurLocalPos + realSize + preReadPartSize) ? realSize + preReadPartSize : -1;
            }

            m_PreReadPos = m_CurFilePos;
            realSize = ReadFile(m_HFile, inBuffer, inBufferSize);
            return SetCurPos(m_CurLocalPos + realSize + preReadPartSize) ? realSize + preReadPartSize : -1;
        }

        virtual IReader& operator >> (uint8& outValue)
        {
            Read((byte*)&outValue, sizeof(uint8));
            return *this;
        }

        virtual IReader& operator >> (uint16& outValue)
        {
            Read((byte*)&outValue, sizeof(uint16));
            return *this;
        }

        virtual IReader& operator >> (uint32& outValue)
        {
            Read((byte*)&outValue, sizeof(uint32));
            return *this;
        }

        virtual IReader& operator >> (uint64& outValue)
        {
            Read((byte*)&outValue, sizeof(uint64));
            return *this;
        }

        virtual IReader& operator >> (int8& outValue)
        {
            Read((byte*)&outValue, sizeof(int8));
            return *this;
        }

        virtual IReader& operator >> (int16& outValue)
        {
            Read((byte*)&outValue, sizeof(int16));
            return *this;
        }

        virtual IReader& operator >> (int32& outValue)
        {
            Read((byte*)&outValue, sizeof(int32));
            return *this;
        }

        virtual IReader& operator >> (int64& outValue)
        {
            Read((byte*)&outValue, sizeof(int64));
            return *this;
        }

        virtual IReader& operator >> (float& outValue)
        {
            Read((byte*)&outValue, sizeof(float));
            return *this;
        }

        virtual IReader& operator >> (double& outValue)
        {
            Read((byte*)&outValue, sizeof(double));
            return *this;
        }

        virtual IReader& operator >> (InputAnsiStr& outStr)
        {
            Read((byte*)outStr.StrBuf, sizeof(InputAnsiStr::CharType) * outStr.MaxStrLen);
            return *this;
        }

        virtual IReader& operator >> (InputWideStr& outStr)
        {
            Read((byte*)outStr.StrBuf, sizeof(InputWideStr::CharType) * outStr.MaxStrLen);
            return *this;
        }
    };

    struct MemoryWriter : public IWriter
    {
    private:
        byte* m_Buffer;
        SIZE_T m_BufferSize;
        SIZE_T m_CurPos;

    public:
        MemoryWriter(byte* inBuf, SIZE_T inBufSize)
            : IWriter(),
            m_Buffer(inBuf),
            m_BufferSize(inBufSize),
            m_CurPos(0)
        {}
        
        virtual bool IsEOF() const { return m_CurPos == m_BufferSize; }

    private:
        bool SetCurPos(SIZE_T inPos)
        {
            if (inPos > m_BufferSize && inPos < 0)
                m_IsFailed = true;

            m_CurPos = inPos;
            return m_IsFailed;
        }

    public:
        virtual int32 Seek(ESEEK_POS inRelativePos, uint32 inOffset)
        {
            uint32 pos = 0;
            switch (inRelativePos)
            {
            case BladeEngine::ESEEK_POS_BEGIN:
                pos = 0;
                break;
            case BladeEngine::ESEEK_POS_CUR:
                pos = m_CurPos;
                break;
            case BladeEngine::ESEEK_POS_END:
                pos = m_BufferSize;
                break;
            default:
                break;
            }

            SetCurPos(pos + inOffset);
            return m_CurPos;
        }

        virtual int32 Write(const byte* inBuffer, SIZE_T inBufferSize)
        {
            if (m_IsFailed || inBufferSize > m_BufferSize - m_CurPos)
            {
                m_IsFailed = true;
                return -1;
            }

            if (inBufferSize == 0)
            {
                return 0;
            }

            MemUtil::Memcopy(m_Buffer + m_CurPos, inBufferSize, inBuffer, inBufferSize);
            return SetCurPos(m_CurPos + inBufferSize) ? inBufferSize : -1;
        }

        virtual IWriter& operator << (uint8 outValue)
        {
            Write((const byte*)&outValue, sizeof(uint8));
            return *this;
        }

        virtual IWriter& operator << (uint16 outValue)
        {
            Write((const byte*)&outValue, sizeof(uint16));
            return *this;
        }
        virtual IWriter& operator << (uint32 outValue)
        {
            Write((const byte*)&outValue, sizeof(uint32));
            return *this;
        }
        virtual IWriter& operator << (uint64 outValue)
        {
            Write((const byte*)&outValue, sizeof(uint64));
            return *this;
        }
        virtual IWriter& operator << (int8 outValue)
        {
            Write((const byte*)&outValue, sizeof(int8));
            return *this;
        }
        virtual IWriter& operator << (int16 outValue)
        {
            Write((const byte*)&outValue, sizeof(int16));
            return *this;
        }
        virtual IWriter& operator << (int32 outValue)
        {
            Write((const byte*)&outValue, sizeof(int32));
            return *this;
        }
        virtual IWriter& operator << (int64 outValue)
        {
            Write((const byte*)&outValue, sizeof(int64));
            return *this;
        }
        virtual IWriter& operator << (float outValue)
        {
            Write((const byte*)&outValue, sizeof(float));
            return *this;
        }
        virtual IWriter& operator << (double outValue)
        {
            Write((const byte*)&outValue, sizeof(double));
            return *this;
        }
        virtual IWriter& operator << (OutputAnsiStr& outStr)
        {
            Write((const byte*)&outStr.StrBuf, sizeof(OutputAnsiStr::CharType) *  outStr.StrLen);
            return *this;
        }
        virtual IWriter& operator << (OutputWideStr& outStr)
        {
            Write((const byte*)&outStr.StrBuf, sizeof(InputWideStr::CharType) * outStr.StrLen);
            return *this;
        }
    };

}

#endif // !__BLADE_SERIALZATION_ARCHIVE_H__