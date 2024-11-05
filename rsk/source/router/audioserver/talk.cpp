#include "HPR/HPR_Utils.h"
#include "HPR/HPR_Thread.h"
#include "HPR/HPR_Error.h"
#include "ezDevSDK_talk.h"
#include "talk.h"
#include "utils.h"
#include "log.h"
#include "AudioPlayUtils.hpp"

#include <cstring>
#include <string_view>

extern HPR_VOIDPTR g_pKernel;
extern HPR_BOOL g_bAccessEz;
CTalk g_ctalk;
int g_playing_priority = NON_PLAY_PRIORITY;

HPR_BOOL CTalk::initBase() {
    ezDevSDK_base_function_init_info struInitInfo{};
    std::string_view logPath = "/tmp/ctalk.log";
    std::copy(logPath.begin(), logPath.end(), struInitInfo.log_info.path);
    struInitInfo.log_info.log_size = 2 * 1024 * 1024;
    struInitInfo.cb.recv_msg = BaseFunctionRecvMsg;
    struInitInfo.cb.get_runtime_info = BaseFunctionGetRuntimeInfo;
    struInitInfo.cb.ota_notifier = BaseFunctionOtaNotifier;
    struInitInfo.channel_num_max = 128;

    [[maybe_unused]] int iRet = ezDevSDK_base_function_init(&struInitInfo, g_bAccessEz, g_pKernel);
    return HPR_TRUE;
}

HPR_BOOL CTalk::init() {
    if (m_bInit) {
        LOG(INFO) << "ctalk is initialized.";
        return HPR_TRUE;
    }

    if (!g_pKernel) {
        LOG(INFO) << "ctalk init skipped since kernel is not ready";
        return HPR_FALSE;
    }

    initBase();
    ezDevSDK_talk_init_info struInitInfo{};
    struInitInfo.max_buf = 50 * 1024;
    struInitInfo.cb.recv_msg = RecvMsg;
    struInitInfo.cb.on_get_runtime_info = GetRuntimeInfo;
    struInitInfo.cb.on_check_resource = CheckResource;
    struInitInfo.cb.on_recv_talk_data = RecvTalkData;
    struInitInfo.cb.on_recv_talk_data_new = RecvTalkDataNew;

    int iRet = ezDevSDK_talk_init(&struInitInfo, g_bAccessEz, g_pKernel);
    LOG(INFO) << "ctalk init ret: " << iRet;
    if (iRet == 0) {
        m_bInit = true;
    }
    return HPR_TRUE;
}

int CTalk::BaseFunctionRecvMsg(ezDevSDK_base_function_msg_to_dev* msg) {
    LOG(INFO) << "enter!";
    return 0;
}

int CTalk::BaseFunctionGetRuntimeInfo(ezDevSDK_base_function_runtime_info* info) {
    LOG(INFO) << "enter!";
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

HPR_BOOL CTalk::GetEncodeType(void* pData, const HPR_UINT32 dwLen) {
    if (pData == nullptr || dwLen != sizeof(ezDevSDK_talk_encode_type)) {
        return HPR_FALSE;
    }
    auto* pEncodeType = static_cast<ezDevSDK_talk_encode_type*>(pData);
    LOG(INFO) << "old encode " << pEncodeType->encode << " reset to " << EZDEVSDK_AUDIO_PCM;
    pEncodeType->encode = EZDEVSDK_AUDIO_PCM;
    return HPR_TRUE;
}

int CTalk::BaseFunctionOtaNotifier(int percent, void* data, int data_len, ezDevSDK_down_err_code errcode, ezDevSDK_down_status status) {
    LOG(INFO) << "enter!";
    return -1;
}

int CTalk::RecvMsg(ezDevSDK_talk_msg_to_dev* msg) {
    if (msg == nullptr) {
        return -1;
    }
    switch (msg->type) {
        case EZDEVSDK_TALK_ON_START_TALK: {
            auto* param = static_cast<ezDevSDK_talk_start_talk*>(msg->data);
            LOG(INFO) << "priority: " << param->priority;
            if (g_playing_priority <= param->priority) {
                LOG(INFO) << "skip start talk! playing priority " << g_playing_priority << " " << param->priority;
                return -1;
            }
            AudioPlayUtil::audio_stop();
            break;
        }
        case EZDEVSDK_TALK_ON_STOP_TALK:
            break;
        default:
            break;
    }
    return 0;
}

//BY GST ARMV8 GCC 13.2