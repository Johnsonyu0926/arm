#ifndef _EBASE_BASE_HANDLEMGR_MEMBERMGRBASE_H_
#define _EBASE_BASE_HANDLEMGR_MEMBERMGRBASE_H_

#include "DevComPublicDef.h"

namespace DevEBase {

class CMemberBase;
class CMemberMgrBasePrivate;

class NET_COM_CLASS CMemberMgrBase {
public:
    explicit CMemberMgrBase(HPR_UINT32 uiTotalNum);
    virtual ~CMemberMgrBase();
    virtual HPR_BOOL CheckResource() const;

    CMemberBase* GetMember(HPR_INT32 iIndex);
    HPR_BOOL IsFull();

    HPR_BOOL LockMember(HPR_INT32 iIndex) const;
    HPR_BOOL UnlockMember(HPR_INT32 iIndex) const;
    HPR_BOOL TryLockMember(HPR_INT32 iIndex) const;

    HPR_BOOL ReadLockMember(HPR_INT32 iIndex) const;
    HPR_BOOL ReadUnlockMember(HPR_INT32 iIndex) const;

    HPR_UINT32 GetMemberNum() const;
    HPR_UINT32 GetMaxMemberNum() const;

    virtual void* NewMemoryObject(const void *pCond) = 0;

protected:
    HPR_INT32 AllocIndex(CMemberBase *pMember);
    HPR_INT32 AllocIndex(const void *pCond, void * pParams);
    HPR_BOOL  FreeIndex(HPR_INT32 iIndex, HPR_BOOL bCallStop = HPR_TRUE);

private:
    CMemberMgrBasePrivate* m_pPrivate;
};

} // namespace DevEBase

#endif //_EBASE_BASE_HANDLEMGR_MEMBERMGRBASE_H_
