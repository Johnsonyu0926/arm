#ifndef _HCDEVEBASE_UTILS_UTILSFUNCTION_H_ 
#define _HCDEVEBASE_UTILS_UTILSFUNCTION_H_

#include "HPR/HPR_Config.h"
#include "HPR/HPR_Types.h"
#include "cJSON/IoTcJSON.h"

#ifndef maxum
#define maxum(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef minum
#define minum(a,b)            (((a) < (b)) ? (a) : (b))
#endif


namespace DevEBase
{
    #define HTTP_EHOMESESSION_LEN   32
    HPR_VOID EBase_HexDump(unsigned const char* pSrc, int iLen, int iUpper, unsigned char* pDst);

    HPR_BOOL DomainParse(const char *pInBuf, char *pOutBuf);

    template <typename T>
    HPR_VOID ZeroValue(T& dst)
    {
        memset(&dst, 0, sizeof(T));
    }

    template <typename T>
    inline HPR_VOID CopyValue(T& dst, const T& src)
    {
        memcpy(&dst, &src, sizeof(T));
    }

    template <typename T>
    inline HPR_BOOL IsValueEqual(const T& dst, const T& src)
    {
        return (memcmp(&dst, &src, sizeof(T)) == 0);
    }

    template <typename FT, typename ST>
    inline HPR_VOID CopyNValueByptr(FT *dst, const ST *src)
    {
        memcpy(dst, src, minum(sizeof(FT), sizeof(ST)));
    }

    HPR_VOIDPTR NewArray(HPR_UINT32 dwSize);

    HPR_VOID DelArray(HPR_VOIDPTR pAddress);

    HPR_BOOL IsEndWith(const char *pSrcBuff, HPR_UINT32 dwSrcBuffLen, const char* pDstBuff, HPR_UINT32 dwDstBuff);

    HPR_BOOL IsBeginWithURIMethod(const char *pSrcBuff, HPR_UINT32 dwSrcBuffLen);

    const char* Utils_StrnStr(const char* pSrcBuff, const char* pDstBuff, HPR_UINT32 dwSrcBuffLen);

    HPR_BOOL GetMiddleString(const char *pInBuff, HPR_UINT32 dwInLen, const char *pPreStr, const char *pSufStr, char *pOutBuf, HPR_UINT32 dwOutLen);

    HPR_BOOL SetMiddleString(const char *pInBuff, HPR_UINT32 dwInLen, const char *pPreStr, const char *pInsertStr, char *pOutBuf, HPR_UINT32* pOutLen,HPR_BOOL bFlag);

    HPR_VOID GenerateRandom(char* pRandom);

    //去除前后空格
    HPR_BOOL DelBlankSpace(char *pBuf, HPR_UINT32 dwBufLen); 

    //严格来说，本方法并不严谨，因为并不是按照递归的方式一级级排查的，实际上会出错，但是就目前来说够用了
    HPR_BOOL GetJSonValueByJSonItem(const char *pInBuff, HPR_UINT32 dwInLen, const char *pJSonItemName, char *pOutBuf, HPR_UINT32 dwOutLen, HPR_BOOL bString);

    HPR_BOOL GetJsonItemString(IoTcJSON *pNode, const char *pItem, char *pBuf, const HPR_UINT32 dwBufLen);
        
    //模板函数，不能获取浮点数值，支持整型和bool类型
    template <typename T>
    inline HPR_BOOL GetJsonItemInt(IoTcJSON *pNode, const char *pItem, T &var)
    {
        IoTcJSON *pNodeNode = IoTcJSON_GetObjectItem(pNode, pItem);
        if (!pNodeNode)
        {
            return HPR_FALSE;
        }
        if (pNodeNode->type == IoTcJSON_Number)
        {
            var = (T)pNodeNode->valueint;
        }
        else if (pNodeNode->type == IoTcJSON_True)
        {
            var = HPR_TRUE;
        }
        else if (pNodeNode->type == IoTcJSON_False)
        {
            var = HPR_FALSE;
        }
        else
        {
            return HPR_FALSE;
        }
        return HPR_TRUE;
    }
}


#endif //_HCDEVEBASE_UTILS_UTILSFUNCTION_H_