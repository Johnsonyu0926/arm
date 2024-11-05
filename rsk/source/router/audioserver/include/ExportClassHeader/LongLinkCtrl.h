#ifndef _DEVCOM_EBASE_TRANMIT_LONGLINKCTRL_H_
#define _DEVCOM_EBASE_TRANMIT_LONGLINKCTRL_H_

#include "DevComPublicDef.h"
using namespace std;

namespace DevEBase {
    
    class CLongLinkCtrlPrivate;

    typedef bool(*LongCfgRecvDataCB)(void* pUserdata, const void* pRecvdata, uint32_t nDatalen, uint32_t nStatus);

    //虚基类，长连接封装，只涉及多线程操作，数据收发接口的定义
    class CLongLinkCtrl {
    public:
        CLongLinkCtrl();
        virtual ~CLongLinkCtrl();

        //连接相关操作
        virtual bool CreateLink(const CONNECT_COND* pConnectCond);
        virtual bool Stop();
        bool HasCreateLink() const;

        //线程相关操作
        bool StartRecvThread(LongCfgRecvDataCB fRecvCallBack, void* pParam);
        void StopRecvThread();
        void ExitRecvThread();
        bool EnableRecvCallBack(bool bRecvCallBack);
        bool ResumeRecvThread();
        void SuspendRecvThread();

        int32_t SendData(const uint8_t* pBuff, uint32_t dwBuffLen, uint32_t dwTimeOut);
        bool GetLocalIP(void* pStru, uint32_t dwSize) const;
        bool GetSocket(void* pStru, uint32_t dwSize) const;

    protected:
        void CheckFreezeThread();
        bool NeedAllocRecvBuffer() const;
        void FreezeThread();
        void UnfreezeThread();

        static void* CALLBACK RecvThreadCallbackStatic(void* pParam);
        virtual void RecvThreadCallback();
        bool CloseLink();
        void DestroyLink();

        bool CheckResource() const;

        CLongLinkCtrlPrivate* m_pPrivate;
    };

}

#endif // !_DEVCOM_EBASE_TRANMIT_LONGLINKCTRL_H_
