#ifndef TCPTRANSFERTHREAD_H
#define TCPTRANSFERTHREAD_H

#include "public.h"
#include "add_column_custom_audio_file.h"
#include "utils.h"
#include "rs485.h"
#include "CSocket.h"

typedef struct {
    CSocket *pClient;
    int m_nPort;
    char **m_vecStr;
    int m_vecStr_count;
} TcpTransferThread;

BOOL TcpTransferThread_InitInstance(TcpTransferThread *self);
BOOL TcpTransferThread_ExitInstance(TcpTransferThread *self);
void TcpTransferThread_SetPort(TcpTransferThread *self, int nPort);
void TcpTransferThread_SetClient(TcpTransferThread *self, CSocket *pClient);
void TcpTransferThread_SetVecStr(TcpTransferThread *self, char **vecStr, int vecStr_count);

#endif // TCPTRANSFERTHREAD_H