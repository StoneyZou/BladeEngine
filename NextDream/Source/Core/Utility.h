#ifndef __BLADE_CORE_UTILITY_H__
#define __BLADE_CORE_UTILITY_H__

#include <new>
#include <type_traits>
#include <TypeDefine.h>
#include <MacroDefine.h>

namespace BladeEngine
{
    void _BladeAssert_Impl(bool inCondition, const char* inMessage, const char* inFile, int inLine);

	#define BladeAssert(condition) _BladeAssert_Impl(condition, "Condition of assertion couldn't be satisfied!", __FILE__,  __LINE__);
	//#define BladeAssert(condition, message) _BladeAssert_Impl(condition, message, __FILE__,  __LINE__);

    struct _BladeConstruct
    {
        template<typename Type>
        _BladeConstruct(Type* ptr)
        {
            new((void*)ptr)Type();
        }

        template<typename Type, typename P1>
        _BladeConstruct(Type* ptr, P1 p1)
        {
            new((void*)ptr)Type(p1);
        }

        template<typename Type, typename P1, typename P2>
        _BladeConstruct(Type* ptr, P1 p1, P2 p2)
        {
            new((void*)ptr)Type(p1);
        }

        template<typename Type, typename P1, typename P2, typename P3>
        _BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3)
        {
            new((void*)ptr)Type(p1, p2, p3);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4>
        _BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            new((void*)ptr)Type(p1, p2, p3, p4);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5>
        _BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
        _BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5, p6);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
        _BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5, p6, p7);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
        _BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5, p6, p7, p8);
        }

        template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
        _BladeConstruct(Type* ptr, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
        {
            new((void*)ptr)Type(p1, p2, p3, p4, p5, p6, p7, p8, p9);
        }
    };

    struct _BladeCopyConstruct
    {
        template<typename Type>
        _BladeCopyConstruct(Type* ptr, const Type& value)
        {
            new((void*)ptr)Type(value);
        }
    };

    struct _BladeDestruct
    {
        template<typename Type>
        _BladeDestruct(Type* ptr)
        {
            ptr->~Type();
        }
    };

    #define BladeCopyConstruct(ptr, value, type) _BladeCopyConstruct ptr##_BladeCopyConstruct(ptr, value);
    #define BladeDestruct(ptr, type) _BladeDestruct ptr##_BladeDestruct(ptr);
    #define BladeConstruct(ptr, type) _BladeConstruct ptr##_BladeConstruct (ptr);

#if defined UNICODE
    #define TEXT(str) L##str

    struct INoncopyable
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
        virtual ~IReferencable() {};
        
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

    protected:
        ReferenceType* m_pPtr;

    public:
        RefCountObject() : m_pPtr(NULL)
        {}

        RefCountObject(ReferenceType* inPtr) : m_pPtr(inPtr)
        {
            if (m_pPtr != NULL) { m_pPtr->AddRef(); }
        }

        RefCountObject(const RefCountObject& rh) : m_pPtr(const_cast<ReferenceType*>(rh.m_pPtr))
        {
            if (m_pPtr != NULL) { m_pPtr->AddRef(); }
        }

        template<typename Other>
        RefCountObject(const RefCountObject<Other>& rh) : m_pPtr(const_cast<ReferenceType*>((const ReferenceType*)rh.GetReferencePtr()))
        {
            if (m_pPtr != NULL) { m_pPtr->AddRef(); }
        }

        virtual ~RefCountObject()
        {
            if (m_pPtr != NULL) { m_pPtr->Release(); }
        }

        RefCountObject& operator = (const RefCountObject& rl)
        {
            if (rl.m_pPtr == m_pPtr)
            {
                return *this;
            }

            if (m_pPtr != NULL)
            {
                m_pPtr->Release();
            }
            rl.m_pPtr->AddRef();

            m_pPtr = rl.m_pPtr;
            return *this;
        }

        RefCountObject& operator = (TYPE_OF_NULLPTR) { m_pPtr = NULL; return *this; }

        RefCountObject& operator = (TYPE_OF_NULL inPtr) { BladeAssert(inPtr == 0); m_pPtr = NULL; return *this; }

        template<typename Other>
        operator RefCountObject<Other>()
        {
            return RefCountObject<Other>(static_cast<Other*>(m_pPtr));
        }

		ReferenceType& operator* () { return *m_pPtr; }
		const ReferenceType& operator* () const { return *m_pPtr; }

		ReferenceType* operator-> () { return m_pPtr; }
		const ReferenceType* operator-> () const { return m_pPtr; }

		ReferenceType* GetReferencePtr() { return m_pPtr; }
		const ReferenceType* GetReferencePtr() const { return m_pPtr; }

        bool operator == (const RefCountObject& rh) const { return m_pPtr == rh.m_pPtr; }

        bool operator != (const RefCountObject& rh) const { return m_pPtr != rh.m_pPtr; }

        bool operator == (TYPE_OF_NULLPTR) const { return m_pPtr == NULL; }

        bool operator != (TYPE_OF_NULLPTR) const { return m_pPtr != NULL; }

        bool operator == (TYPE_OF_NULL inPtr) const { BladeAssert(inPtr == 0); return m_pPtr == NULL; }

        bool operator != (TYPE_OF_NULL inPtr) const { BladeAssert(inPtr == 0); return m_pPtr != NULL; }
	};

    #define countof(arr) sizeof(arr) / sizeof(arr[0])
}

#endif	// !__BLADE_CORE_UTILITY_H__