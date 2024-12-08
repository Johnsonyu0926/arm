/**  @file   CtrlBase.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief  控制类基类 
 *   
 *   @author shijianfengyf2
 *   @modify shijianfengyf2
 *   @date   2018/08/21
 *   
 *   @note:
 */

#ifndef _EBASE_EXPORTCLASS_CTRLBASE_H_
#define _EBASE_EXPORTCLASS_CTRLBASE_H_

#include "DevComPublicDef.h"

namespace DevEBase
{
#define MAX_SESSION_MGR_NUM  32

    //前置生命
    class CCtrlBasePrivate;

    /**   @class CCtrlBase
    *    @brief 全局控制基类。该类的子类都是单一实例
    */
    class NET_COM_CLASS CCtrlBase
    {
    public:
        CCtrlBase();
        virtual ~CCtrlBase();

        HPR_BOOL Init();                                //带计数功能，初始化相关资源
        HPR_BOOL Fini();                                //带计数功能，反初始化相关资源

        HPR_BOOL CheckInit();                           //检查资源: 成功时，使用计数会+1

    protected:
        virtual HPR_BOOL InitAllResource() = 0;         //初始化所有需要资源
        virtual HPR_VOID CleanupAllResource() = 0;      //清理掉所有资源
        virtual HPR_BOOL CheckConstructResource() const;//检查构造函数里需要动态创建的资源是否创建成功了

        HPR_BOOL Lock();
        HPR_BOOL UnLock();

        CCtrlBasePrivate* m_pPrivate;
    };
}

#endif //_EBASE_EXPORTCLASS_CTRLBASE_H_

