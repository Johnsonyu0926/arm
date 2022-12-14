/**  @file   MemberMgrBase.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief 
 *   
 *   @author shijianfengyf2
 *   @modify shijianfengyf2
 *   @date   2018/09/12
 *   
 *   @note:
 */
#ifndef _EBASE_BASE_HANDLEMGR_MEMBERMGRBASE_H_
#define _EBASE_BASE_HANDLEMGR_MEMBERMGRBASE_H_

#include "DevComPublicDef.h"

namespace DevEBase
{
    class CMemberBase;
    class CMemberMgrBasePrivate;
    /**    @class CMemberMgrBase
    *     @brief 句柄管理基类
    *     1.为了提高响应速度，各个成员相互影响减到最小，每个成员都拥有一把锁。且创建以后除非SDK cleanup，否则不销毁。这点，和4.0保持一致。
    *     2.在释放成员时，必须要有把锁全程锁上。
    *     3.!!!其余组件不从这个类派生，从CModuleMgrBase派生
    */
    class NET_COM_CLASS CMemberMgrBase
    {
    public:
        CMemberMgrBase(HPR_UINT32 uiTotalNum);
        virtual ~CMemberMgrBase();
        virtual HPR_BOOL CheckResource() const;      //由每个模块dll的初始化函数进行验证。每个外部接口都要验证是否初始化

        CMemberBase* GetMember(HPR_INT32 iIndex);
        HPR_BOOL IsFull();

        //写锁动作
        HPR_BOOL LockMember(HPR_INT32 iIndex) const;
        HPR_BOOL UnlockMember(HPR_INT32 iIndex) const;
        HPR_BOOL TryLockMember(HPR_INT32 iIndex) const;
        //读锁动作
        HPR_BOOL ReadLockMember(HPR_INT32 iIndex) const;
        HPR_BOOL ReadUnlockMember(HPR_INT32 iIndex) const;

        HPR_UINT32 GetMemberNum();//获取已有成员数目
        HPR_UINT32 GetMaxMemberNum();//获取对大成员数目

        virtual void* NewMemoryObject(const void *pCond) = 0;
    protected:

        HPR_INT32 AllocIndex(CMemberBase *pMember);
        //pCond用于New成员的条件，pParams用于开始时的入口参数
        HPR_INT32 AllocIndex(const void *pCond, void * pParams);
        HPR_BOOL  FreeIndex(HPR_INT32 iIndex, HPR_BOOL bCallStop = HPR_TRUE);

    private:
        CMemberMgrBasePrivate* m_pPrivate;
    };
}
#endif //_EBASE_BASE_HANDLEMGR_MEMBERMGRBASE_H_
