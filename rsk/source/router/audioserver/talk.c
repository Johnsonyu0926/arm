#include "talk.h"
#include "HPR/HPR_Utils.h"
#include "HPR/HPR_Thread.h"
#include "HPR/HPR_Error.h"
#include "ezDevSDK_talk.h"
#include "utils.h"
#include "log.h"
#include "AudioPlayUtils.h"

extern HPR_VOIDPTR g_pKernel;
extern HPR_BOOL g_bAccessEz;
CTalk g_ctalk;

int g_playing_priority = NON_PLAY_PRIORITY;

HPR_BOOL CTalk_initBase(CTalk *self) {
    ezDevSDK_base_function_init_info struInitInfo = {0};
    HPR_Snprintf(struInitInfo.log_info.path, sizeof(struInitInfo.log_info.path) - 1, "/tmp/ctalk.log");
    struInitInfo.log_info.log_size = 2 * 1024 * 1024;
    struInitInfo.cb.recv_msg = CTalk_BaseFunctionRecvMsg;
    struInitInfo.cb.get_runtime_info = CTalk_BaseFunctionGetRuntimeInfo;
    struInitInfo.cb.ota_notifier = CTalk_BaseFunctionOtaNotifier;
    struInitInfo.channel_num_max = 128;
    int iRet = ezDevSDK_base_function_init(&struInitInfo, g_bAccessEz, g_pKernel);
    (void)iRet;
    return HPR_TRUE;
}

HPR_BOOL CTalk_init(CTalk *self) {
    if (self->m_bInit) {
        LOG(INFO) << "ctalk is inited.";
        return HPR_TRUE;
    }
    if (!g_pKernel) {
        LOG(INFO) << "ctalk init skip since kernel is not ready";
        return HPR_FALSE;
    }

    CTalk_initBase(self);

    ezDevSDK_talk_init_info struInitInfo = {0};
    struInitInfo.max_buf = 50 * 1024;
    struInitInfo.cb.recv_msg = CTalk_RecvMsg;
    struInitInfo.cb.on_get_runtime_info = CTalk_GetRuntimeInfo;
    struInitInfo.cb.on_check_resource = CTalk_CheckResource;
    struInitInfo.cb.on_recv_talk_data = CTalk_RecvTalkData;
    struInitInfo.cb.on_recv_talk_data_new = CTalk_RecvTalkDataNew;
    int iRet = ezDevSDK_talk_init(&struInitInfo, g_bAccessEz, g_pKernel);
    LOG(INFO) << "ctalk init ret: " << iRet;
    if (iRet == 0) {
        self->m_bInit = 1;
    }
    return HPR_TRUE;
}

int CTalk_BaseFunctionRecvMsg(ezDevSDK_base_function_msg_to_dev *msg) {
    LOG(INFO) << "enter! ";
    return 0;
}

int CTalk_BaseFunctionGetRuntimeInfo(ezDevSDK_base_function_runtime_info *info) {
    LOG(INFO) << "enter! ";
    if (info == NULL) {
        return -1;
    }
    switch (info->type) {
    case EZDEVSDK_TALK_RT_INFO_GET_ENCODE_TYPE:
        CTalk_GetEncodeType(&g_ctalk, info->data, info->len);
        break;
    default:
        break;
    }
    return 0;
}

HPR_BOOL CTalk_GetEncodeType(CTalk *self, void *pData, const HPR_UINT32 dwLen) {
    if (pData == NULL || dwLen != sizeof(ezDevSDK_talk_encode_type)) {
        return HPR_FALSE;
    }
    ezDevSDK_talk_encode_type *pEncodeType = (ezDevSDK_talk_encode_type *)pData;
    LOG(INFO) << "old encode " << pEncodeType->encode << " reset to " << EZDEVSDK_AUDIO_PCM;
    pEncodeType->encode = EZDEVSDK_AUDIO_PCM; // EZDEVSDK_AUDIO_AAC;
    return HPR_TRUE;
}

int CTalk_BaseFunctionOtaNotifier(int percent, void *data, int data_len, ezDevSDK_down_err_code errcode, ezDevSDK_down_status status) {
    LOG(INFO) << "enter! ";
    return -1;
}

