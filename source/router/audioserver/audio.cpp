#include "HCEBase.h"

#include "mbedtls/md5.h"
#include "mbedtls/sha256.h"
#include "mbedtls/md.h"
#include "mbedtls/pkcs5.h"

#include "HPR/HPR_String.h"
#include "HPR/HPR_Utils.h"
#include "HPR/HPR_Mutex.h"

#include "talk.h"
#include "broadcast_plan.hpp"
#include "volume.hpp"
#include "audiocfg.hpp"
#include "doorsbase.h"
#include "serverthread.h"
#include "udpclient.hpp"
#include "login.hpp"

#include "hkVolume.hpp"
#include "hkTts.hpp"
// #include "login.hpp"
#include <csignal>
using namespace asns;
using namespace std;

/* --- server -- */
extern int g_nDebugMode;
extern char *optarg;

#define EZVIA_STREAM 1

#ifdef EZVIA_STREAM

#include "HCEStream.h"

#endif

#define MAX_PATH 128
#define MAX_CLIENTS 128

HPR_BOOL g_bexception = HPR_FALSE;
HPR_BOOL g_bAyncConnect = HPR_FALSE;

char dev_id[33];     //= "e118dfec13094accbe3be608463a9324";
char master_key[17]; // = "38A7D56C5DA56562";

HPR_Mutex g_mlock;

HPR_BOOL g_bException = HPR_FALSE;
HPR_VOIDPTR g_pKernel = NULL;
HPR_BOOL g_bAccessEz = HPR_FALSE;

CAddCustomAudioFileBusiness g_addAudioBusiness;
CBroadcastPlanBusiness g_plan;
CAudioCfgBusiness g_audiocfg;
CVolumeSet g_volumeSet;
//
// CVolumeSet g_volume;

char save_prefix[128];

// vector<int> playing;

#define DEBUG_MODE_ALL 5
void CALLBACK loginfo(HPR_UINT32 dwLevel, char const *pBuffer, HPR_VOIDPTR pUserData)
{
    if (g_nDebugMode == DEBUG_MODE_ALL)
    {
        printf("[SDKLOG] %s\n", pBuffer);
    }
}

HPR_VOID hexdump(unsigned const char *pSrc, int iLen, int iUpper, unsigned char *pDst) {
    int i;
    char fmt[8];

    if (iUpper) {
        strcpy(fmt, "%02X");
    } else {
        strcpy(fmt, "%02x");
    }

    for (i = 0; i < iLen; ++i) {
        sprintf((char *) pDst + 2 * i, fmt, pSrc[i]);
    }
}

