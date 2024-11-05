#ifndef _NPQ_INTERFACE_LINKER_H_
#define _NPQ_INTERFACE_LINKER_H_

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "NPQos.h"
#include "NPQDefine.h"

namespace DevEBase {

    struct NPQ_INTERFACE {
        HC_NPQ_Create pNpq_Create;
        HC_NPQ_Destroy pNpq_Destroy;
        HC_NPQ_Start pNpq_Start;
        HC_NPQ_Stop pNpq_Stop;
        HC_NPQ_RegisterDataCallBack pNpq_RegisterDataCallBack;
        HC_NPQ_InputData pNpq_InputData;
        HC_NPQ_InputRawData pNpq_InputRawData;
        HC_NPQ_SetParam pNpq_SetParam;
        HC_NPQ_GetStat pNpq_GetStat;
        HC_NPQ_SetNotifyParam pNpq_SetNotifyParam;
        HC_NPQ_GetVersion pNpq_GetVersion;
    };

    using NPQ_DATACB = void (CALLBACK *)(HPR_INT32, HPR_INT32, unsigned char*, HPR_UINT32, HPR_VOIDPTR);

    class NETSDK_CORE_CLASS CNpqInterface {
    public:
        CNpqInterface();
        ~CNpqInterface();

        static HPR_BOOL LoadNpqLib();
        static HPR_BOOL UnloadNpqLib();

        HPR_BOOL StartNpqService(NPQ_QOS_ROLE enumRole, HPR_UINT32 dwQosType, NPQ_DATACB fnDataCallBack, HPR_VOIDPTR pUser, char *sSdp);
        HPR_BOOL StopNpqService();
        HPR_BOOL InputData(HPR_INT32 iDataType, unsigned char* pData, HPR_UINT32 nDataLen);
        HPR_BOOL InputRawData(LPNET_UTILS_NPQ_PACK_PARAM lpPackParam, unsigned char* pData, HPR_UINT32 nDataLen);
        HPR_BOOL GetState(NPQ_MAIN_TYPE enMainType, NPQ_STAT* lpState);
        HPR_BOOL SetNotifyParam(NPQ_SET_NOTIFY_PARAM* pNotifyParam);
        HPR_BOOL GetVersion(HPR_UINT8 *szVersion);

    private:
        HPR_INT32 ConvertNpqError(HPR_INT32 iError);
        HPR_INT32 m_iNpqHandle;
        HPR_BOOL m_bNPQStart;

        static HPR_INT32 m_iInitCount;
        static HPR_HANDLE m_hNpqLib;
    };

}

NETSDK_CORE_API DevEBase::CNpqInterface* CALLBACK Core_NPQ_Create();
NETSDK_CORE_API HPR_VOID CALLBACK Core_NPQ_Destroy(DevEBase::CNpqInterface *pNPQInterface);

#endif // _NPQ_INTERFACE_LINKER_H_
