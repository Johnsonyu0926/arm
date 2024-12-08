#ifndef __SERVER_THREAD_H__
#define __SERVER_THREAD_H__

#include "CSThread.hpp"
#include "CSocket.hpp"
#include "spdlog/spdlog.h"
#include <mutex>

class CServerThread : public CSThread {
public:
    virtual BOOL InitInstance() override {
        spdlog::info("Initializing server thread on port {}", m_nPort);
        // 在这里添加初始化逻辑，例如创建和绑定套接字
        return TRUE;
    }

    virtual BOOL ExitInstance() override {
        spdlog::info("Exiting server thread on port {}", m_nPort);
        // 在这里添加清理逻辑，例如关闭套接字
        return TRUE;
    }

    void SetPort(int nPort) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_nPort = nPort;
    }

private:
    int m_nPort{0};
    std::mutex m_mutex;
};

#endif // __SERVER_THREAD_H__