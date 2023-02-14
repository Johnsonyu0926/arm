#include <stdlib.h>

#ifdef __FreeBSD__
#include <sys/types.h>
#endif

#include <thread>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "doorsbase.h"
#include "serverthread.h"

#include "login.hpp"

#include "audiocfg.hpp"

extern int g_nDebugMode;

//#define _DEBUG_TRACE_LOG_
extern char *optarg;

int g_nStop;
int g_nThreadCount;
int g_nThreadFailedCount;
int g_nCheckSum = 0;
int g_nDoubleDirect = 0;

static pthread_mutex_t g_finishLock = PTHREAD_MUTEX_INITIALIZER;

/* -------------------------- audio global ---------------------*/

/* ---------------------------------------------------------------*/

int PrintVersion() {
    CAudioCfgBusiness cfg;
    cfg.load();
    printf("audioserver version %s\n", cfg.business[0].codeVersion.c_str());
    return 0;
}

int Usage() {
    char szUsage[] = "audioserver options\n\
options:\n\
	-p xx: port\n\
For example:\n\
\n\
		 ./audioserver -p 10005 -d 5\n\
			\n";

    PrintVersion();
    printf(szUsage);
    exit(0);
    return 0;
}

extern int audio_work(); // in audio.cpp

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);

    if (argc < 2)
        Usage();

    g_nThreadCount = 0;
    g_nThreadFailedCount = 0;

    g_nDebugMode = 0;
    g_nStop = 0;

    BOOL bCloseSocketImmediately = FALSE;

    int nPort = 10006;
    int op;
    BOOL bTransmit = FALSE;
    int nBackThreadCount = 1;
    int nYouRate = 0;
    int nMyRate = -1;
    char szHost[32];
    int nWait = 0;
    int nSize = 0;
    int nSpeedTime = 0;
    int nStopWhenFinish = 0;
    char szUpperServer[32];
    memset(szUpperServer, 0, sizeof(szUpperServer));
    int nUpperServerPort = 0;
    int bSmtp = FALSE;
    BOOL bSmtpClient = FALSE;

    // for smtp client
    char szSender[128];
    char szRcpt[128];
    int nMailSize = 0;

    int nLoopTime = 0;

    char szPassword[64];

    int bAuth = TRUE;

    int nWaitToContinue = 0;

    int nBlockCount = 1; // k --> max 64, 32k
    int nDataSize = 1;     // K -> 1024k.
    int nReadCount = 0;

    BOOL bHttpClient = FALSE;

    while (
            (op = getopt(argc, argv, "a:A:B:b:cCd:De:f:gGh:i:j:kl:m:n:o:p:q:r:s:tu:Uvw:xy:z:")) != EOF) {
        switch (op) {
            case 'a':
                nBlockCount = atoi(optarg);
                break;
            case 'A':
                nDataSize = atoi(optarg);
                break;
            case 'b':
                nBackThreadCount = atoi(optarg);
                break;
            case 'B':
                nReadCount = atoi(optarg);
                break;
            case 'c':
                g_nCheckSum = 1;
                break;
            case 'C':
                bCloseSocketImmediately = TRUE;
                break;
            case 'd':
                g_nDebugMode = atoi(optarg);
                break;
            case 'D':
                g_nDoubleDirect = 1;
                break;
            case 'e':
                nSpeedTime = atoi(optarg);
                break;
            case 'f':
                nStopWhenFinish = atoi(optarg);
                break;
            case 'g':
                bSmtpClient = TRUE;
                break;
            case 'G':
                bHttpClient = TRUE;
                break;
            case 'h':
                strncpy(szHost, optarg, sizeof(szHost));
                break;
            case 'i':
                strncpy(szUpperServer, optarg, sizeof(szUpperServer));
                break;
            case 'j':
                nUpperServerPort = atoi(optarg);
                break;
            case 'k':
                bSmtp = TRUE;
                break;
            case 'l':
                strncpy(szSender, optarg, sizeof(szSender));
                break;
            case 'm':
                nMyRate = atoi(optarg);
                break;
            case 'n':
                nMailSize = atoi(optarg);
                break;
            case 'o':
                nLoopTime = atoi(optarg);
                DS_TRACE("nLoopTime = " << nLoopTime);
                break;
            case 'p':
                nPort = atoi(optarg);
                break;
            case 'q':
                strncpy(szRcpt, optarg, sizeof(szRcpt));
                break;
            case 's':
                nSize = atoi(optarg);
                break;
            case 't':
                bTransmit = TRUE; // As client. transmit.
                break;
            case 'u':
                bAuth = FALSE;
                break;
            case 'v':
                PrintVersion();
                exit(0);
                break;
            case 'w':
                nWait = atoi(optarg);
                break;

            case 'x':
                nWaitToContinue = TRUE;
                break;
            case 'y':
                nYouRate = atoi(optarg);
                break;
            case 'z':
                strncpy(szPassword, optarg, sizeof(szPassword));
                break;
            default:
                Usage();
                break;
        };
    }

    // DS_TRACE("Working...");
    // printf("Checksum = %d\n",g_nCheckSum);
    CSTime t;
    t.TickStart();

    CServerThread *pServer = new CServerThread();
    // DS_TRACE("Set port:" << nPort << ",Upper server ip:" << szUpperServer << ", upper server port:" << nUpperServerPort);
    pServer->SetPort(nPort);
    pServer->CreateThread();
    g_nThreadCount++;

    audio_work();
    return TRUE;
}