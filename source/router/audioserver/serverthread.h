#ifndef __SERVER_THREAD_H__
#define __SERVER_THREAD_H__

class CSThread;
class CSocket;
class CServerThread:public CSThread
{

	public:
		virtual BOOL InitInstance();
		virtual BOOL ExitInstance();
	public:
		void SetPort(int nPort) { m_nPort = nPort;}
		
	private:
		int m_nPort;

};
#endif
