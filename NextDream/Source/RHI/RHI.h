#ifndef __BLADE_RHI_RHI_H__
#define __BLADE_RHI_RHI_H__

#include <Utility.h>
#include <RHIEnum.h>
#include <RHIContext.h>
#include <RHIDevice.h>
#include <RHIShaderBase.h>

namespace BladeEngine
{
    namespace RHI
    {
        class RHICommandList : INoncopyable
        {
        private:
            RHICommandBase m_Root;
            RHICommandBase* m_LastCommand;

        public:
            RHICommandList() : m_LastCommand(&m_Root)
            {}

        public:
            void AddCmdSetShaderState(RHIShaderStateRef& inShaderState)
            {
                RHICmdSetShaderState* cmd = (RHICmdSetShaderState*)SystemMalloc::GetInstance().Alloc(sizeof(RHICmdSetShaderState));
                BladeConstruct(cmd, inShaderState, m_LastCommand);
            }

            void AddCmdSetVertexShader(RHIShaderStateRef& inShaderState)
            {
                RHICmdSetVertexShader* cmd = (RHICmdSetVertexShader*)SystemMalloc::GetInstance().Alloc(sizeof(RHICmdSetVertexShader));
                BladeConstruct(cmd, inShaderState, m_LastCommand);
            }
        };

        class RHICommandBase : public INoncopyable
        {
            friend class RHICommandList;
        private:
            typedef void(*ExecuteFunc)(RHIContextBase* inContext, RHICommandBase* inCmd);
        private:
            ExecuteFunc m_pExecuteFunc;
            RHICommandBase* m_pNext;

        private:
            RHICommandBase() : m_pExecuteFunc(NULL), m_pNext(NULL)
            {}

        protected:
			RHICommandBase(ExecuteFunc inExecuteFunc, RHICommandBase* inParent) :
				m_pExecuteFunc(inExecuteFunc)
			{
				BladeAssert(inParent != NULL && inParent->m_pNext == NULL);
				BladeAssert(inExecuteFunc != NULL);

				inParent->m_pNext = this;
			}

			void Execute(RHIContextBase* inContext, RHICommandBase* inCmd)
			{
				BladeAssert(m_pExecuteFunc != NULL);
				m_pExecuteFunc(inContext, inCmd);
			}
		};

		class RHICmdSetShaderState : public RHICommandBase
		{
        private:
            RHIShaderStateRef m_ShaderState;

		public:
            RHICmdSetShaderState(RHIShaderStateRef& inShaderState, RHICommandBase* inParent) :
                RHICommandBase(Execute, inParent),
                m_ShaderState(inShaderState)
			{
                BladeAssert(!m_ShaderState.IsNull());
			}

		private:
			static void Execute(RHIContextBase* inContext, RHICommandBase* inCmd)
			{
                RHICmdSetShaderState* cmd = (RHICmdSetShaderState*)inCmd;
                inContext->SetShaderState(cmd->m_ShaderState);
			}
		};

        class RHICmdSetVertexShader : public RHICommandBase
        {
        private:
            RHIVertexShaderRef m_VertexShader;

        public:
            RHICmdSetVertexShader(RHIVertexShaderRef& inVertexShader, RHICommandBase* inParent) :
                RHICommandBase(Execute, inParent),
                m_VertexShader(inVertexShader)
            {
                BladeAssert(!m_VertexShader.IsNull());
            }

        private:
            static void Execute(RHIContextBase* inContext, RHICommandBase* inCmd)
            {
                RHICmdSetVertexShader* cmd = (RHICmdSetVertexShader*)inCmd;
                inContext->SetVertexShader(cmd->m_VertexShader);
            }
        };
	}
}

#endif	//! __BLADE_RHI_RHI_H__
