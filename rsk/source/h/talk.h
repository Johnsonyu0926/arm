#ifndef __CTALK_H__
#define __CTALK_H__

#include "ezDevSDK_talk_def.h"
#include "ezDevSDK_base_function.h"
#include "HPR_Types.h"

#define NON_PLAY_PRIORITY 100

typedef struct {
    char *buf;
    int len;
} CBlock;

typedef struct {
    FILE *fp;
    int m_bInit;
    int pipefd[2];
} CTalk;

extern CTalk g_ctalk;

HPR_BOOL CTalk_initBase(CTalk *self);
HPR_BOOL CTalk_init(CTalk *self);
int CTalk_RecvMsg(ezDevSDK_talk_msg_to_dev *msg);
int CTalk_GetRuntimeInfo(ezDevSDK_talk_runtime_info *info);
int CTalk_RecvTalkData(int channel, char *data, int len);
int CTalk_CheckResource(int cur_count, int channel);
int CTalk_RecvTalkDataNew(int channel, char *data, int len, int encode_type);
int CTalk_BaseFunctionRecvMsg(ezDevSDK_base_function_msg_to_dev *msg);
int CTalk_BaseFunctionGetRuntimeInfo(ezDevSDK_base_function_runtime_info *info);
int CTalk_BaseFunctionOtaNotifier(int percent, void *data, int data_len, ezDevSDK_down_err_code errcode, ezDevSDK_down_status status);
HPR_BOOL CTalk_GetEncodeType(CTalk *self, void *pData, const HPR_UINT32 dwLen);
int CTalk_initMp3(CTalk *self);
int CTalk_finiMp3(CTalk *self);
int CTalk_saveMp3(CTalk *self, unsigned char *data, int len);
int CTalk_do_fork(CTalk *self);

#endif // __CTALK_H__