void otap_generate_sharekey(unsigned char *pDevVerificationCode, unsigned char *pDevSubSerial, unsigned char *pshare_key) {
    // printf("code:%s, serial:%s\n", pDevVerificationCode, pDevSubSerial);

    unsigned char sharekey_src[512];
    HPR_UINT16 sharekey_src_len;

    HPR_UINT8 szEhomeKey256[32] = {0};

    unsigned char sharekey_dst[16];
    unsigned char sharekey_dst_hex[MAX_EHOME50_KEY_LEN + 1]; // mbedtls_hexdump 锟斤拷锟斤拷\0锟斤拷锟斤拷

    memset(sharekey_src, 0, 512);
    memset(sharekey_dst, 0, 16);
    memset(sharekey_dst_hex, 0, MAX_EHOME50_KEY_LEN + 1);

    sharekey_src_len = strlen((char *) pDevVerificationCode);

    memcpy(sharekey_src, pDevVerificationCode, sharekey_src_len);
    memcpy(sharekey_src + sharekey_src_len, pDevSubSerial, strlen((const char *) pDevSubSerial));

    sharekey_src_len += strlen((const char *) pDevSubSerial);

    mbedtls_md5(sharekey_src, sharekey_src_len, sharekey_dst);

    hexdump(sharekey_dst, 16, 1, sharekey_dst_hex);

    memset(sharekey_src, 0, 512);

    const char *pShareKeySalt = "www.88075998.com";
    // printf("salt:%s\n", pShareKeySalt);

    const HPR_UINT32 dwShareKeySaltLen = strlen(pShareKeySalt);

    memcpy(sharekey_src, sharekey_dst_hex, MAX_EHOME50_KEY_LEN);
    memcpy(sharekey_src + 32, pShareKeySalt, dwShareKeySaltLen);

    sharekey_src_len = MAX_EHOME50_KEY_LEN + dwShareKeySaltLen;

    mbedtls_md5(sharekey_src, sharekey_src_len, sharekey_dst);
    hexdump(sharekey_dst, 16, 1, sharekey_dst_hex);

    memset(sharekey_src, 0, 512);
    memcpy(sharekey_src, sharekey_dst_hex, MAX_EHOME50_KEY_LEN);
    sharekey_src_len = MAX_EHOME50_KEY_LEN;

    mbedtls_md5(sharekey_src, sharekey_src_len, sharekey_dst);
    hexdump(sharekey_dst, 16, 1, sharekey_dst_hex);

    mbedtls_md_context_t sha1_ctx;
    const mbedtls_md_info_t *info_sha1;
    info_sha1 = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if (info_sha1 == NULL) {
        printf("error! info sha is null!\n");
        return;
    }
    if (mbedtls_md_setup(&sha1_ctx, info_sha1, 1) != 0) {
        printf("error! mbedtls md setup failed.\n");
        return;
    }
    unsigned char sharekey_sha256_dst[MAX_EHOME50_KEY_LEN];
    unsigned char sharekey_sha256_dst_hex[MAX_EHOME50_KEY_LEN * 2 + 1]; // mbedtls_hexdump 锟斤拷锟斤拷\0锟斤拷锟斤拷
    int ret = mbedtls_pkcs5_pbkdf2_hmac(&sha1_ctx, sharekey_dst_hex, MAX_EHOME50_KEY_LEN,(const unsigned char *) pShareKeySalt, dwShareKeySaltLen, 3,MAX_EHOME50_KEY_LEN, sharekey_sha256_dst);
    hexdump(sharekey_sha256_dst, MAX_EHOME50_KEY_LEN, 1, sharekey_sha256_dst_hex);

    mbedtls_md_free(&sha1_ctx);

    memcpy(pshare_key, sharekey_sha256_dst_hex + 10, MAX_EHOME50_KEY_LEN);
    // printf("success gen key: %s\n", pshare_key);
}

int state_machine = 0;
char identifier[64] = {0};

NET_IOT_CLIENT_COMMAND client_cmd = {0};
NET_EBASE_CLIENT_DATA struData = {0};

// char resp[] = "{\"status\": 200,\"code\": \"0x00000000\",\"errorMsg\":\"\",\"data\":{\"Value\": {\"deviceName\":\"SDX1001\",\"deviceDescription\": \"IP BC Term\",\"deviceLocation\": \"This\",\"model\": \"DS-KL7601-S\",\"serialNumber\": \"SDX1001\",\"subSerialNumber\": \"SDX1001\"}}}";
char resp[1024];
#define RESP_FMT "{\"status\": 200,\"code\": \"0x00000000\",\"errorMsg\":\"\",\"data\":{\"Value\": {\"deviceName\":\"%s\",\"deviceDescription\": \"IP BC Term\",\"deviceLocation\": \"This\",\"model\": \"DS-KL7601-S\",\"serialNumber\": \"%s\",\"subSerialNumber\": \"%s\"}}}"

char channel_resp[] = "{\"status\":200,\"code\":\"0x00000000\",\"errorMsg\":\"success!\",\"data\":{\"ChannelList\":[{\"ID\":1}]}}";

char resp_get_audio[] = "{\"status\":200,\"code\":\"0x00000000\",\"errorMsg\":\"\",\"data\":{\"CustomAudioInfoList\":[{\"customAudioID\":1,\"customAudioName\":\"test.mp3\",\"customAudioPath\":\"/home/ftp/NAS2007/\",\"audioFileFormat\":\"mp3\",\"audioFileSize\":0,\"audioFileDuration\":0}]}}";

