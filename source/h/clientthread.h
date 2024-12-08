#ifndef __CLIENT_THREAD_H__
#define __CLIENT_THREAD_H__

#include "json.hpp"
#include "doorsbase.h"
#include <vector>
#include <string>

using json = nlohmann::json;

class CSThread;
class CSocket;

class CClientThread : public CSThread {
public:
    virtual BOOL InitInstance() override;
    virtual BOOL ExitInstance() override;

    void SetClient(CSocket* pClient) { m_pClient = pClient; }

private:
    BOOL Check(const unsigned char* szBuf);
    BOOL Gen(BYTE* szBuf); // make sure the buf size is 1024 Bytes.
    int do_req(char* buf, CSocket* pClient);
    int do_verify(char* buf);
    int do_str_req(CSocket* pClient);
    int do_str_verify(char* buf, CSocket* pClient);

    CSocket* m_pClient{nullptr};
    json m_json;
    std::vector<std::string> m_str;
};

#endif // __CLIENT_THREAD_H__