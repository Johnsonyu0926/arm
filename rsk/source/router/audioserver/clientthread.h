// clientthread.hpp
#pragma once

#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <memory>

class CSThread;
class CSocket;

class CClientThread : public CSThread {
public:
    CClientThread() = default;
    ~CClientThread() override = default;

    CClientThread(const CClientThread&) = delete;
    CClientThread& operator=(const CClientThread&) = delete;
    CClientThread(CClientThread&&) = delete;
    CClientThread& operator=(CClientThread&&) = delete;

    [[nodiscard]] bool InitInstance() override;
    [[nodiscard]] bool ExitInstance() override;
    void SetClient(std::shared_ptr<CSocket> pClient) { m_pClient = std::move(pClient); }

private:
    [[nodiscard]] bool Check(const unsigned char* szBuf) const;
    [[nodiscard]] bool Gen(unsigned char* szBuf) const; // ensure the buffer size is 1024 Bytes
    [[nodiscard]] int do_req(const char* buf);
    [[nodiscard]] int do_verify(const char* buf) const;
    [[nodiscard]] int do_str_req();
    [[nodiscard]] int do_str_verify(const char* buf);

    std::shared_ptr<CSocket> m_pClient;
    nlohmann::json m_json;
    std::vector<std::string> m_str;
};

//By GST ARMV8 GCC13.2 clientthread.hpp