char resp_add_broadcast_plan[] = "{\"status\":200,\"code\":\"0x00000000\",\"errorMsg\":\"\",\"data\":{}}";
char resp_add_custom_audio_file[] = "{\"status\":200,\"code\":\"0x00000000\",\"errorMsg\":\"\",\"data\":{}}";

char resp_start_voice_talk[] = "";

HPR_INT32 g_dwHandle = -1;

void CALLBACK FuncClientSession(HPR_INT32 dwHandle, enumEBaseDataType dwType, HPR_UINT32 dwLen, HPR_VOIDPTR pData, HPR_VOIDPTR pUserData)
{
    // printf("dwtype=%d, dwHandle = %d\n", dwType, dwHandle);
    if (dwType == DATATYPE_IOT)
    {
        g_dwHandle = dwHandle;

        LPNET_EBASE_SERVER_DATA pTemp = (LPNET_EBASE_SERVER_DATA) pData;

        LPNET_IOT_SERVER_COMMAND cmd = (LPNET_IOT_SERVER_COMMAND)(pTemp->pCmdIdentify);
        // printf("cmd: size:%d, type:%d, method:%d, business type:%d\n", cmd->dwSize, cmd->wModuleType, cmd->wMethod, cmd->wBusinessType);
        // printf("resource id:%s\n", cmd->byResourceID);
        // printf("resource type:%s\n", cmd->byResourceType);
        // printf("identifier:%s\n", cmd->byIdentifier);

        memset(identifier, 0, sizeof(identifier));
        strncpy(identifier, (char *)cmd->byIdentifier, sizeof(identifier));

        // printf("module:%s\n", cmd->byModule);
        // printf("method:%s\n", cmd->byMethod);
        // printf("msg type:%s\n", cmd->byMsgType);

        client_cmd.dwSize = sizeof(client_cmd);
        client_cmd.wModuleType = cmd->wModuleType;
        client_cmd.wMethod = cmd->wMethod;
        client_cmd.wBusinessType = cmd->wBusinessType;
        memcpy(client_cmd.byResourceID, cmd->byResourceID, sizeof(client_cmd.byResourceID));
        memcpy(client_cmd.byResourceType, cmd->byResourceType, sizeof(client_cmd.byResourceType));
        memcpy(client_cmd.byIdentifier, cmd->byIdentifier, sizeof(client_cmd.byIdentifier));
        memcpy(client_cmd.byModule, cmd->byModule, sizeof(client_cmd.byModule));
        memcpy(client_cmd.byMethod, cmd->byMethod, sizeof(client_cmd.byMethod));
        memcpy(client_cmd.byMsgType, cmd->byMsgType, sizeof(client_cmd.byMsgType));
        memcpy(client_cmd.byDomainID, cmd->byDomainID, sizeof(client_cmd.byDomainID));
        client_cmd.dwmMulIndex = cmd->dwmMulIndex;

        struData.dwSize = sizeof(NET_EBASE_CLIENT_DATA);
        struData.dwDataType = DATATYPE_IOT;
        if (strcmp(identifier,"GetVoiceTalkChannelList") == 0)
        {
            struData.pBodyData = (HPR_VOIDPTR)(channel_resp);
        }
        else if (strcmp(identifier, "GetCustomAudioCfg") == 0)
        {
            struData.pBodyData = (HPR_VOIDPTR)(resp_get_audio);
        }
        else if (strcmp(identifier,"AddBroadcastPlan") == 0)
        {
            struData.pBodyData = (HPR_VOIDPTR)(resp_add_broadcast_plan);
            printf("broadcast plan json:%s\n", (char *)pTemp->pDodyData);
            g_plan.parseRequest((char *)pTemp->pDodyData);
        }
        else if (strcmp(identifier,"AddCustomAudioFile") == 0)
        {
            printf("json:%s\n", (char *)pTemp->pDodyData);
            g_addAudioBusiness.add((char *)pTemp->pDodyData);
            struData.pBodyData = (HPR_VOIDPTR)(resp_add_custom_audio_file);
        } else if (strcmp(identifier, "BroadcastAudioCfgList") == 0) {
            printf("json:%s\n", (char *)pTemp->pDodyData);
            CHKVolume v;
            v.parse((char *)pTemp->pDodyData);
            char res[64] = {0};
            sprintf(res,resp_add_custom_audio_file,"success");
            struData.pBodyData = (HPR_VOIDPTR)(res);
		}
        else if(strcmp(identifier, "StartTTSAudio") == 0) {
            printf("json:%s\n", (char *)pTemp->pDodyData);
            CStartTTSAudio ttsAudio;
            std::string res = ttsAudio.parse((char *)pTemp->pDodyData);
            struData.pBodyData = (HPR_VOIDPTR)(res.c_str());
        }
        else
        {
            printf("unknown body:%s\n", (char *)pTemp->pDodyData);
            struData.pBodyData = (HPR_VOIDPTR)(resp);
            printf("resp = %s\n", resp);
        }
        struData.dwBodyLen = strlen(resp);
        struData.dwSequence = pTemp->dwSequence;
        struData.pCommandType = &client_cmd;

        // printf("call back done! dwSize:%d, data type:%d, bodylen:%d seq:%d\n", struData.dwSize, struData.dwDataType, struData.dwBodyLen, struData.dwSequence);

        state_machine = dwType;
    }
    else if (dwType == DATETYPE_EXPECTION)
    {
        g_bException = HPR_TRUE;
    }
    else if (dwType == DATATYPE_IOT_KERNEL_READY)
    {
        // printf("KERNEL READY!\n");
        g_pKernel = ((NET_EBASE_IOT_KERNEL_READY_INFO *) pData)->pKernelPtr;
        g_ctalk.init();
    }
}

