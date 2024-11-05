// doorsbase.h
#ifndef __DOORS_BASE_H__
#define __DOORS_BASE_H__

#include <array>
#include <vector>
#include <memory>
#include <string_view>
#include <span>
#include "doorsdefs.h"

class CSocket {
public:
    CSocket();
    explicit CSocket(SOCKET s);
    ~CSocket();

    CSocket(const CSocket&) = delete;
    CSocket& operator=(const CSocket&) = delete;
    CSocket(CSocket&&) noexcept = default;
    CSocket& operator=(CSocket&&) noexcept = default;

    [[nodiscard]] bool Create(SOCKTYPE nSockType);
    [[nodiscard]] bool Connect(std::string_view szServAddr, int nServPort);
    [[nodiscard]] bool ConnectTimeOut(std::string_view szHost, unsigned short nPort, long timeout);
    [[nodiscard]] bool Bind(int nBindPort);
    [[nodiscard]] bool Listen();
    [[nodiscard]] bool Accept(CSocket* clnaccept);
    
    [[nodiscard]] int Send(std::span<const std::byte> buf);
    [[nodiscard]] int Recv(std::span<std::byte> szRecvBuf);
    [[nodiscard]] int RecvFrom(std::span<std::byte> szRecvBuf, struct sockaddr_in* pFromAddr = nullptr, int* nAddrLen = nullptr);
    
    bool Close();
    bool SetNonBlock();
    bool ResetBlock();

    [[nodiscard]] static bool QueryDns(std::string_view szAddress, std::string_view szHost);
    [[nodiscard]] static bool IsDotIp(std::string_view szHost);

private:
    SOCKET m_hSocket{INVALID_SOCKET};
    bool m_bConnected{false};
    uint32_t m_dwPeerIp{};
    int m_nPeerPort{};
    int flags{};
};

// Other class declarations...

#endif // __DOORS_BASE_H__

//BY GST ARMV8 GCC 13.2