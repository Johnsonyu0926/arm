#ifndef __DOORS_DEFS_H__
#define __DOORS_DEFS_H__

#include <cstdint>
#include <atomic>
#include <cstring>

// 类型定义
using LONGLONG = int64_t;
using ULONG = unsigned long;
using INT32 = int32_t;
using INT = int;
using UINT = unsigned int;
using DWORD = unsigned long;
using BOOL = int;
using BYTE = unsigned char;
using WORD = unsigned short;
using SHORT = short;
using USHORT = unsigned short;
using FLOAT = float;
using LONG = long;
using LPBOOL = BOOL*;
using LPBYTE = BYTE*;
using LPWORD = WORD*;
using LPDWORD = DWORD*;
using LPINT32 = INT32*;
using LPINT = INT*;
using VOID = void;
using LPVOID = void*;
using LPCVOID = const void*;
using LPCSTR = const char*;
using LPCTSTR = LPCSTR;
using LPSTR = char*;
using LPTSTR = LPSTR;
using LPCWSTR = const unsigned short*;
using LPWSTR = unsigned short*;
using TCHAR = char;
using WCHAR = unsigned short;
using SOCKET = int;

// 常量定义
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (SOCKET)-1
#endif

// 引用计数接口
class IDsObject {
public:
    IDsObject() = default;
    virtual ~IDsObject() = default;
    virtual int AddRef() = 0;
    virtual int ReleaseRef() = 0;
};

// 引用计数实现类
class CDsObject : public IDsObject {
public:
    CDsObject() : m_dwRef(0) {}

    virtual ~CDsObject() = default;

    int AddRef() override {
        return ++m_dwRef;
    }

    int ReleaseRef() override {
        int iRet = --m_dwRef;
        if (m_dwRef == 0) {
            delete this;
        }
        return iRet;
    }

protected:
    std::atomic<DWORD> m_dwRef;
};

// 网络过滤器接口
class COracleReader;

class IDsNetworkFilter : public CDsObject {
public:
    virtual void SetTimeRange(DWORD dwFromTime, DWORD dwToTime) = 0;
    virtual void IncludePort(int port) = 0;
    virtual void ExcludePort(int port) = 0;
    virtual void IncludeAddress(LPCSTR ip_address) = 0;
    virtual void IncludeAddress(LPCSTR ip_address1, LPCSTR ip_address2) = 0;
    virtual void ExcludeAddress(LPCSTR ip_address) = 0;
    virtual void ExcludeAddress(LPCSTR ip_address1, LPCSTR ip_address2) = 0;
    virtual BOOL CheckTime(DWORD dwTime) = 0;
    virtual BOOL CheckPort(int port) = 0;
    virtual BOOL CheckAddress(LPCSTR ip_address1, LPCSTR ip_address2) = 0;
    virtual BOOL CheckTimeAfter(DWORD dwTime) = 0;
    virtual BOOL CheckTimeBefore(DWORD dwTime) = 0;
    virtual DWORD GetFromTime() = 0;
    virtual BOOL IsRealTime() = 0;
    virtual DWORD GetToTime() = 0;
    virtual void SetFilePath(LPCSTR file_path) = 0;
    virtual LPCSTR GetFilePath() = 0;
    virtual LPCSTR GetAddress() = 0;
    virtual WORD GetPort() = 0;
    virtual DWORD GetRealToTime() = 0;
    virtual void SetRealToTime(DWORD dwTime) = 0;
    virtual void SetInterval(DWORD interval) = 0;
    virtual void SetSrcToDst(LPCSTR pSrcIP, LPCSTR pDstIP) = 0;
    virtual LPCSTR GetSrcAddr() = 0;
    virtual LPCSTR GetDstAddr() = 0;
    virtual BOOL CheckSrcAndDstAddr(LPCSTR lpSrcIPAddr, LPCSTR lpDstIPAddr) = 0;
    virtual void SetLogTime(int nLogTime) = 0;
    virtual int GetLogTime() = 0;
    virtual int GetCacheCount() = 0;
    virtual void SetCacheCount(int nCount) = 0;
    virtual void SetOracle(COracleReader* oracle) = 0;
    virtual COracleReader* GetOracle() = 0;
};

// 创建网络过滤器函数
extern "C" {
    void DsCreateNetworkFilter(IDsNetworkFilter** ppNetFilter, DWORD* pdwTime);
}

#endif // __DOORS_DEFS_H__