#ifndef __CLIENT_THREAD_H__
#define __CLIENT_THREAD_H__

class CSThread;
class CSocket;
#include "json.hpp"
using json = nlohmann::json;
class CClientThread : public CSThread
{
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

public:
	void SetClient(CSocket *pClient) { m_pClient = pClient; }

private:
	CSocket *m_pClient;
	BOOL Check(const unsigned char *szBuf);
	BOOL Gen(BYTE *szBuf); // make sure the buf size is 1024 Bytes.
	int do_req(char *buf, CSocket *pClient);
	int do_verify(char *buf);
	int do_str_req(CSocket *pClient);
	int do_str_verify(char *buf, CSocket *pClient);

private:
	json m_json;
	std::vector<std::string> m_str;
	//int m_login_status;
};

#endif
