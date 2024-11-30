#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "utils.h"
#include "audiocfg.h"
#include "audioplay.h"
#include "audiostop.h"
#include "audio_del.h"
#include "broadcast_plan.h"
#include "MqttConfig.h"
#include "Relay.h"
#include "Rs485.h"
#include "Rs485NoiseMange.h"
#include "socket.h"

typedef struct {
    // Add necessary fields here
} CClientThread;

bool client_thread_init_instance(CClientThread* thread);
bool client_thread_exit_instance(CClientThread* thread);
bool client_thread_check(const CClientThread* thread, const unsigned char* szBuf);
bool client_thread_gen(const CClientThread* thread, unsigned char* szBuf);
int client_thread_do_req(CClientThread* thread, const char* buf, CSocket* pClient);
int client_thread_do_verify(const CClientThread* thread, const char* buf);
int client_thread_do_str_req(CClientThread* thread, CSocket* pClient);
int client_thread_do_str_verify(const CClientThread* thread, const char* buf, CSocket* pClient);

#endif // CLIENTTHREAD_H