void CALLBACK KeyValueLoadFunc(HPR_INT32 dwHandle,EBaseIoTKeyValueType dwKeyValueType,unsigned char *pData, HPR_UINT32 dwLen,HPR_VOIDPTR pUserData)
{
    // g_mlock.Lock();
    //  printf("size111111111111111111111:[%s]\r\n", dev_id[dwHandle]);
    // ReadKeValueXML(dev_id[dwHandle], 32, master_key[dwHandle], 16, dwHandle);
    if (dwKeyValueType == KEYVALUE_DEVICE_ID)
    {
        if (strlen(dev_id) > 0)
        {
            memcpy(pData, dev_id, dwLen); // todo，这两个KEYVALUE是什么作用，取什么值，在哪配
        }
    }else if (dwKeyValueType == KEYVALUE_MASTER_KEY)
    {
        if (strlen(master_key) > 0)
        {
            memcpy(pData, master_key, dwLen); // todo
        }
    }
    // g_mlock.Unlock();
}

void CALLBACK KeyValueSaveFunc(HPR_INT32 dwHandle,EBaseIoTKeyValueType dwKeyValueType,unsigned char *pData, HPR_UINT32 dwLen,HPR_VOIDPTR pUserData)
{
    // g_mlock.Lock();
    if (dwKeyValueType == KEYVALUE_DEVICE_ID)
    {
        memcpy(dev_id, pData, dwLen);
        // printf("dev_id is got = %s\n", dev_id);
    }else if (dwKeyValueType == KEYVALUE_MASTER_KEY)
    {
        memcpy(master_key, pData, dwLen);
    // printf("master key is got:%s\n", master_key);
}

    // if (strlen(dev_id[dwHandle]) > 0 && strlen(master_key[dwHandle]) > 0)
    //{
    //   WriteKeValueXML(dev_id[dwHandle], strlen(dev_id[dwHandle]), master_key[dwHandle], strlen(master_key[dwHandle]), dwHandle);
    //}
    // printf("size222222222222222222222:[%s]\r\n", dev_id[dwHandle]);
    // g_mlock.Unlock();
}

