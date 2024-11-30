#include "tcptransferthead.h"
#include "public.h"
#include "add_column_custom_audio_file.h"
#include "utils.h"
#include "rs485.h"
#include "CSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

BOOL TcpTransferThread_InitInstance(TcpTransferThread *self) {
    CSocket socket;
    if (!CSocket_Create(&socket, TCP)) {
        return TcpTransferThread_SendFast(self, asns::OPERATION_FAIL_ERROR, self->pClient);
    }
    int opt = 1;
    setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(opt));
    for (int i = self->m_nPort; i < asns::ENDPORT; ++i) {
        if (CSocket_Bind(&socket, i)) {
            self->m_nPort = i;
            if (atoi(self->m_vecStr[3]) != asns::RECORDEND) {
                char res[64];
                snprintf(res, sizeof(res), "01 E1 %s %d", self->m_vecStr[5], self->m_nPort);
                if (self->pClient == NULL) {
                    Rs485_uart_work(res, strlen(res));
                } else {
                    CSocket_Send(self->pClient, res, strlen(res));
                }
            } else {
                CUtils utils;
                int file_size = CUtils_get_file_size(asns::RECORD_PATH);
                char res[64];
                snprintf(res, sizeof(res), "01 E1 %s %d %d", self->m_vecStr[4], file_size, self->m_nPort);
                if (self->pClient == NULL) {
                    Rs485_uart_work(res, strlen(res));
                } else {
                    CSocket_Send(self->pClient, res, strlen(res));
                }
            }
            break;
        } else if (i == asns::ENDPORT) {
            printf("fatal , bind error!\n");
            return TcpTransferThread_SendFast(self, asns::OPERATION_FAIL_ERROR, self->pClient);
        }
    }
    CSocket_Listen(&socket);
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(socket.m_hSocket, &rset);
    struct timeval timeout;
    timeout.tv_sec = 30;
    timeout.tv_usec = 0;
    int n = select(socket.m_hSocket + 1, &rset, NULL, NULL, &timeout);
    if (n < 0) {
        printf("fatal , select error!\n");
        return TcpTransferThread_SendFast(self, asns::OPERATION_FAIL_ERROR, self->pClient);
    } else if (n == 0) {
        printf("timeout!\n");
        return TcpTransferThread_SendFast(self, asns::TCP_TIMEOUT, self->pClient);
    } else if (n > 0) {
        printf("server select n = %d\n", n);
    }
    CSocket *pTcp = (CSocket *)malloc(sizeof(CSocket));
    CSocket_Accept(&socket, pTcp);
    printf("Got the no. connection : %s:%d\n", CSocket_GetRemoteIp(pTcp), ntohs(CSocket_GetPeerPort(pTcp)));
    TcpTransferThread_do_req(self, pTcp);
    CSocket_Close(&socket);
    free(pTcp);
    return 1;
}

BOOL TcpTransferThread_ExitInstance(TcpTransferThread *self) {
    printf("exit tcp transfer thread.\n");
    return TRUE;
}

void TcpTransferThread_SetPort(TcpTransferThread *self, int nPort) {
    self->m_nPort = nPort;
}

void TcpTransferThread_SetClient(TcpTransferThread *self, CSocket *pClient) {
    self->pClient = pClient;
}

void TcpTransferThread_SetVecStr(TcpTransferThread *self, char **vecStr, int vecStr_count) {
    self->m_vecStr = vecStr;
    self->m_vecStr_count = vecStr_count;
}

int TcpTransferThread_do_req(TcpTransferThread *self, CSocket *pTcp) {
    int condition = atoi(self->m_vecStr[3]);
    switch (condition) {
        case asns::RECORDEND:
            TcpTransferThread_Record(self, pTcp);
            break;
        case asns::AUDIOFILEUPLOAD:
            TcpTransferThread_FileUpload(self, pTcp);
            break;
        case asns::REMOTEFILEUPGRADE:
            TcpTransferThread_FileUpgrade(self, pTcp);
            break;
        default:
            TcpTransferThread_SendFast(self, asns::NONSUPPORT_ERROR, self->pClient);
            break;
    }
    return 1;
}

int TcpTransferThread_Record(TcpTransferThread *self, CSocket *pTcp) {
    CUtils utils;
    int file_size = CUtils_get_file_size(asns::RECORD_PATH);
    printf("record file size: %d\n", file_size);
    char buf[asns::BUFSIZE] = {0};
    FILE *fs = fopen(asns::RECORD_PATH, "rb");
    if (fs == NULL) {
        printf("failed to open record file\n");
        return TcpTransferThread_SendFast(self, asns::OPERATION_FAIL_ERROR, self->pClient);
    }
    while (!feof(fs)) {
        size_t read_count = fread(buf, 1, sizeof(buf), fs);
        printf("%zu ", read_count);
        if (read_count <= 0) {
            printf("read count < 0\n");
            break;
        }
        CSocket_Send(pTcp, buf, read_count);
        file_size -= read_count;
        if (file_size <= 0) {
            printf("file size < 0\n");
            break;
        }
    }
    fclose(fs);
    printf("Send ok\n");
    system("rm /tmp/record.mp3");
    return TcpTransferThread_SendTrue(self, self->pClient);
}

