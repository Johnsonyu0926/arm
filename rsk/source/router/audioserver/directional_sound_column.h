#ifndef DIRECTIONAL_SOUND_COLUMN_H
#define DIRECTIONAL_SOUND_COLUMN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "volume.h"
#include "audiocfg.h"
#include "gpio.h"
#include "hkVolume.h"
#include "hkTts.h"
#include "getDeviceBaseInfo.h"
#include "getaudiolist.h"
#include "fileUpload.h"
#include "file_recv.h"
#include "login.h"
#include "micRecord.h"
#include "epoll.h"
#include "easylogging++.h"
#include "easylogging++.cc"
#include "dodownload.sh"
#include "gpio.json"
#include "rs485.h"
#include "csocket.h"

extern int g_playing_priority;
extern VolumeSet g_volumeSet;

typedef struct {
    char* codeVersion;
    char* coreVersion;
    int relayMode;
    char* ip;
    int storageType;
    int port;
    int playStatus;
    int volume;
    int relayStatus;
    char* hardwareReleaseTime;
    int spiFreeSpace;
    int flashFreeSpace;
    char* hardwareVersion;
    int hardwareModelId;
    char* password;
    int temperature;
    char* netmask;
    char* address;
    char* gateway;
    char* userName;
    char* imei;
    char* functionVersion;
    char* deviceCode;
    char* serverAddress;
    char* serverPort;
} DeviceBaseInfo;

typedef struct {
    int relayStatus;
    int volume;
    int storageType;
} DeviceBaseState;

void device_base_info_do_success(DeviceBaseInfo* info);
void device_base_state_do_success(DeviceBaseState* state);

int send_true(CSocket* pClient);
int send_fast(const char* err_code, CSocket* pClient);
int login(const char** m_str, CSocket* pClient);
int audio_play(const char** m_str, CSocket* pClient);
int audio_stop(CSocket* pClient);
int volume_set(const char** m_str, CSocket* pClient);
int reboot(CSocket* pClient);
int get_device_status(CSocket* pClient);
int restore(CSocket* pClient);
int flash_config(const char** m_str, CSocket* pClient);
int tts_play(const char** m_str, CSocket* pClient);
int tts_num_time_play(const char** m_str, CSocket* pClient);

#endif // DIRECTIONAL_SOUND_COLUMN_H