#ifndef __CTALK_H__
#define __CTALK_H__

#include "ezDevSDK_talk_def.h"
#include "ezDevSDK_base_function.h"
#include "HPR_Types.h"

#define NON_PLAY_PRIORITY 100
class CBlock
{
public:
    CBlock() {}
    ~CBlock()
    {
        if (buf)
        {
            free(buf);
        }
    }
    char *buf;
    int len;
};
class CTalk
{
public:
    CTalk()
    {
        m_bInit = 0;

        fp = NULL;
    }
    ~CTalk() {}

private:
    FILE *fp = NULL;
    int m_bInit;
    int initBase();

protected:
    static int RecvMsg(ezDevSDK_talk_msg_to_dev *msg);           ///< 消息回调接口，不可以阻塞，主动接收消息和回调方式二选一，实现了回调接口，消息就走回调函数
    static int GetRuntimeInfo(ezDevSDK_talk_runtime_info *info); ///< SDK运行时获取信息回调，不能有任何阻塞操作
    static int RecvTalkData(int channel, char *data, int len);
    static int CheckResource(int cur_count, int channel);
    static int RecvTalkDataNew(int channel, char *data, int len, int encode_type); // NPQ对讲走这个回调

    static int BaseFunctionRecvMsg(ezDevSDK_base_function_msg_to_dev *msg);
    static int BaseFunctionGetRuntimeInfo(ezDevSDK_base_function_runtime_info *info);

    static int BaseFunctionOtaNotifier(int percent, void *data, int data_len, ezDevSDK_down_err_code errcode, ezDevSDK_down_status status);

public:
    HPR_BOOL init();
    HPR_BOOL GetEncodeType(void *pData, const HPR_UINT32 dwLen);
    int initMp3();
    int finiMp3();
    int saveMp3(unsigned char *data, int len);
    int do_fork();
    int pipefd[2];
};

extern CTalk g_ctalk;

#endif
