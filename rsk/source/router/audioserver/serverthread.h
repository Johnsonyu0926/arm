// ServerThread.hpp
#ifndef __SERVER_THREAD_H__
#define __SERVER_THREAD_H__

#include "CSThread.h"

class CSocket;

class CServerThread : public CSThread {
public:
    CServerThread() = default;
    ~CServerThread() override = default;

    CServerThread(const CServerThread&) = delete;
    CServerThread& operator=(const CServerThread&) = delete;
    CServerThread(CServerThread&&) noexcept = default;
    CServerThread& operator=(CServerThread&&) noexcept = default;

    [[nodiscard]] bool InitInstance() override;
    [[nodiscard]] bool ExitInstance() override;

    void SetPort(int nPort) noexcept { m_nPort = nPort; }
    [[nodiscard]] int GetPort() const noexcept { return m_nPort; }

private:
    int m_nPort{0};
};

#endif // __SERVER_THREAD_H__

//BY GST ARMV8 GCC 13.2