static int priority = NON_PLAY_PRIORITY;

int CTalk_RecvMsg(ezDevSDK_talk_msg_to_dev *msg) {
    if (NULL == msg) {
        return -1;
    }
    HPR_BOOL bRet = HPR_FALSE;
    switch (msg->type) {
    case EZDEVSDK_TALK_ON_START_TALK: {
        ezDevSDK_talk_start_talk *param = (ezDevSDK_talk_start_talk *)(msg->data);
        LOG(INFO) << "priority: " << param->priority;
        if (g_playing_priority <= param->priority) {
            LOG(INFO) << "skip start talk! playing priority " << g_playing_priority << " " << param->priority;
            return -1;
        }
        AudioPlayUtil_audio_stop();
        CTalk_do_fork(&g_ctalk);

        g_playing_priority = param->priority;
        priority = param->priority;
        LOG(INFO) << "ezdevsdk talk on start talk msg type is incoming, len: " << msg->len;
    } break;
    case EZDEVSDK_TALK_ON_STOP_TALK: {
        LOG(INFO) << "ezdevsdk talk on stop talk msg type is incoming.";
        AudioPlayUtil_audio_stop();
        g_playing_priority = NON_PLAY_PRIORITY;
    } break;
    case EZDEVSDK_TALK_ON_RECV_OTAP_MSG:
        LOG(INFO) << "ezdevsdk talk on recv otap msg msg type is incoming.";
        // bRet = g_ctalk.ProcessTalk(msg);
        break;
    default:
        LOG(INFO) << "unknown msg type: " << msg->type;
        break;
    }
    if (bRet) {
        return 0;
    }
    return 0;
}

int CTalk_GetRuntimeInfo(ezDevSDK_talk_runtime_info *info) {
    if (info == NULL) {
        return -1;
    }
    switch (info->type) {
    case EZDEVSDK_TALK_RT_INFO_GET_ENCODE_TYPE:
        CTalk_GetEncodeType(&g_ctalk, info->data, info->len);
        break;
    default:
        break;
    }
    return 0;
}

int CTalk_RecvTalkData(int channel, char *data, int len) {
    LOG(INFO) << "data =====" << len << "writing to " << g_ctalk.pipefd[1];
    if (g_ctalk.pipefd[1] < 0) {
        LOG(INFO) << "pipe is not open.";
        return -1;
    }

    int num = write(g_ctalk.pipefd[1], data, len);
    if (num < 0) {
        LOG(INFO) << "failed to write pipe! " << g_ctalk.pipefd[1];
        return -1;
    }
    return 0;
}

int CTalk_do_fork(CTalk *self) {
    if (pipe(self->pipefd) == -1) {
        LOG(INFO) << "error open pipe.";
        return -1;
    }

    pid_t pid;
    pid = fork();
    if (pid == -1) {
        LOG(INFO) << "failed to fork.";
        return -1;
    }

    if (pid == 0) {
        // child process.
        dup2(self->pipefd[0], STDIN_FILENO);
        close(self->pipefd[0]);
        close(self->pipefd[1]);
        g_playing_priority = priority;
        execlp("madplay", "madplay", "-", NULL);
        PlayStatus_getInstance()->setPlayId(asns::TALK_TASK_PLAYING);
        PlayStatus_getInstance()->setPriority(g_playing_priority);
    }

    // parent process.
    close(self->pipefd[0]); // close write

    return 0;
}

int CTalk_initMp3(CTalk *self) {
    self->fp = fopen("/audiodata/test.mp3", "wb");
    if (!self->fp) {
        LOG(INFO) << "error open test.mp3";
        return -1;
    }
    return 0;
}

int CTalk_finiMp3(CTalk *self) {
    if (self->fp) {
        fclose(self->fp);
    }
    return 0;
}

int CTalk_saveMp3(CTalk *self, unsigned char *data, int len) {
    if (self->fp) {
        fwrite(data, 1, len, self->fp);
    } else {
        LOG(INFO) << "error write, fp is null.";
        return -1;
    }
    return 0;
}

int CTalk_CheckResource(int cur_count, int channel) {
    return 0;
}

int CTalk_RecvTalkDataNew(int channel, char *data, int len, int encode_type) {
    return 0;
}