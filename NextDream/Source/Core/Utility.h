#ifndef __BLADE_CORE_UTILITY_H__
#define __BLADE_CORE_UTILITY_H__

#include "TypeDefine.h"

namespace BladeEngine
{
	#ifndef NULL
		#define NULL 0
	#endif

	void _BladeAssert_Impl(bool inCondition, const char* inMessage, const char* inFile, int inLine)
	{

	}

	#define BladeAssert(condition) _BladeAssert_Impl(condition, "Condition of assertion couldn't be satisfied!", __FILE__,  __LINE__);
	//#define BladeAssert(condition, message) _BladeAssert_Impl(condition, message, __FILE__,  __LINE__);

	class IRefCountBase
	{
	public:
		virtual uint32 AddRef() = 0;
		virtual uint32 Release() = 0;
		virtual uint32 GetRefCount() = 0;
	};

	struct NotThreadSafeRefCount
	{
	public:
		NotThreadSafeRefCount() : m_iRefCount(0)
		{}

		uint32 AddRef()
		{
			return (++m_iRefCount);
		}

		uint32 Release()
		{
			return (--m_iRefCount);
		}

		uint32 GetRefCount()
		{
			return m_iRefCount;
		}

	private:
		uint32 m_iRefCount;
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
		{}

		RefCountObject(const RefCountObject& rl) : m_pPtr(rl._pPtr)
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

		RefCountObject& operator= (const IRefCountObject& rl)
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