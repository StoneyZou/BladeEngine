#ifndef __BLADE_RHI_RHI_H__
#define __BLADE_RHI_RHI_H__

#include "Utility.h"
#include "RHIEnum.h"

namespace BladeEngine
{
	namespace RHI
	{
		class RHICommandList : INoncopyable
		{
		private:
			RHICommandBase m_Root;

		public:

		};

		class RHICommandBase : public INoncopyable
		{
			friend class RHICommandList;
		private:
			typedef void(*ExecuteFunc)(bool inImmediate, RHICommandBase* inCmd);
		private:
			ExecuteFunc m_pExecuteFunc;
			RHICommandBase* m_pNext;

		private:
			RHICommandBase() : m_pExecuteFunc(NULL), m_pNext(NULL)
			{}

		public:
			RHICommandBase(ExecuteFunc inExecuteFunc, RHICommandBase* inParent) :
				m_pExecuteFunc(inExecuteFunc)
			{
				BladeAssert(inParent != NULL && inParent->m_pNext == NULL);
				BladeAssert(inExecuteFunc != NULL);

				inParent->m_pNext = this;
			}

			void Execute(bool inImmediate, RHICommandBase* inCmd)
			{
				BladeAssert(m_pExecuteFunc != NULL);
				m_pExecuteFunc(inImmediate, inCmd);
			}

			RHICommandBase* GetNextCommandPtr()
			{
				return m_pNext;
			}
		};

		class RHISetShaderStateCmd : public RHICommandBase
		{
		public:
			RHISetShaderStateCmd(RHICommandBase* inParent) : RHICommandBase()
			{

			}

		private:
			static void Execute(bool inImmediate, RHICommandBase* inCmd)
			{
				RHISetShaderStateCmd* tCmd = (RHISetShaderStateCmd*)inCmd;
			}
		};
	}
}

#endif	//! __BLADE_RHI_RHI_H__
