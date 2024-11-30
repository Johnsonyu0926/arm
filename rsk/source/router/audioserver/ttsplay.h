#ifndef TTSPLAY_H
#define TTSPLAY_H

#include "json.hpp"
#include "utils.h"
#include "public.h"
#include "AudioPlayUtils.h"
#include "CSocket.h"

typedef struct {
    char cmd[32];
    int resultId;
    char msg[256];
} CTtsPlayResult;

typedef struct {
    char cmd[32];
    char content[256];
    int playType;
    int duration;
    int voiceType;
    int speed;
} CTtsPlay;

void CTtsPlayResult_init(CTtsPlayResult *self);
void CTtsPlayResult_do_success(CTtsPlayResult *self);
void CTtsPlayResult_do_fail(CTtsPlayResult *self, const char *str);
void CTtsPlay_init(CTtsPlay *self);
int CTtsPlay_do_req(CTtsPlay *self, CSocket *pClient);

#endif // TTSPLAY_H