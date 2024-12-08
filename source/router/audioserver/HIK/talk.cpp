#include "HPR/HPR_Utils.h"
#include "HPR/HPR_Thread.h"
#include "HPR/HPR_Error.h"
#include "ezDevSDK_talk.h"
#include "talk.h"
#include "utils.h"
#include "log.h"
#include "AudioPlayUtils.hpp"

extern HPR_VOIDPTR g_pKernel;
extern HPR_BOOL g_bAccessEz;
CTalk g_ctalk;

int g_playing_priority = NON_PLAY_PRIORITY;

HPR_BOOL CTalk::initBase() {
    ezDevSDK_base_function_init_info struInitInfo = {0};
    HPR_Snprintf(struInitInfo.log_info.path, sizeof(struInitInfo.log_info.path) - 1, "/tmp/ctalk.log");
    struInitInfo.log_info.log_size = 2 * 1024 * 1024;
    struInitInfo.cb.recv_msg = BaseFunctionRecvMsg;
    struInitInfo.cb.get_runtime_info = BaseFunctionGetRuntimeInfo;
    struInitInfo.cb.ota_notifier = BaseFunctionOtaNotifier;
    struInitInfo.channel_num_max = 128;
    int iRet = ezDevSDK_base_function_init(&struInitInfo, g_bAccessEz, g_pKernel);
    (void)iRet;
    return HPR_TRUE;
}

HPR_BOOL CTalk::init() {
    if (m_bInit) {
        LOG(INFO) << "ctalk is inited.";
        return HPR_TRUE;
    }
    if (!g_pKernel) {
        LOG(INFO) << "ctalk init skip since kernel is not ready";
        return HPR_FALSE;
    }

    initBase();

    ezDevSDK_talk_init_info struInitInfo = {0};
    struInitInfo.max_buf = 50 * 1024;
    struInitInfo.cb.recv_msg = RecvMsg;
    struInitInfo.cb.on_get_runtime_info = GetRuntimeInfo;
    struInitInfo.cb.on_check_resource = CheckResource;
    struInitInfo.cb.on_recv_talk_data = RecvTalkData;
    struInitInfo.cb.on_recv_talk_data_new = RecvTalkDataNew;
    int iRet = ezDevSDK_talk_init(&struInitInfo, g_bAccessEz, g_pKernel);
    LOG(INFO) << "ctalk init ret: " << iRet;
    if (iRet == 0) {
        m_bInit = 1;
    }
    return HPR_TRUE;
}

int CTalk::BaseFunctionRecvMsg(ezDevSDK_base_function_msg_to_dev *msg) {
    LOG(INFO) << "enter! ";
    return 0;
}

int CTalk::BaseFunctionGetRuntimeInfo(ezDevSDK_base_function_runtime_info *info) {
    LOG(INFO) << "enter! ";
    if (info == nullptr) {
        return -1;
    }
    switch (info->type) {
        case EZDEVSDK_TALK_RT_INFO_GET_ENCODE_TYPE:
            g_ctalk.GetEncodeType(info->data, info->len);
            break;
        default:
            break;
    }
    return 0;
}

HPR_BOOL CTalk::GetEncodeType(void *pData, const HPR_UINT32 dwLen) {
    if (pData == nullptr || dwLen != sizeof(ezDevSDK_talk_encode_type)) {
        return HPR_FALSE;
    }
    auto *pEncodeType = static_cast<ezDevSDK_talk_encode_type *>(pData);
    LOG(INFO) << "old encode " << pEncodeType->encode << " reset to " << EZDEVSDK_AUDIO_PCM;
    pEncodeType->encode = EZDEVSDK_AUDIO_PCM; // EZDEVSDK_AUDIO_AAC;
    return HPR_TRUE;
}

int CTalk::BaseFunctionOtaNotifier(int percent, void *data, int data_len, ezDevSDK_down_err_code errcode, ezDevSDK_down_status status) {
    LOG(INFO) << "enter! ";
    return -1;
}

static int priority = NON_PLAY_PRIORITY;

int CTalk::RecvMsg(ezDevSDK_talk_msg_to_dev *msg) {
    if (msg == nullptr) {
        return -1;
    }
    HPR_BOOL bRet = HPR_FALSE;
    switch (msg->type) {
        case EZDEVSDK_TALK_ON_START_TALK: {
            auto *param = static_cast<ezDevSDK_talk_start_talk *>(msg->data);
            LOG(INFO) << "priority: " << param->priority;
            if (g_playing_priority <= param->priority) {
                LOG(INFO) << "skip start talk! playing priority " << g_playing_priority << " " << param->priority;
                return -1;
            }
            AudioPlayUtil::audio_stop();
            g_ctalk.do_fork();

            g_playing_priority = param->priority;
            priority = param->priority;
            LOG(INFO) << "ezdevsdk talk on start talk msg type is incoming, len: " << msg->len;
        }
        break;
        case EZDEVSDK_TALK_ON_STOP_TALK: {
            LOG(INFO) << "ezdevsdk talk on stop talk msg type is incoming.";
            AudioPlayUtil::audio_stop();
            g_playing_priority = NON_PLAY_PRIORITY;
        }
        break;

        case EZDEVSDK_TALK_ON_RECV_OTAP_MSG:
            LOG(INFO) << "ezdevsdk talk on recv otap msg msg type is incoming.";
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

int CTalk::GetRuntimeInfo(ezDevSDK_talk_runtime_info *info) {
    if (info == nullptr) {
        return -1;
    }
    switch (info->type) {
        case EZDEVSDK_TALK_RT_INFO_GET_ENCODE_TYPE:
            g_ctalk.GetEncodeType(info->data, info->len);
            break;
        default:
            break;
    }
    return 0;
}

int CTalk::RecvTalkData(int channel, char *data, int len) {
    LOG(INFO) << "data =====" << len << " writing to " << g_ctalk.pipefd[1];
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

int CTalk::do_fork() {
    if (pipe(g_ctalk.pipefd) == -1) {
        LOG(INFO) << "error open pipe.";
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        LOG(INFO) << "failed to fork.";
        return -1;
    }

    if (pid == 0) {
        // child process.
        dup2(g_ctalk.pipefd[0], STDIN_FILENO);
        close(g_ctalk.pipefd[0]);
        close(g_ctalk.pipefd[1]);
        g_playing_priority = priority;
        execlp("madplay", "madplay", "-", nullptr);
        PlayStatus::getInstance().setPlayId(asns::TALK_TASK_PLAYING);
        PlayStatus::getInstance().setPriority(g_playing_priority);
    }

    // parent process.
    close(g_ctalk.pipefd[0]); // close write

    return 0;
}

int CTalk::initMp3() {
    fp = fopen("/audiodata/test.mp3", "wb");
    if (!fp) {
        LOG(INFO) << "error open test.mp3";
        return -1;
    }
    return 0;
}

int CTalk::finiMp3() {
    if (fp) {
        fclose(fp);
        fp = nullptr;
    }
    return 0;
}

int CTalk::saveMp3(unsigned char *data, int len) {
    if (fp) {
        fwrite(data, 1, len, fp);
    } else {
        LOG(INFO) << "error write, fp is null.";
        return -1;
    }
    return 0;
}

int CTalk::CheckResource(int cur_count, int channel) {
    return 0;
}

int CTalk::RecvTalkDataNew(int channel, char *data, int len, int encode_type) {
    return 0;
}