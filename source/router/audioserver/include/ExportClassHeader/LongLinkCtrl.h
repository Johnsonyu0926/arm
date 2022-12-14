/**  @file   LongLinkCtrl.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief 
 *   
 *   @author shijianfengyf2
 *   @modify shijianfengyf2
 *   @date   2018/09/06
 *   
 *   @note:
 */
#ifndef _DEVCOM_EBASE_TRANMIT_LONGLINKCTRL_H_
#define _DEVCOM_EBASE_TRANMIT_LONGLINKCTRL_H_

#include "DevComPublicDef.h"

namespace DevEBase
{
  //  class CLinkBase;
    class CLongLinkCtrlPrivate;

    typedef HPR_BOOL(*LongCfgRecvDataCB)(HPR_VOIDPTR pUserdata, void const *pRecvdata, HPR_UINT32 nDatalen, HPR_UINT32 nStatus);

    //虚基类，长连接封装，只涉及多线程操作，数据收发接口的定义
    class NET_COM_CLASS CLongLinkCtrl
    {
    public:
        CLongLinkCtrl();
        virtual ~CLongLinkCtrl();

        //---------------连接相关操作-----------------
        virtual HPR_BOOL CreateLink(const CONNECT_COND *pConnectCond);
        virtual HPR_BOOL Stop();
        HPR_BOOL HasCreateLink() const;

        //---------------线程相关操作-----------------
        HPR_BOOL StartRecvThread(LongCfgRecvDataCB fRecvCallBack, HPR_VOIDPTR pParam);
        //停止线程
        HPR_VOID StopRecvThread();
        //这个函数只对m_bExitRecvThread进行赋值，不会等待接收线程退出。使用环境，当上层回调函数中发现底层接收数据有错误时，调用该接口来结束接收线程。因为StopRecvThread
        //函数中会等待接收线程退出，如果在上层的回调函数中来调用，会导致死锁。所以增加了ExitRecvThread这个函数
        HPR_VOID ExitRecvThread();
        //回调函数使能
        HPR_BOOL EnableRecvCallBack(HPR_BOOL bRecvCallBack);
        HPR_BOOL ResumeRecvThread();
        HPR_VOID SuspendRecvThread();

        HPR_INT32 SendData(const HPR_UINT8* pBuff, HPR_UINT32 dwBuffLen, HPR_UINT32 dwTimeOut);
        HPR_BOOL GetLocalIP(void *pStru, HPR_UINT32 dwSize) const;
        HPR_BOOL GetSocket(void *pStru, HPR_UINT32 dwSize) const;

    protected:
        HPR_VOID CheckFreezeThread(HPR_VOID);
        HPR_BOOL NeedAllocRecvBuffer(HPR_VOID) const;
        HPR_VOID FreezeThread(HPR_VOID);
        HPR_VOID UnfreezeThread(HPR_VOID);

        //接收数据线程
        static HPR_VOIDPTR CALLBACK RecvThreadCallbackStatic(HPR_VOIDPTR pParam);
        virtual void RecvThreadCallback();
        HPR_BOOL CloseLink();
        HPR_VOID DestroyLink();

        HPR_BOOL CheckResource() const;

        CLongLinkCtrlPrivate *m_pPrivate;
    };
}

#endif // !_DEVCOM_EBASE_TRANMIT_LONGLINKCTRL_H_