HPR_BOOL init_otap_reg_info(NET_EBASE_IOT_REGINFO *pRegInfo) {
    NET_EBASE_IOT_REGINFO *pOTAPRegInfo = NULL;
    pOTAPRegInfo = (NET_EBASE_IOT_REGINFO *) pRegInfo;
    pOTAPRegInfo->dwSize = sizeof(pRegInfo);
    pOTAPRegInfo->dwDevType = 1;
    pOTAPRegInfo->dwState = 1;
    pOTAPRegInfo->fnCB = FuncClientSession;
    pOTAPRegInfo->fnCBKeyValueLoad = KeyValueLoadFunc;
    pOTAPRegInfo->fnCBKeyValueSave = KeyValueSaveFunc;
    pOTAPRegInfo->byKeyValueMethod = 1;
    pOTAPRegInfo->byAsynConnect = 0; //采用同步方式
    pOTAPRegInfo->byDevInfoType = 1; // 1-设备信息通过结构体方式传入SDK

    // memcpy(pOTAPRegInfo->byServerAdd,  "10.40.240.121",sizeof(pOTAPRegInfo->byServerAdd) - 1);
    memcpy(pOTAPRegInfo->byServerAdd, g_audiocfg.business[0].server.c_str(), sizeof(pOTAPRegInfo->byServerAdd) - 1);
    pOTAPRegInfo->wServerPort = g_audiocfg.business[0].port;
    HPR_Strncpy((char *) pOTAPRegInfo->byDeviceID, g_audiocfg.business[0].deviceID.c_str(),sizeof(pOTAPRegInfo->byDeviceID) - 1);
    otap_generate_sharekey((unsigned char *) g_audiocfg.business[0].password.c_str(),(unsigned char *) pOTAPRegInfo->byDeviceID, (unsigned char *) pOTAPRegInfo->byShareKey);
    HPR_Strncpy((char *) pOTAPRegInfo->byDeviceSerial, g_audiocfg.business[0].serial.c_str(),sizeof(pOTAPRegInfo->byDeviceSerial));
    HPR_Strncpy((char *) pOTAPRegInfo->bySubSerial, g_audiocfg.business[0].subSerial.c_str(),sizeof(pOTAPRegInfo->bySubSerial));
    HPR_Strncpy((char *) pOTAPRegInfo->byDevName, g_audiocfg.business[0].devName.c_str(),sizeof(pOTAPRegInfo->byDevName));

    g_bAyncConnect = pOTAPRegInfo->byAsynConnect;

    return HPR_TRUE;
}

int do_state_machine() {
    if (state_machine == DATATYPE_IOT) {
         printf("state machine is IOT. sending response..., identifier=%s\n", identifier);
        if (NET_EBASE_Reponse(g_dwHandle, &struData)) {
             printf("Send iot data to server sucess\r\n");
        } else {
            HPR_UINT32 dwError = NET_EBASE_GetLastError();
            printf("Send iot data to server failed ,error:%d\r\n", dwError);
        }
        state_machine = -1;
    }
}

