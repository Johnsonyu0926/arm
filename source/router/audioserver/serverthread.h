#ifndef __SERVER_THREAD_H__
#define __SERVER_THREAD_H__

#include "CSThread.h"
#include "CSocket.h"
#include "spdlog/spdlog.h"

class CServerThread : public CSThread {
public:
    virtual BOOL InitInstance() override {
        spdlog::info("Initializing server thread on port {}", m_nPort);
        // 在这里添加初始化逻辑，例如创建和绑定套接字
        CSocket socket;
        socket.Create(TCP);
        int opt = 1;
        setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(opt));

        if (!socket.Bind(m_nPort)) {
            spdlog::error("Failed to bind port: {}", m_nPort);
            exit(1);
        }
        socket.Listen();
        int i = 0;
        while (true) {
            struct timeval timeout;
            timeout.tv_sec = 5;
            timeout.tv_usec = 0;

            fd_set rset;
            FD_ZERO(&rset);
            FD_SET(socket.m_hSocket, &rset);
            CUtils utils;
            utils.heart_beat("/tmp/audio_server_netvoice_heartbeat.txt");
            int n = select(socket.m_hSocket + 1, &rset, nullptr, nullptr, &timeout);
            if (n < 0) {
                spdlog::warn("Fatal, select error!");
                return 0;
            } else if (n == 0) {
                continue;
            } else if (n > 0) {
                spdlog::info("Server select n = {}", n);
            }
            auto pClient = std::make_unique<CSocket>();
            socket.Accept(pClient.get());
            spdlog::info("Got the no.{} connection: {}:{}", i, pClient->GetRemoteIp(), ntohs(pClient->GetPeerPort()));
            Singleton::getInstance().m_connIp = pClient->GetRemoteIp();
            i++;
            auto pThread = std::make_unique<CClientThread>();
            pThread->SetClient(pClient.release());
            pThread->CreateThread();
        }
        return TRUE;
    }

    virtual BOOL ExitInstance() override {
        spdlog::info("Exiting server thread on port {}", m_nPort);
        // 在这里添加清理逻辑，例如关闭套接字
        return TRUE;
    }

    void SetPort(int nPort) { m_nPort = nPort; }

private:
    int m_nPort{0};
};

#endif // __SERVER_THREAD_H__