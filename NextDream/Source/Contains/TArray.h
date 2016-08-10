#ifndef __BLADE_CONTAINS_TARRAY_H__
#define __BLADE_CONTAINS_TARRAY_H__

#include <Utility.h>

namespace BladeEngine
{
    
    template<typename Type>
    class TArray : public INoncopyable
    {
    private:
        class Iterator
        {
        public:
            TArray& m_Data;
            SIZE_T m_Index;

        public:
            Iterator(TArray& inData, SIZE_T inIndex) : m_Data(inData), m_Index(inIndex)
            {}

        public:
            void operator ++()
            {
                ++m_Index;
            }

            void operator --()
            {
                ++m_Index;
            }

            Type& operator-> ()
            {
                return m_Data[m_Index];
            }

            const Type& operator-> () const
            {
                return m_Data[m_Index];
            }
        };

    private:
        const SIZE_T MIN_ADJUST_CAPACITY = 10;

    private:
        SIZE_T m_Capacity;
        SIZE_T m_Length;
        Type* m_pData;

    private:
        void _ExpandCapacity( SIZE_T inNewCapacity )
        {
            if (inNewCapacity < m_Capacity)
            {
                return;
            }

            m_Capacity = inNewCapacity;
            m_pData = (Type*)Malloc::Alloc(sizeof(Type) * m_Capacity);
        }

        void _ExpandCapacity()
        {
            SIZE_T newCapacity = m_Capacity * 2 + 1;
            newCapacity = newCapacity < MIN_ADJUST_CAPACITY ? MIN_ADJUST_CAPACITY : newCapacity;

            _ExpandCapacity(newCapacity);
        }

    public:
        TArray() : m_pData(NULL), m_Length(0), m_Capacity(0)
        {}

        TArray(SIZE_T inCapacity) : m_Length(0), m_Capacity(inCapacity)
        {
            m_pData = (Type*)Malloc::Alloc(sizeof(Type) * inCapacity);
        }

    public:
        Type& operator[] (SIZE_T index)
        {
            if (index < 0 && index >= m_Length)
            {
                //Logger::LogError();
            }
            return ((Type*)m_pData)[index];
        }

        const Type& operator[] (SIZE_T index) const
        {
            if (index < 0 && index >= m_Length)
            {
                //Logger::LogError();
                return ((Type*)m_pData)[0];
            }
            return m_pData[index];
        }

    public:
        void Resize(SIZE_T inNewLength)
        {
            SIZE_T oldLength = m_Length;
            m_Length = inNewLength;
            m_Capacity = m_Length < m_Capacity ? m_Capacity : m_Length;

            if (oldLength < m_Capacity)
            {
                _ExpandCapacity();
            }

            for (SIZE_T i = oldLength; i < inNewLength; ++i)
            {
                Type* newElement = &m_pData[i];
                BladeConstruct(newElement, Type);
            }
        }

        Type& Add(const Type& inValue)
        {
            if (m_Length >= m_Capacity)
            {
                _ExpandCapacity();
            }
            
            Type* newElement = &m_pData[m_Length];
            BladeCopyConstruct(newElement, inValue, Type);
            m_Length = m_Length + 1;

            return m_pData[m_Length - 1];
        }

        Type& Insert(SIZE_T index, const Type& inValue)
        {
            if (m_Length >= m_Capacity)
            {
                _ExpandCapacity();
            }

            Type* element = NULL;
            if (index == m_Length)
            {
                element = &m_pData[m_Length];
                BladeCopyConstruct(element, inValue, Type);
            }
            else if (index < m_Length)
            {
                for (SIZE_T i = m_Length; i > m_Length; --i)
                {
                    element = &m_pData[i];
                    BladeCopyConstruct(element, m_pData[i - 1], Type);
                }
                element = &m_pData[index];
                BladeCopyConstruct(element, inValue, Type);
            }
            m_Length = m_Length + 1;

            return m_pData[index];
        }

        void RemoveAt(SIZE_T index)
        {
            Type* element = NULL;
            for (SIZE_T i = index; i < m_Length - 1; ++i)
            {
                element = &m_pData[i];
                BladeCopyConstruct(element, m_pData[i + 1], Type);
            }
            element = &m_pData[m_Length - 1];
            BladeDestruct(element, Type);
        }

        void Clear()
        {
            for (SIZE_T i = 0; i < m_Length - 1; ++i)
            {
                Type* element = &m_pData[m_Length];
                BladeDestruct(element, Type);
            }
            m_Length = 0;
        }

    public:
        void* VoidPtr() { return m_pData; }
        const void* VoidPtr() const { return m_pData; }

        Type* TypePtr() { return m_pData; }
        const Type* TypePtr() const { return m_pData; }

        SIZE_T GetLength() const { return m_Length; }
    };
}

#endif // !__BLADE_CONTAINS_TARRAY_H__
