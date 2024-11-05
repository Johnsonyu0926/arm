// doorsdefs.h
#ifndef __DOORS_DEFS_H__
#define __DOORS_DEFS_H__

#include <string>
#include <memory>
#include <cstdint>

using LONGLONG = long long;
using ULONG = unsigned long;
using INT32 = std::int32_t;
using INT = int;
using UINT = unsigned int;
using DWORD = std::uint32_t;
using BOOL = bool;
using BYTE = std::uint8_t;
using WORD = std::uint16_t;
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
using LPCWSTR = const char16_t*;
using LPWSTR = char16_t*;
using TCHAR = char;
using WCHAR = char16_t;
using SOCKET = int;

inline constexpr BOOL TRUE = true;
inline constexpr BOOL FALSE = false;

#define IN
#define OUT

inline constexpr SOCKET INVALID_SOCKET = static_cast<SOCKET>(-1);

class COracleReader;

class IDsObject {
public:
    virtual ~IDsObject() = default;
    [[nodiscard]] virtual int AddRef() = 0;
    [[nodiscard]] virtual int ReleaseRef() = 0;
};

class CDsObject : public IDsObject {
public:
    CDsObject() : m_dwRef(0) {}
    ~CDsObject() override = default;

    [[nodiscard]] int AddRef() override { return ++m_dwRef; }
    [[nodiscard]] int ReleaseRef() override {
        int iRet = --m_dwRef;
        if (m_dwRef == 0) delete this;
        return iRet;
    }

protected:
    DWORD m_dwRef;
};

// Other class declarations...

#endif // __DOORS_DEFS_H__

//BY GST ARMV8 GCC 13.2