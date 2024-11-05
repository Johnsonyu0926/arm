#ifndef _EBASE_BASE_HANDLEMGR_MEMBERBASE_H_
#define _EBASE_BASE_HANDLEMGR_MEMBERBASE_H_

#include "DevComPublicDef.h"

namespace DevEBase {

class CMemberBasePrivate {
public:
    friend class CMemberBase;
    CMemberBasePrivate() : m_iMemberIndex(-1) {}

    ~CMemberBasePrivate() = default;

private:
    HPR_INT32 m_iMemberIndex;
};

class NET_COM_CLASS CMemberBase {
public:
    CMemberBase();
    virtual ~CMemberBase();
    virtual HPR_BOOL CheckResource();

    virtual HPR_BOOL Start(void *pParams) = 0;
    virtual HPR_BOOL Stop() = 0;

    HPR_BOOL SetMemberIndex(HPR_INT32 iMemberIndex);
    HPR_INT32 GetMemberIndex() const;

private:
    CMemberBasePrivate* m_pPrivate;
};

} // namespace DevEBase

#endif //_EBASE_BASE_HANDLEMGR_MEMBERBASE_H_
