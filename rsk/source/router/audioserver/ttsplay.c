#include "ttsplay.h"
#include "json.hpp"
#include "utils.h"
#include "public.h"
#include "AudioPlayUtils.h"
#include "CSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CTtsPlayResult_init(CTtsPlayResult *self) {
    memset(self->cmd, 0, sizeof(self->cmd));
    self->resultId = 0;
    memset(self->msg, 0, sizeof(self->msg));
}

void CTtsPlayResult_do_success(CTtsPlayResult *self) {
    strcpy(self->cmd, "TtsPlay");
    self->resultId = 1;
    strcpy(self->msg, "play success");
}

void CTtsPlayResult_do_fail(CTtsPlayResult *self, const char *str) {
    strcpy(self->cmd, "TtsPlay");
    self->resultId = 2;
    strcpy(self->msg, str);
}

void CTtsPlay_init(CTtsPlay *self) {
    memset(self->cmd, 0, sizeof(self->cmd));
    memset(self->content, 0, sizeof(self->content));
    self->playType = 0;
    self->duration = 0;
    self->voiceType = 0;
    self->speed = 0;
}

int CTtsPlay_do_req(CTtsPlay *self, CSocket *pClient) {
    CTtsPlayResult ttsPlayResult;
    CTtsPlayResult_init(&ttsPlayResult);
    CTtsPlayResult_do_success(&ttsPlayResult);
    CUtils utils;
    if (CUtils_get_process_status("madplay") || CUtils_get_process_status("ffplay") || PlayStatus_getInstance()->getPlayState()) {
        CTtsPlayResult_do_fail(&ttsPlayResult, "Existing playback task");
    } else {
        char txt[256];
        CUtils_hex_to_string(&utils, self->content, txt);
        switch (self->playType) {
            case 0:
                AudioPlayUtil_tts_loop_play(txt, ASYNC_START, self->speed, self->voiceType);
                break;
            case 1:
                if (self->duration < 1) {
                    CTtsPlayResult_do_fail(&ttsPlayResult, "parameter cannot be less than 1");
                    break;
                }
                AudioPlayUtil_tts_num_play(self->duration, txt, ASYNC_START, self->speed, self->voiceType);
                break;
            case 2:
                if (self->duration < 1) {
                    CTtsPlayResult_do_fail(&ttsPlayResult, "parameter cannot be less than 1");
                    break;
                }
                AudioPlayUtil_tts_time_play(self->duration, txt, ASYNC_START, self->speed, self->voiceType);
                break;
        }
    }
    json js = json_parse((const char *)&ttsPlayResult);
    char *str = json_dump(&js);
    int result = CSocket_Send(pClient, str, strlen(str));
    free(str);
    return result;
}