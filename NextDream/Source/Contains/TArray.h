#ifndef __BLADE_CONTAINS_TARRAY_H__
#define __BLADE_CONTAINS_TARRAY_H__

#include <TypeDefine.h>

namespace BladeEngine
{
    template<typename Type>
    class TArray : public INoncopyable
    {
    private:
        SIZE_T m_Length;
        void* m_pData;

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
            return ((Type*)m_pData)[index];
        }

        void* VoidPtr() { return m_pData; }
        const void* VoidPtr() const { return m_pData; }

        Type* TypePtr() { return (Type*)m_pData; }
        const Type* TypePtr() { return (Type*)m_pData; }

        SIZE_T Length() const { return m_Length; }
    };
}

#endif // !__BLADE_CONTAINS_TARRAY_H__
