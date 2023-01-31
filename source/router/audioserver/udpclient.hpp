#pragma once

#include "doorsbase.h"
#include <iostream>
#include <string>

class CSUdpClient
{
public:
    CSUdpClient(std::string ip, int nPort) : m_ip(ip), m_nPort(nPort)
    {
        // socket.Connect(m_ip.c_str(), m_nPort);
    }
    void SetSockOpt()
    {
        // int opt = 1;
        // setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
        // unsigned char ttl = 255;
        // setsockopt(socket.m_hSocket, IPPROTO_TP, IP_MULTICAST_TTL, &ttl, sizeof ttl);
        // ip_mreq mreq;
        // mreq.imr_multiaddr.s_addr = inet_addr(m_ip.c_str());
        // mreq.imr_interface.s_addr = htons(m_nPort);
        // setsockopt(socket.m_hSocket, IPPROTO_TP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
    }
    int Send(std::string str)
    {
        CSocket socket;
        socket.Create(UDP);
        int opt = 1;
        setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
        unsigned char ttl = 255;
        setsockopt(socket.m_hSocket, IPPROTO_TP, IP_MULTICAST_TTL, &ttl, sizeof ttl);
        ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(m_ip.c_str());
        mreq.imr_interface.s_addr = htons(m_nPort);
        setsockopt(socket.m_hSocket, IPPROTO_TP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
        std::cout << "IP:" << m_ip << "port:" << m_nPort << " " << str << std::endl;
        return socket.SendTo(str.c_str(), str.length(), m_ip.c_str(), m_nPort);
    }

public:
    void SetPort(int nPort) { m_nPort = nPort; }

    void SetIp(std::string ip) { m_ip = ip; }

private:
    int m_nPort;
    std::string m_ip;
};