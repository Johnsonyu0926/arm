// ServerThread.cpp
#include <cstdio>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <chrono>
#include "doorsbase.h"
#include "serverthread.h"
#include "clientthread.h"
#include "utils.h"
#include "Singleton.hpp"

bool CServerThread::InitInstance() {
    CSocket socket;
    if (!socket.Create(TCP)) {
        LOG(ERROR) << "Failed to create socket";
        return false;
    }
    
    int opt = 1;
    if (setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        LOG(ERROR) << "Failed to set socket option";
        return false;
    }

    if (!socket.Bind(m_nPort)) {
        LOG(ERROR) << "Failed to bind port: " << m_nPort;
        return false;
    }

    if (!socket.Listen()) {
        LOG(ERROR) << "Failed to listen on socket";
        return false;
    }

    CUtils utils;
    int connectionCount = 0;

    while (true) {
        fd_set rset;
        FD_ZERO(&rset);
        FD_SET(socket.m_hSocket, &rset);

        struct timeval timeout{5, 0};

        utils.heart_beat("/tmp/audio_server_netvoice_heartbeat.txt");

        int n = select(socket.m_hSocket + 1, &rset, nullptr, nullptr, &timeout);
        if (n < 0) {
            LOG(WARNING) << "Fatal error: select error!";
            return false;
        } else if (n == 0) {
            continue;
        }

        LOG(INFO) << "Server select n = " << n;

        auto pClient = std::make_unique<CSocket>();
        if (!socket.Accept(pClient.get())) {
            LOG(ERROR) << "Failed to accept connection";
            continue;
        }

        LOG(INFO) << "Got connection #" << connectionCount << ": " 
                  << pClient->GetRemoteIp() << ":" << ntohs(pClient->GetPeerPort());
        Singleton::getInstance().setConnIp(pClient->GetRemoteIp());
        ++connectionCount;

        auto pThread = std::make_unique<CClientThread>();
        pThread->SetClient(pClient.release());
        if (!pThread->CreateThread()) {
            LOG(ERROR) << "Failed to create client thread";
            continue;
        }
        pThread.release(); // Thread will delete itself when finished
    }

    return true;
}

bool CServerThread::ExitInstance() {
    return true;
}

//BY GST ARMV8 GCC 13.2