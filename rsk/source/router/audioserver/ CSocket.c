#include "CSocket.h"
#include <unistd.h>
#include "CSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int CSocket_Create(CSocket *self, int type) {
    self->socket_fd = socket(AF_INET, type, 0);
    return self->socket_fd != -1;
}

int CSocket_Bind(CSocket *self, int port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    return bind(self->socket_fd, (struct sockaddr *)&addr, sizeof(addr)) != -1;
}

int CSocket_Listen(CSocket *self) {
    return listen(self->socket_fd, 5) != -1;
}

int CSocket_Accept(CSocket *self, CSocket *client) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    client->socket_fd = accept(self->socket_fd, (struct sockaddr *)&addr, &addr_len);
    return client->socket_fd != -1;
}

int CSocket_Send(CSocket *self, const char *data, size_t length) {
    return send(self->socket_fd, data, length, 0);
}

int CSocket_Recv(CSocket *self, char *buffer, size_t length) {
    return recv(self->socket_fd, buffer, length, 0);
}

void CSocket_Close(CSocket *self) {
    close(self->socket_fd);
}

const char *CSocket_GetRemoteIp(CSocket *self) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    getpeername(self->socket_fd, (struct sockaddr *)&addr, &addr_len);
    return inet_ntoa(addr.sin_addr);
}

int CSocket_GetPeerPort(CSocket *self) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    getpeername(self->socket_fd, (struct sockaddr *)&addr, &addr_len);
    return ntohs(addr.sin_port);
}
int CSocket_Send(CSocket *self, const char *data, size_t length) {
    return write(self->socket_fd, data, length);
}
