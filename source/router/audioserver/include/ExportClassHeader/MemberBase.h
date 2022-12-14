/**  @file   MemberBase.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief 
 *   
 *   @author shijianfengyf2
 *   @modify shijianfengyf2
 *   @date   2018/09/12
 *   
 *   @note:
 */
#ifndef _EBASE_BASE_HANDLEMGR_MEMBERBASE_H_
#define _EBASE_BASE_HANDLEMGR_MEMBERBASE_H_

#include "DevComPublicDef.h"

namespace DevEBase
{
    class NET_COM_CLASS CMemberBasePrivate
    {
    public:
        friend class CMemberBase;
        CMemberBasePrivate()
        {
            m_iMemberIndex = -1;
        }

        ~CMemberBasePrivate(){}
    private:
        HPR_INT32 m_iMemberIndex;
    };

    /**    @class CMemberBase
    *    @brief 
    */
    class NET_COM_CLASS CMemberBase
    {
    public:
        CMemberBase();
        virtual ~CMemberBase();
        virtual HPR_BOOL CheckResource();

        virtual HPR_BOOL Start(void *pParams) = 0;
        virtual HPR_BOOL Stop() = 0;

        HPR_BOOL SetMemberIndex(HPR_INT32 iMemberIndex);
        HPR_INT32 GetMemberIndex();

    private:
        CMemberBasePrivate* m_pPrivate;
    };
}

#endif //_EBASE_BASE_HANDLEMGR_MEMBERBASE_H_
