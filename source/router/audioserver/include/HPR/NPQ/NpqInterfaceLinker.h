/**  @file    NpqInterfaceLinker.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief NPQ接口动态链接接口定义
 *   
 *   @author zhaolin
 *   @modify zhaolin
 *   @date   2016/12/14
 *   
 *   @note:
 */

#ifndef _NPQ_INTERFACE_LINKER_H_
#define _NPQ_INTERFACE_LINKER_H_

#include "HPR_Config.h"
#include "HPR_Types.h"
//#include "Core/ComInterfaceCore.h"
//#include "Core/ProtocolDef.h"
//#include "core/ProtocolMacroDef.h"
#include "NPQos.h"
//#include "Base/Global/NetUtilGlobal.h"
#include "NPQDefine.h"





namespace DevEBase
{
    typedef struct _NPQ_INTERFACE
    {
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
    }NPQ_INTERFACE, *LPNPQ_INTERFACE;

    typedef void (CALLBACK *NPQ_DATACB)(HPR_INT32 id, HPR_INT32 iDataType, unsigned char* pData, HPR_UINT32 nDataLen, HPR_VOIDPTR pUser);

    class NETSDK_CORE_CLASS CNpqInterface
    {
    public:
        CNpqInterface();
        ~CNpqInterface();

        static HPR_BOOL LoadNpqLib();
        static HPR_BOOL UnloadNpqLib();

        HPR_BOOL StartNpqService(NPQ_QOS_ROLE enumRole, HPR_UINT32 dwQosType, NPQ_DATACB fnDataCallBack, HPR_VOIDPTR pUser, char *sSdp);
        HPR_BOOL StopNpqService();
        //InputData输入的是RTP或者RTCP数据
        //InputRawData输入的是裸数据，并且需要按帧输入，内部进行RTP打包
        HPR_BOOL InputData(HPR_INT32 iDataType, unsigned char* pData, HPR_UINT32 nDataLen);
        HPR_BOOL InputRawData(LPNET_UTILS_NPQ_PACK_PARAM lpPackParam, unsigned char* pData, HPR_UINT32 nDataLen);

        HPR_BOOL GetState(NPQ_MAIN_TYPE enMainType, NPQ_STAT* lpState);

        HPR_BOOL SetNotifyParam(NPQ_SET_NOTIFY_PARAM* pNotifyParam);

        HPR_BOOL GetVersion(HPR_UINT8 *szVersion);

    private:
        HPR_INT32 ConvertNpqError(HPR_INT32 iError);
        HPR_INT32 m_iNpqHandle;
        HPR_BOOL m_bNPQStart;

        static HPR_INT32 m_iInitCount;  //初始化次数
        static HPR_HANDLE m_hNpqLib;


    };

}

NETSDK_CORE_API CNpqInterface* CALLBACK Core_NPQ_Create();
NETSDK_CORE_API HPR_VOID CALLBACK Core_NPQ_Destroy(CNpqInterface *pNPQInterface);



#endif //_NPQ_INTERFACE_LINKER_H_
