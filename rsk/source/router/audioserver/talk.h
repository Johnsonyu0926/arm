// talk.h
#ifndef __CTALK_H__
#define __CTALK_H__

#include <cstdio>
#include <memory>
#include <array>
#include "ezDevSDK_talk_def.h"
#include "ezDevSDK_base_function.h"
#include "HPR_Types.h"

inline constexpr int NON_PLAY_PRIORITY = 100;

class CBlock {
public:
    CBlock() = default;
    ~CBlock() = default;

    std::unique_ptr<char[]> buf;
    int len{0};
};

class CTalk {
public:
    CTalk() : m_bInit(false), fp(nullptr) {}
    ~CTalk() = default;

    CTalk(const CTalk&) = delete;
    CTalk& operator=(const CTalk&) = delete;
    CTalk(CTalk&&) noexcept = default;
    CTalk& operator=(CTalk&&) noexcept = default;

private:
    FILE* fp;
    bool m_bInit;
    int initBase();

protected:
    static int RecvMsg(ezDevSDK_talk_msg_to_dev* msg);
    static int GetRuntimeInfo(ezDevSDK_talk_runtime_info* info);
    static int RecvTalkData(int channel, char* data, int len);
    static int CheckResource(int cur_count, int channel);
    static int RecvTalkDataNew(int channel, char* data, int len, int encode_type);
    static int BaseFunctionRecvMsg(ezDevSDK_base_function_msg_to_dev* msg);
    static int BaseFunctionGetRuntimeInfo(ezDevSDK_base_function_runtime_info* info);
    static int BaseFunctionOtaNotifier(int percent, void* data, int data_len, ezDevSDK_down_err_code errcode, ezDevSDK_down_status status);

public:
    [[nodiscard]] bool init();
    [[nodiscard]] bool GetEncodeType(const void* pData, HPR_UINT32 dwLen);
    int initMp3();
    int finiMp3();
    int saveMp3(const unsigned char* data, int len);
    int do_fork();

    std::array<int, 2> pipefd{};
};

extern CTalk g_ctalk;

#endif // __CTALK_H__

//BY GST ARMV8 GCC 13.2