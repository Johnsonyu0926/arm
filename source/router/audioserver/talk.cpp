#include "HPR/HPR_Utils.h"
#include "HPR/HPR_Thread.h"
#include "HPR/HPR_Error.h"
#include "ezDevSDK_talk.h"
#include "talk.h"

extern HPR_VOIDPTR g_pKernel;
extern HPR_BOOL g_bAccessEz;
CTalk g_ctalk;
int g_bStop = 1;

int g_playing_priority = NON_PLAY_PRIORITY;
// extern CBroadcastPlanBusiness g_plan;
HPR_BOOL CTalk::initBase()
{
    ezDevSDK_base_function_init_info struInitInfo = {0};
    HPR_Snprintf(struInitInfo.log_info.path, sizeof(struInitInfo.log_info.path) - 1, "./log");
    struInitInfo.log_info.log_size = 2 * 1024 * 1024;
    struInitInfo.cb.recv_msg = BaseFunctionRecvMsg;
    struInitInfo.cb.get_runtime_info = BaseFunctionGetRuntimeInfo;
    struInitInfo.cb.ota_notifier = BaseFunctionOtaNotifier;
    struInitInfo.channel_num_max = 128;
    int iRet = ezDevSDK_base_function_init(&struInitInfo, g_bAccessEz, g_pKernel);

    return HPR_TRUE;
}

HPR_BOOL CTalk::init()
{
    if (m_bInit)
    {
        // printf("ctalk is inited.\n");
        return HPR_TRUE;
    }
    if (!g_pKernel)
    {
        // printf("ctalk init skip since kernel is not ready\n");
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
    // printf("ctalk init ret:%d\n", iRet);
    if (iRet == 0)
    {
        m_bInit = 1;
    }
    return HPR_TRUE;
}
int CTalk::BaseFunctionRecvMsg(ezDevSDK_base_function_msg_to_dev *msg)
{
    // printf("%s %d enter!\n", __FUNCTION__, __LINE__);
    return 0;
}
int CTalk::BaseFunctionGetRuntimeInfo(ezDevSDK_base_function_runtime_info *info)
{
    // printf("%s %d enter!\n", __FUNCTION__, __LINE__);
    if (info == NULL)
    {
        return -1;
    }
    switch (info->type)
    {
    case EZDEVSDK_TALK_RT_INFO_GET_ENCODE_TYPE:
        g_ctalk.GetEncodeType(info->data, info->len);
        break;
    default:
        break;
    }
    return 0;
}

HPR_BOOL CTalk::GetEncodeType(void *pData, const HPR_UINT32 dwLen)
{
    if (pData == NULL || dwLen != sizeof(ezDevSDK_talk_encode_type))
    {
        return HPR_FALSE;
    }
    ezDevSDK_talk_encode_type *pEncodeType = (ezDevSDK_talk_encode_type *)pData;
    // printf("old encode %d , reset to %d\n", pEncodeType->encode, EZDEVSDK_AUDIO_PCM);
    pEncodeType->encode = EZDEVSDK_AUDIO_PCM; // EZDEVSDK_AUDIO_AAC;
    return HPR_TRUE;
}

int CTalk::BaseFunctionOtaNotifier(int percent, void *data, int data_len, ezDevSDK_down_err_code errcode, ezDevSDK_down_status status)
{
    // printf("%s %d enter!\n", __FUNCTION__, __LINE__);
    return -1;
}
int CTalk::RecvMsg(ezDevSDK_talk_msg_to_dev *msg)
{
    if (NULL == msg)
    {
        return -1;
    }
    HPR_BOOL bRet = HPR_FALSE;
    switch (msg->type)
    {
    case EZDEVSDK_TALK_ON_START_TALK:
    {

        ezDevSDK_talk_start_talk *param = (ezDevSDK_talk_start_talk *)(msg->data);
        // printf("priority:%d\n", param->priority);
        if (g_playing_priority < param->priority)
        {
            // printf("skip start talk! playing priority  %d %d\n", g_playing_priority, param->priority);
            return -1;
        }
        system("killall -9 madplay");
        // g_ctalk.initMp3();
        g_bStop = 0;
        g_ctalk.do_fork();
        // printf("ezdevsdk talk on start talk msg type is incoming , len:%d,\n", msg->len);
    }
    break;
    case EZDEVSDK_TALK_ON_STOP_TALK:
    {
        if (g_bStop)
        {
            printf("skip stop.\n");
            return -1;
        }
        g_bStop = 1;
        // printf("ezdevsdk talk on stop talk msg type is incoming.\n");
        system("killall -9 madplay");
    }
    break;

    case EZDEVSDK_TALK_ON_RECV_OTAP_MSG:
        // printf("ezdevsdk talk on recv otap msg msg type is incoming.\n");
        //  bRet = g_ctalk.ProcessTalk(msg);
        break;
    default:
        // printf("unknown msg type:%d\n", msg->type);
        break;
    }
    if (bRet)
    {
        return 0;
    }
    return 0;
}

int CTalk::GetRuntimeInfo(ezDevSDK_talk_runtime_info *info)
{
    if (info == NULL)
    {
        return -1;
    }
    switch (info->type)
    {
    case EZDEVSDK_TALK_RT_INFO_GET_ENCODE_TYPE:
        g_ctalk.GetEncodeType(info->data, info->len);
        break;
    default:
        break;
    }
    return 0;
}

int CTalk::RecvTalkData(int channel, char *data, int len)
{
    // printf("data =====%d writing to %d\n", len, g_ctalk.pipefd[1]);
    if (g_ctalk.pipefd[1] < 0)
    {
        // printf("pipe is not open.\n");
        return -1;
    }

    int num = write(g_ctalk.pipefd[1], data, len);
    if (num < 0)
    {
        // printf("failed to write pipe! %d\n", g_ctalk.pipefd[1]);
        system("killall -9 madplay");
        return -1;
    }
    return 0;
}
int CTalk::do_fork()
{
    if (pipe(g_ctalk.pipefd) == -1)
    {
        printf("error open pipe.\n");
        return -1;
    }

    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        printf("failed to fork.\n");
        return -1;
    }

    if (pid == 0)
    {
        // child process.
        dup2(g_ctalk.pipefd[0], STDIN_FILENO);
        close(g_ctalk.pipefd[0]);
        close(g_ctalk.pipefd[1]);
        execlp("madplay", "madplay", "-", NULL);
    }

    // parent process.
    close(g_ctalk.pipefd[0]); // close write

    return 0;
}

int CTalk::initMp3()
{
    fp = fopen("/audiodata/test.mp3", "wb");
    if (!fp)
    {
        printf("error open test.mp3");
        return -1;
    }
    return 0;
}
int CTalk::finiMp3()
{
    if (fp)
    {
        fclose(fp);
    }
    return 0;
}
int CTalk::saveMp3(unsigned char *data, int len)
{
    if (fp)
    {
        fwrite(data, 1, len, fp);
    }
    else
    {
        printf("error write, fp is null.\n");
        return -1;
    }
    return 0;
}

int CTalk::CheckResource(int cur_count, int channel)
{
    return 0;
}

int CTalk::RecvTalkDataNew(int channel, char *data, int len, int encode_type)
{
    return 0;
}