int TcpTransferThread_FileUpload(TcpTransferThread *self, CSocket *pTcp) {
    char prefix[64];
    char suffix[64];
    sscanf(self->m_vecStr[6], "%[^.].%s", prefix, suffix);
    for (char *p = suffix; *p; ++p) *p = tolower(*p);
    char name[128];
    snprintf(name, sizeof(name), "%s.%s", prefix, suffix);
    asns::CAudioCfgBusiness cfg;
    char path[256];
    snprintf(path, sizeof(path), "%s%s", cfg.getAudioFilePath(), name);
    FILE *fs = fopen(path, "wb");
    if (fs == NULL) {
        printf("failed to open file for writing\n");
        return TcpTransferThread_SendFast(self, asns::OPERATION_FAIL_ERROR, self->pClient);
    }

    int file_size = atoi(self->m_vecStr[4]);
    char buf[asns::BUFSIZE] = {0};

    while (file_size > 0) {
        int len = CSocket_Recv(pTcp, buf, sizeof(buf));
        printf("recv len: %d\n", len);
        if (len > 0) {
            fwrite(buf, 1, len, fs);
            file_size -= len;
        } else if (len == 0) {
            printf("read end\n");
            break;
        } else if (len < 0) {
            fclose(fs);
            char cmd[256];
            snprintf(cmd, sizeof(cmd), "rm %s", path);
            system(cmd);
            return TcpTransferThread_SendFast(self, asns::RECORD_SIZE_ERROR, self->pClient);
        }
    }
    fclose(fs);
    printf("begin json\n");
    CUtils utils;
    CUtils_bit_rate_32_to_48(path);
    asns::CAddColumnCustomAudioFileBusiness business;
    if (!CAddColumnCustomAudioFileBusiness_exist(&business, name)) {
        asns::CAddColumnCustomAudioFileData node;
        node.type = asns::AUDIO_FILE_TYPE;
        CAddColumnCustomAudioFileData_setName(&node, name);
        node.size = CUtils_get_file_size(path);
        CAddColumnCustomAudioFileBusiness_push_back(&business, &node);
        CAddColumnCustomAudioFileBusiness_saveJson(&business);
    } else {
        CAddColumnCustomAudioFileBusiness_updateSize(&business, name, atoi(self->m_vecStr[4]));
    }
    return TcpTransferThread_SendTrue(self, self->pClient);
}

int TcpTransferThread_FileUpgrade(TcpTransferThread *self, CSocket *pTcp) {
    CUtils utils;
    FILE *fs = fopen(asns::FIRMWARE_PATH, "wb");
    if (fs == NULL) {
        printf("failed to open firmware file for writing\n");
        return TcpTransferThread_SendFast(self, asns::OPERATION_FAIL_ERROR, self->pClient);
    }
    int file_size = atoi(self->m_vecStr[4]);
    char buf[asns::BUFSIZE] = {0};
    while (file_size > 0) {
        int len = CSocket_Recv(pTcp, buf, sizeof(buf));
        if (len > 0) {
            fwrite(buf, 1, len, fs);
            printf("%d ", len);
            file_size -= len;
        } else if (len <= 0) {
            fclose(fs);
            return TcpTransferThread_SendFast(self, asns::TCP_TRANSFER_ERROR, self->pClient);
        }
    }
    fclose(fs);
    TcpTransferThread_SendTrue(self, self->pClient);
    printf("begin up read size: %d\n", CUtils_get_file_size(asns::FIRMWARE_PATH));
    char *cmdRes = CUtils_get_by_cmd_res("webs -U /var/run/SONICCOREV100R001.bin");
    printf("cmd res: %s\n", cmdRes);
    if (strstr(cmdRes, "OK") != NULL) {
        CUtils_reboot();
    } else {
        system("rm /var/run/SONICCOREV100R001.bin");
    }
    free(cmdRes);
    return 1;
}

int TcpTransferThread_SendTrue(TcpTransferThread *self, CSocket *pClient) {
    const char *res = "01 E1";
    printf("return: %s\n", res);
    if (pClient == NULL) {
        return Rs485_uart_work(res, strlen(res));
    }
    return CSocket_Send(pClient, res, strlen(res));
}

int TcpTransferThread_SendFast(TcpTransferThread *self, const char *err_code, CSocket *pClient) {
    char buf[64];
    snprintf(buf, sizeof(buf), "01 %s", err_code);
    printf("return: %s\n", buf);
    if (pClient == NULL) {
        return Rs485_uart_work(buf, strlen(buf));
    }
    return CSocket_Send(pClient, buf, strlen(buf));
}