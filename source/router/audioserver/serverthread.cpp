#include <stdio.h>
#include "doorsbase.h"
#include "serverthread.h"
#include "clientthread.h"
#include <sys/ioctl.h>
#include <fcntl.h>

BOOL CServerThread::InitInstance() {
    CSocket socket;
    socket.Create(TCP);
    int opt = 1;
    setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(opt));

    if (!socket.Bind(m_nPort)) {
        DS_TRACE("Failed bind port:" << m_nPort);
        exit(1);
    }
    socket.Listen();
    int i = 0;
    while (1) {
        CSocket *pClient = new CSocket;

        fd_set rset;
        FD_ZERO(&rset);
        FD_SET(socket.m_hSocket, &rset);

        int n = select(socket.m_hSocket + 1, &rset, NULL, NULL, NULL);
        if (n < 0) {
            DS_TRACE("fatal , select error!\n");
            return 0;
        } else if (n > 0) {
            DS_TRACE("server select n = " << n);
        }

        socket.Accept(pClient);
        // DS_TRACE("Got the no."<<i<<" connection :"<<pClient->GetRemoteIp()<<":"<<ntohs(pClient->GetPeerPort()));
        i++;
        CClientThread *pThread = new CClientThread();
        pThread->SetClient(pClient);
        pThread->CreateThread();
    }
    return TRUE;
}

BOOL CServerThread::ExitInstance() {
    return TRUE;
}
