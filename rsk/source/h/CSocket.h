#ifndef CSOCKET_H
#define CSOCKET_H

typedef struct {
    int socket_fd;
} CSocket;

int CSocket_Send(CSocket *self, const char *data, size_t length);
int CSocket_Create(CSocket *self, int type);
int CSocket_Bind(CSocket *self, int port);
int CSocket_Listen(CSocket *self);
int CSocket_Accept(CSocket *self, CSocket *client);
int CSocket_Send(CSocket *self, const char *data, size_t length);
int CSocket_Recv(CSocket *self, char *buffer, size_t length);
void CSocket_Close(CSocket *self);
const char *CSocket_GetRemoteIp(CSocket *self);
int CSocket_GetPeerPort(CSocket *self);

int CSocket_Send(CSocket *self, const char *data, size_t length);


#endif // CSOCKET_H