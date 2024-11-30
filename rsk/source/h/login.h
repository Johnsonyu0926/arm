#ifndef LOGIN_H
#define LOGIN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "audiocfg.h"
#include "utils.h"
#include "volume.h"
#include "Relay.h"
#include "AudioPlayStatus.h"
#include "socket.h"

typedef struct {
    char* ip;
    char* netmask;
    char* gateway;
} NetworkConfig;

typedef struct {
    NetworkConfig lan;
    NetworkConfig wan;
    char* codeVersion;
    char* coreVersion;
    int relayMode;
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
    char* address;
    char* userName;
    char* imei;
    char* functionVersion;
    char* deviceCode;
    char* serverAddress;
    char* serverPort;
} DeviceInfo;

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
} LoginData;

typedef struct {
    char* cmd;
    int resultId;
    LoginData data;
    char* msg;
} LoginResult;

typedef struct {
    char* cmd;
    char* userName;
    char* password;
} Login;

void load_network_config(NetworkConfig* config, bool isLan);
void load_device_info(DeviceInfo* info);
void load_login_data(LoginData* data);
void init_login_result(LoginResult* result);
void free_login_result(LoginResult* result);
void login_result_success(LoginResult* result);
void login_result_fail(LoginResult* result);
void init_login(Login* login);
void free_login(Login* login);
int process_login_request(const Login* login, CSocket* pClient);

#endif // LOGIN_H