HPR_VOID loop_work(HPR_UINT32 dwClientType,NET_EBASE_IOT_REGINFO struOTAPRegInfo)
{
    // cout << "loop work enter." << endl;
    HPR_INT32 iHandle = -1; // (HPR_INT32)HPR_INVALID_HANDLE;
    HPR_BOOL bConnect = HPR_FALSE;
    while (1)
    {
        if (g_bException)
        {
        // printf("exception !\n");
            if (iHandle >= 0)
            {
                if (!NET_EBASE_DeystoryClient(iHandle))
                {
                // printf("destroy client error.\n");
                }
                iHandle = -1; // (HPR_INT32)HPR_INVALID_HANDLE;
            }
            g_bException = HPR_FALSE;
            bConnect = HPR_FALSE;
        }

        if (bConnect)
        {
            do_state_machine();

            // printf("connected. ok. \n");
            HPR_Sleep(1000);
            continue;
        }

        if (iHandle < 0)
        {
            // cout << "iot client is creating now." << endl;

            iHandle = NET_EBASE_CreateIOTClient(dwClientType);

            // cout << "iHandle is created:" << iHandle << endl;

            if (iHandle < 0)
            {
                HPR_UINT32 dwError = NET_EBASE_GetLastError();
                NET_EBASE_OTAP_ERROR_INFO err;
                NET_EBASE_GetOTAPErrorMsg(dwError, &err);
                // printf("failed to create iot client. type:%d, errno:%d, msg:%s, statusCode:%d\n", dwClientType, dwError, err.szErrorMsg, err.dwStatusCode);
                HPR_Sleep(1000);
                continue;
            }
        }
        // printf("connect to iot server by iot client...\n");
        bConnect = NET_EBASE_ConnectToIOTServer(iHandle, &struOTAPRegInfo);
        if (!bConnect)
        {
            HPR_UINT32 dwError = NET_EBASE_GetLastError();
            // cout << "connect to iot server by iot client failed... dwError:" << dwError << endl;
            HPR_Sleep(1000);
        } else if (strcmp(identifier, "")){

        } else {

            HPR_UINT32 dwErrorCode = 0x00100006;
            NET_EBASE_OTAP_ERROR_INFO struErrorInfo = {0};
            HPR_BOOL bRet = NET_EBASE_GetOTAPErrorMsg(dwErrorCode, &struErrorInfo);
            if (bRet)
            {
                cout << "success to connect to server." <<endl;
            }
            else
            {
                HPR_UINT32 dwError = NET_EBASE_GetLastError();
            }

        // cout << "connect to iot server by iot client success" << endl;
            HPR_UINT16 wInterval = 10000;
            NET_EBASE_SetParam(iHandle, EBASE_PARAM_ALIVE_INTERVAL, &wInterval,
            sizeof(wInterval));
        }
    }
}

/* -- server --*/

int PrintVersion() {
    CAudioCfgBusiness cfg;
    cfg.load();
    printf("audioserver version %s\n", cfg.business[0].codeVersion.c_str());
    return 0;
}

int Usage() {
    char szUsage[] = "audioserver options\n\
options:\n\
	-p xx: port\n\
For example:\n\
\n\
		 ./audioserver -p 10005 -d 5\n\
			\n";

    PrintVersion();
    printf(szUsage);
    exit(0);
    return 0;
}

#include "mqttManage.hpp"
#include "rs485Manage.hpp"
#include <thread>

#define AUDIO_DATA_DIR "/audiodata"
#define AUDIO_CFG_DIR	"/cfg"
#define AUDIO_TTS_DIR "/tts"

