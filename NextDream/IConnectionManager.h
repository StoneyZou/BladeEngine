#ifndef __BLADE_CORE_INTERFACE_ICONNECTION_MANAGER_H__
#define __BLADE_CORE_INTERFACE_ICONNECTION_MANAGER_H__

#include <TypeDefine.h>
#include <TArray.h>

namespace BladeEngine
{
    struct SimpleTypeParam
    {
        union
        {
            bool        bool_value;
            uint8       uint8_value;
            uint16      uint16_value;
            uint32      uint32_value;
            uint64      uint64_value;
            int8        int8_value;
            int16       int16_value;
            int32       int32_value;
            int64       int64_value;
            byte        byte_value;
            ANSICHAR    ansi_ch;
            WIDECHAR    wide_ch;
            TCHAR       ch;
            ANSICHAR*   ansi_str;
            WIDECHAR*   wide_str;
            TCHAR*      str;
            const ANSICHAR*     ansi_cstr;
            const WIDECHAR*     wide_cstr;
            const TCHAR*        cstr;
        };
    };

    typedef void (*OnServntStateChanged) (IMaster*, IServnt*, SimpleTypeParam, void*);

    class IMaster
    {
    private:
        TArray<IContractMgr*> m_ContractMgrList;
        TArray<uint32> m_ContractMgrCntList;

    public:
        virtual void AddContract(IContractMgr* inMgr, IServnt* inServnt)
        {
            bool isContractMgrExist = false;
            for (uint32 i = 0; i < m_ContractMgrList.Size(); ++i)
            {
                if (m_ContractMgrList[i] == inMgr)
                {
                    isContractMgrExist = true;
                    m_ContractMgrCntList[i] = m_ContractMgrCntList[i] + 1;
                }
            }

            if(!isContractMgrExist)
            {
                m_ContractMgrList.Add(inMgr);
                m_ContractMgrCntList.Add(1);
            }
        }

        virtual void RemoveContract(IContractMgr* inMgr, IServnt* inServnt)
        {
            int32 idxContractMgrRemove = -1;
            for (uint32 i = 0; i < m_ContractMgrList.Size(); ++i)
            {
                if (m_ContractMgrList[i] == inMgr)
                {
                    if(m_ContractMgrCntList[i] != 1)
                    {
                        m_ContractMgrCntList[i] = m_ContractMgrCntList[i] - 1;
                    }
                    else
                    {
                        idxContractMgrRemove = i;
                    }
                }
            }

            if (idxContractMgrRemove != -1)
            {
                m_ContractMgrList.RemoveAt(idxContractMgrRemove);
                m_ContractMgrCntList.RemoveAt(idxContractMgrRemove);;
            }
        }
    };

    struct IServnt
    {
    private:
        TArray<IContractMgr*> m_ContractMgrList;
        TArray<uint32> m_ContractMgrCntList;

    public:
        virtual void AddContract(IContractMgr* inMgr, IMaster* inMaster)
        {
            bool isContractMgrExist = false;
            for (uint32 i = 0; i < m_ContractMgrList.Size(); ++i)
            {
                if (m_ContractMgrList[i] == inMgr)
                {
                    isContractMgrExist = true;
                    m_ContractMgrCntList[i] = m_ContractMgrCntList[i] + 1;
                }
            }

            if (!isContractMgrExist)
            {
                m_ContractMgrList.Add(inMgr);
                m_ContractMgrCntList.Add(1);
            }
        }

        virtual void RemoveContract(IContractMgr* inMgr, IMaster* inMaster)
        {
            int32 idxContractMgrRemove = -1;
            for (uint32 i = 0; i < m_ContractMgrList.Size(); ++i)
            {
                if (m_ContractMgrList[i] == inMgr)
                {
                    if (m_ContractMgrCntList[i] != 1)
                    {
                        m_ContractMgrCntList[i] = m_ContractMgrCntList[i] - 1;
                    }
                    else
                    {
                        idxContractMgrRemove = i;
                    }
                }
            }

            if (idxContractMgrRemove != -1)
            {
                m_ContractMgrList.RemoveAt(idxContractMgrRemove);
                m_ContractMgrCntList.RemoveAt(idxContractMgrRemove);;
            }
        }
    };

    struct IContractMgr
    {
        virtual void MakeContract(IMaster* inMaster, IServnt* inServnt, OnServntStateChanged inHandle);
        virtual void BreakContract(IMaster* inMaster, IServnt* inServnt);
        virtual void BreakAllContract(IServnt* inServnt);
        virtual void BreakAllContract(IMaster* inMaster);
    };
}

#endif  // !__BLADE_CORE_INTERFACE_ICONNECTION_MANAGER_H__