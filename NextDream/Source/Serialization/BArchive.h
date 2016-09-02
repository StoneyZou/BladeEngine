#ifndef __BLADE_SERIALZATION_ARCHIVE_H__
#define __BLADE_SERIALZATION_ARCHIVE_H__

#include <BArchiveBase.h>
#include <MemUtil.h>
#include <EnumDefine.h>
#include <PlatformAPI.h>
#include <Utility.h>

namespace BladeEngine
{
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
        virtual SIZE_T Seek(ESEEK_POS inRelativePos, uint32 inOffset)
        {
            SIZE_T pos = 0;
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

        virtual SIZE_T Read(byte* inBuffer, SIZE_T inBufferSize)
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
        FileHandle m_FileHandle;
        SIZE_T m_FileSize;

        SIZE_T m_CurPreReadPos;
        SIZE_T m_ReadBufferSize;
        TArray<byte> m_ReadBuffer;

        SIZE_T m_CurLocalPos;
        SIZE_T m_CurFilePos;

    public:
        FileReader(FileHandle inHFile, uint32 inReadBufSize) 
            : IReader(), 
            m_FileHandle(inHFile),
            m_FileSize(0), 
            m_CurPreReadPos(0),
            m_ReadBufferSize(0),
            m_ReadBuffer(inReadBufSize),
            m_CurLocalPos(0),
            m_CurFilePos(0)
        {
            m_FileSize = PlatformAPI::GetFileSize(m_FileHandle);
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
            if (inPos < m_CurPreReadPos && inPos >= m_CurPreReadPos + m_ReadBufferSize)
            {
                SeekFile(m_FileHandle, inPos);
                m_ReadBufferSize = 0;
                m_ReadBuffer.Clear();
            }
            m_CurLocalPos = inPos;

            return m_IsFailed;
        }

        SIZE_T ReadFile(byte* inBuffer, SIZE_T inBufferSize)
        {
            int32 realSize = PlatformAPI::ReadFile(m_FileHandle, inBuffer, inBufferSize);
            if (realSize == -1)
            {
                m_IsFailed = true;
                return -1;
            }

            m_CurFilePos += realSize;
            m_CurLocalPos = m_CurFilePos;

            return realSize;
        }

        SIZE_T ReadBuffer(byte* inBuffer, SIZE_T inBufferSize)
        {
            BladeAssert(m_CurLocalPos >= m_CurPreReadPos);
            BladeAssert(m_ReadBufferSize >= inBufferSize);

            if ( m_CurLocalPos < m_CurPreReadPos || m_CurLocalPos > m_CurPreReadPos + m_ReadBufferSize)
            {
                m_IsFailed = true;
                return -1;
            }

            if (inBufferSize > m_ReadBufferSize + m_CurPreReadPos - m_CurLocalPos)
            {
                m_IsFailed = true;
                return -1;
            }

            MemUtil::Memcopy(inBuffer, inBufferSize, m_ReadBuffer.TypePtr() + m_CurLocalPos - m_CurPreReadPos, inBufferSize);
            m_CurLocalPos = m_CurFilePos + inBufferSize;

            return inBufferSize;
        }

        SIZE_T PreRead()
        {
            m_ReadBufferSize = PlatformAPI::ReadFile(m_FileHandle, m_ReadBuffer.TypePtr(), m_ReadBuffer.Size());
            if (m_ReadBufferSize == -1)
            {
                m_IsFailed = true;
                return -1;
            }

            m_CurFilePos += m_ReadBufferSize;
            return m_ReadBufferSize;
        }

        SIZE_T SeekFile(FileHandle inFile, SIZE_T inPos)
        {
            SIZE_T result = PlatformAPI::SeekFile(m_FileHandle, inPos, ESEEK_POS_BEGIN);
            if (result == -1)
            {
                m_IsFailed = true;
            }

            m_CurFilePos = inPos;
            m_CurLocalPos = m_CurFilePos;

            return m_CurFilePos;
        }

    public:
        virtual SIZE_T Seek(ESEEK_POS inRelativePos, SIZE_T inOffset)
        {
            SIZE_T pos = 0;
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

        virtual SIZE_T Read(byte* inBuffer, SIZE_T inBufferSize)
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
            SIZE_T preReadPartSize = 0;
            SIZE_T realSize = 0;
            
            if (m_CurLocalPos >= m_CurPreReadPos && m_CurLocalPos < m_CurPreReadPos + m_ReadBufferSize)
            {
                if (inBufferSize < m_CurPreReadPos + m_ReadBufferSize - m_CurLocalPos)
                {
                    return ReadBuffer(inBuffer, inBufferSize);
                }
                else
                {
                    preReadPartSize = m_ReadBufferSize + m_CurPreReadPos - m_CurLocalPos;
                    ReadBuffer(inBuffer, preReadPartSize);
                    
                    inBuffer += preReadPartSize;
                    inBufferSize -= preReadPartSize;

                    // 上一次读取缓存到达文件末端，直接设置读取指针到文件末端
                    if (m_CurPreReadPos + m_ReadBufferSize >= m_FileSize)
                    {
                        return !m_IsFailed ? preReadPartSize : -1;
                    }
                }
            }

            // 本次读取超过读取缓存区的一半大小或者直接读取到文件结尾，直接从文件中读取不经过读取缓存
            if (inBufferSize > (m_ReadBuffer.Size() >> 1) || inBufferSize > m_FileSize - m_CurLocalPos)
            {
                realSize = ReadFile(inBuffer, inBufferSize);
                return !m_IsFailed ? realSize + preReadPartSize : -1;
            }

            PreRead();
            realSize = ReadBuffer(inBuffer, inBufferSize);

            return !m_IsFailed ? realSize + preReadPartSize : -1;
        }

        const void* ReadAll()
        {
            m_ReadBuffer.Clear();
            m_ReadBuffer.Resize(m_FileSize);

            SIZE_T readSize = PreRead();
            if (readSize != m_FileSize)
            {
                m_IsFailed = true;
                return NULL;
            }

            return m_ReadBuffer.VoidPtr();
        }

        SIZE_T GetFileSize() const { return m_FileSize; }

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

    public:
        virtual SIZE_T Seek(ESEEK_POS inRelativePos, uint32 inOffset)
        {
            SIZE_T pos = 0;
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

            if (pos > m_BufferSize && pos < 0)
            {
                m_IsFailed = true;
                return -1;
            }

            m_CurPos = pos + inOffset;
            return m_CurPos;
        }

        virtual SIZE_T Write(const byte* inBuffer, SIZE_T inBufferSize)
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
            m_CurPos = m_CurPos + inBufferSize;

            return inBufferSize;
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

    class FileWriter : public IWriter
    {
    private:
        FileHandle m_HFile;

        SIZE_T m_BufferWrittenLen;
        TArray<byte> m_WritetBuffer;

        SIZE_T m_CurLocalPos;
        SIZE_T m_CurFilePos;

    public:
        FileWriter(FileHandle inHFile, uint32 inWriteBufSize)
            : IWriter(),
            m_HFile(inHFile),
            m_BufferWrittenLen(0),
            m_WritetBuffer(inWriteBufSize),
            m_CurLocalPos(0),
            m_CurFilePos(0)
        {
        }

        virtual ~FileWriter()
        {
        };

        virtual bool IsEOF() const { return false; }

    private:
        SIZE_T WriteFile(FileHandle inFile, const byte* inBuf, uint32 inBufSize)
        {
            Flush();
            SIZE_T realSize = PlatformAPI::WriteFile(m_HFile, inBuf, inBufSize);
            if (realSize == -1)
            {
                m_IsFailed = true;
            }

            m_CurFilePos += realSize;
            m_CurLocalPos = m_CurFilePos;

            return realSize;
        }

        SIZE_T WriteBuffer(const byte* inBuffer, uint32 inBufferSize)
        {
            if (inBufferSize > m_WritetBuffer.Size() - m_BufferWrittenLen)
            {
                m_IsFailed = true;
                return -1;
            }

            MemUtil::Memcopy(m_WritetBuffer.TypePtr() + m_BufferWrittenLen, inBufferSize, inBuffer, inBufferSize);
            m_BufferWrittenLen += inBufferSize;
            m_CurLocalPos = m_CurFilePos + m_BufferWrittenLen;

            return inBufferSize;
        }

        void SeekFile(FileHandle inFile, uint32 inPos)
        {
            Flush();
            SIZE_T result = PlatformAPI::SeekFile(m_HFile, inPos, ESEEK_POS_BEGIN);
            if ( result == -1)
            {
                m_IsFailed = true;
            }

            m_CurFilePos = inPos;
            m_CurLocalPos = m_CurFilePos;
        }

    public:
        virtual SIZE_T Flush()
        {
            SIZE_T realSize = 0;
            if (m_BufferWrittenLen > 0)
            {
                realSize = PlatformAPI::WriteFile(m_HFile, m_WritetBuffer.TypePtr(), m_BufferWrittenLen);
                if (realSize == -1)
                {
                    m_IsFailed = true;
                }
            }

            m_CurFilePos += realSize;
            m_BufferWrittenLen = 0;
            return realSize;
        }

        virtual SIZE_T Seek(ESEEK_POS inRelativePos, SIZE_T inOffset)
        {
            SIZE_T pos = 0;
            switch (inRelativePos)
            {
            case BladeEngine::ESEEK_POS_BEGIN:
                pos = 0;
                break;
            case BladeEngine::ESEEK_POS_CUR:
                pos = m_CurLocalPos;
                break;
            case BladeEngine::ESEEK_POS_END:
                //pos = m_FileSize;
                break;
            default:
                break;
            }

            if (pos < m_CurFilePos && pos >= m_CurFilePos + m_WritetBuffer.Size())
            {
                SeekFile(m_HFile, pos);
            }
            else
            {
                m_CurLocalPos = pos;
            }
        }

        virtual SIZE_T Write(const byte* inBuffer, SIZE_T inBufferSize)
        {
            if (m_IsFailed)
                return -1;

            if (inBufferSize == 0)
                return 0;
            
            int realSize = 0;
            if (inBufferSize > m_WritetBuffer.Size() - m_BufferWrittenLen)
            {
                realSize = WriteFile(m_HFile, inBuffer, inBufferSize);
            }
            else
            {
                realSize = WriteBuffer(inBuffer, inBufferSize);
            }

            return realSize;
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