int prepare_work_dir(string prefix)
{
	string audio_data_dir = prefix + AUDIO_DATA_DIR;
	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd),  "mkdir -p %s", audio_data_dir.c_str());
	system(cmd);
	cout<<"exec cmd:"<<cmd<<endl;

	string audio_cfg_dir = prefix + AUDIO_CFG_DIR;
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd),  "mkdir -p %s", audio_cfg_dir.c_str());
	system(cmd);
	cout<<"exec cmd:"<<cmd<<endl;

    string audio_tts_dir = prefix + AUDIO_TTS_DIR;
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd),  "mkdir -p %s", audio_tts_dir.c_str());
    system(cmd);
    cout<<"exec cmd:"<<cmd<<endl;
}
void signal_handler(int signal) {
    std::cout << "exit" << std::endl;
    exit(0);
}
int main(int argc, char **argv) {
    int op;
    printf("start audio program...\n");
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    std::signal(SIGINT, signal_handler);
    int nPort = 34508;

    if (argc < 2) {
        Usage();
    }

    while ((op = getopt(argc, argv, "p:d:v")) != EOF) {
        switch (op) {
            case 'd':
                g_nDebugMode = atoi(optarg);
                break;
            case 'p':
                nPort = atoi(optarg);
                break;
            case 'v':
                PrintVersion();
                break;
            default:
                Usage();
                break;
        };
    }

    CServerThread *pServer = new CServerThread();
    pServer->SetPort(nPort);
    pServer->CreateThread();

    // cout << "server start success, audio running..." << endl;

    if (g_audiocfg.load() < 0) {
        printf("failed to load audio cfg.\n");
        return -1;
    }
    sprintf(resp, RESP_FMT, g_audiocfg.business[0].devName.c_str(), g_audiocfg.business[0].serial.c_str(),g_audiocfg.business[0].subSerial.c_str());

    CUtils utils;
    asns::CLoginData data;
    utils.async_wait(0, 0, 30, [&] {
        data.do_success();
        json js = data;
        std::string str = js.dump();
        utils.udp_multicast_send("239.255.255.235", 5099, str);
    });

    RSBusinessManage rs;
    std::thread([&] {
        rs.worker();
    }).detach();

    CAudioCfgBusiness cfg;
    cfg.load();

	prepare_work_dir(cfg.business[0].savePrefix);

    if (cfg.business[0].serverType == 2) {
        MqttManage mqttManage;
        mqttManage.start();
    } else if (cfg.business[0].serverType == 1) {
#ifndef EZVIA_STREAM
        NET_ESTREAM_ABILITY_CFG struAb = {0};
    struAb.dwBitStreamCount = 1;
    NET_OTAP_STREAM_INIT_PARAM struEstreamInitParam = {0};
    NET_OTAP_STREAM_INIT_INFO struStreamInitInfo = {0};
    struStreamInitInfo.struCallBack.fnRecvMsgCB = CStreamTest::RecvMsgCB;
    struStreamInitInfo.struCallBack.fnStreamDataCB = CStreamTest::RecvStreamCB;
    struStreamInitInfo.struCallBack.fnExceptionCB = CStreamTest::ExceptionCallback;
    struEstreamInitParam.pStreamInitInfo = &struStreamInitInfo;
    struEstreamInitParam.pEstreamAbility = &struAb;
    NET_OTAP_ESTREAM_Init(&struEstreamInitParam);
#endif

        NET_EBASE_ABILITY_CFG_EX struEStreamAbilityEx = {0};
        struEStreamAbilityEx.dwClientCnt = 256;
        NET_EBASE_InitEx(&struEStreamAbilityEx);

        NET_EBASE_LOGINFO li = {0};
        li.dwLevel = 3;
        li.dwSize = sizeof(NET_EBASE_LOGINFO);
        li.fnLogFunc = loginfo;
        NET_EBASE_SetLogInfo(&li);

        HPR_UINT32 dwClientType = REGISTER_PLATFORM_OTAP; //以OTAP协议接入私有化部署平台

        NET_EBASE_IOT_REGINFO_EXTEND struExDevInfo = {0};
        memcpy(struExDevInfo.byDevType, "HCIotDevSDKTest", strlen("HCIotDevSDKTest"));
        memcpy(struExDevInfo.byDevTypeDisplay, "HCIotDevSDKTest", strlen("HCIotDevSDKTest"));
        memcpy(struExDevInfo.byMAC, "H276E047CF03", strlen("H276E047CF03"));
        memcpy(struExDevInfo.byFirmwareVer, "V5.2.0 build 210705", sizeof(struExDevInfo.byFirmwareVer));

        NET_EBASE_IOT_REGINFO struOTAPRegInfo = {0};

        init_otap_reg_info(&struOTAPRegInfo);

        struOTAPRegInfo.pExDevInfo = &struExDevInfo;

        g_plan.load();
        g_volumeSet.load();
        g_plan.CreateThread();
        g_addAudioBusiness.savePrefix = g_audiocfg.getAudioFilePath();
        g_addAudioBusiness.load();

        cout << "begin working..." << endl;

        loop_work(dwClientType, struOTAPRegInfo);

        NET_EBASE_Fini();
    } else {
        while (true) {
			sleep(1);
		}
    }
    // 1.test stream

    return 0;
}
