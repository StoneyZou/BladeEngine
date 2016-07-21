#ifndef __BLADE_CORE_UTILITY_H__
#define __BLADE_CORE_UTILITY_H__

#include "TypeDefine.h"

namespace BladeEngine
{
	void _BladeAssert_Impl(bool inCondition, const char* inMessage, const char* inFile, int inLine)
	{

	}

	#define BladeAssert(condition) _BladeAssert_Impl(condition, "Condition of assertion couldn't be satisfied!", __FILE__,  __LINE__);
	//#define BladeAssert(condition, message) _BladeAssert_Impl(condition, message, __FILE__,  __LINE__);

    struct BladeConstruct
    {
        template<typename Type>
        BladeConstruct(Type* ptr)
        {
            new((void*)ptr)Type();
        }

        template<typename Type, typename P1>
        BladeConstruct(Type* ptr, P1 p1)
        {
            new((void*)ptr)Type(p1);
        }

        template<typename Type, typename P1, typename P2>
        BladeConstruct(Type* ptr, P1 p1, P2 p2)
        {
            new((void*)ptr)Type(p1);
        }

        template<typename Type, typename P1, typename P2, typename P3>
        BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3)
        {
            new((void*)ptr)Type(p1, p2, p3);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4>
        BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            new((void*)ptr)Type(p1, p2, p3, p4);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5>
        BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
        BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5, p6);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
        BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5, p6, p7);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
        BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5, p6, p7, p8);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
        BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5, p6, p7, p8, p9);
        }
    };

    #define BladeCopyConstruct(ptr, value, type) new((void*)ptr)type(value);
    #define BladeDestruct(ptr, type) ((type*)ptr)->~type();

    class INoncopyable
    {
    protected:
        INoncopyable() {}

    private:
        INoncopyable(const INoncopyable&);
        INoncopyable& operator = (const INoncopyable&);
    };

	struct NotThreadSafeRefCount
	{
	public:
		NotThreadSafeRefCount() : m_iRefCount(0)
		{}

		int32 AddRef() const
		{
			return (++m_iRefCount);
		}

		int32 Release() const
		{
			return (--m_iRefCount);
		}

		int32 GetRefCount() const
		{
			return m_iRefCount;
		}

	private:
		mutable int32 m_iRefCount;
	};

    class IReferencable
    {
    private:
        NotThreadSafeRefCount m_RefCount;

    protected:
        ~IReferencable();
        
    public:
        int32 AddRef() const { return m_RefCount.AddRef(); }
        int32 Release() const { return m_RefCount.Release(); }
        int32 GetRefCount() const { return m_RefCount.GetRefCount(); }
    };

    template<typename T>
    class RefCountObject
    {
    public:
    typedef T ReferenceType;

    private:
        ReferenceType* m_pPtr;

    public:
        RefCountObject() : m_pPtr(NULL)
        {}

        RefCountObject(ReferenceType* inPtr) : m_pPtr(m_pPtr)
        {
            m_pPtr->AddRef();
        }

        RefCountObject(const RefCountObject& rl) : m_pPtr(rl.m_pPtr)
		{
			rl.m_pPtr->AddRef();
		}

        template<typename Other>
        RefCountObject(const RefCountObject<Other>& rl) : m_pPtr(rl.GetReferencePtr())
        {
            rl.m_pPtr->AddRef();
        }

		~RefCountObject()
		{
			if (m_pPtr != NULL)
			{
				m_pPtr->Release();
			}
		}

		RefCountObject& operator = (const RefCountObject& rl)
		{
			if (rl.m_pPtr == m_pPtr)
			{
				return;
			}

			if (m_pPtr != NULL)
			{
				m_pPtr->Release();
			}
			rl.m_pPtr->AddRef();

			m_pPtr = rl.m_pPtr;
		}

        template<typename Other>
        operator RefCountObject<Other>()
        {
            return RefCountObject<Other>(static_cast<Other>(m_pPtr));
        }

        template<typename Other>
        operator RefCountObject<Other>() const
        {
            return RefCountObject<Other>(static_cast<Other>(m_pPtr));
        }

		ReferenceType& operator* () { return *m_pPtr; }
		const ReferenceType& operator* () const { return *m_pPtr; }

		ReferenceType* operator-> () { return m_pPtr; }
		const ReferenceType* operator-> () const { return m_pPtr; }

		ReferenceType* GetReferencePtr() { return m_pPtr; }
		const ReferenceType* GetReferencePtr() const { return m_pPtr; }

		bool IsNull() { return m_pPtr == NULL; }
	};
}

#endif	// !__BLADE_CORE_UTILITY_H__