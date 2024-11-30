#include "login.h"

// 加载网络配置
void load_network_config(NetworkConfig* config, bool isLan) {
    CUtils utils;
    if (isLan) {
        config->ip = strdup(utils.get_lan_ip());
        config->netmask = strdup(utils.get_lan_netmask());
        config->gateway = strdup(utils.get_lan_gateway());
    } else {
        config->ip = strdup(utils.get_wan_ip());
        config->netmask = strdup(utils.get_wan_netmask());
        config->gateway = strdup(utils.get_wan_gateway());
    }
}

// 加载设备信息
void load_device_info(DeviceInfo* info) {
    CUtils util;
    CAudioCfgBusiness cfg;
    cfg.load();
    load_network_config(&info->lan, true);
    load_network_config(&info->wan, false);
    info->codeVersion = strdup(cfg.business[0].codeVersion);
    info->coreVersion = strdup(util.get_core_version());
    info->relayMode = Relay_getInstance()->getGpioModel();
    info->storageType = 0;
    info->port = 34508;
    info->playStatus = PlayStatus_getInstance()->getPlayState();
    g_volumeSet.load();
    info->volume = g_volumeSet.getVolume();
    info->relayStatus = Relay_getInstance()->getGpioStatus();
    info->hardwareReleaseTime = strdup(util.get_hardware_release_time());
    info->spiFreeSpace = 9752500;
    info->flashFreeSpace = util.get_available_Disk("/mnt");
    info->hardwareVersion = strdup(util.get_res_by_cmd("uname -r"));
    info->hardwareModelId = 2;
    info->password = strdup(cfg.business[0].serverPassword);
    info->temperature = 12;
    info->address = strdup("01");
    info->userName = strdup("admin");
    info->imei = strdup(cfg.business[0].deviceID);
    info->functionVersion = strdup("BROADCAST");
    info->deviceCode = strdup(cfg.business[0].deviceID);
    info->serverAddress = strdup(cfg.business[0].server);
    info->serverPort = strdup(cfg.business[0].port);
}

// 加载登录数据
void load_login_data(LoginData* data) {
    CUtils util;
    CAudioCfgBusiness cfg;
    cfg.load();
    data->codeVersion = strdup(cfg.business[0].codeVersion);
    data->coreVersion = strdup(util.get_core_version());
    data->relayMode = Relay_getInstance()->getGpioModel();
    data->ip = strdup(util.get_addr());
    data->storageType = 0;
    data->port = 34508;
    data->playStatus = PlayStatus_getInstance()->getPlayState();
    g_volumeSet.load();
    data->volume = g_volumeSet.getVolume();
    data->relayStatus = Relay_getInstance()->getGpioStatus();
    data->hardwareReleaseTime = strdup(util.get_hardware_release_time());
    data->spiFreeSpace = 9752500;
    data->flashFreeSpace = util.get_available_Disk("/mnt");
    data->hardwareVersion = strdup(util.get_res_by_cmd("uname -r"));
    data->hardwareModelId = 2;
    data->password = strdup(cfg.business[0].serverPassword);
    data->temperature = 12;
    data->netmask = strdup(util.get_netmask());
    data->address = strdup("01");
    data->gateway = strdup(util.get_gateway());
    data->userName = strdup("admin");
    data->imei = strdup(cfg.business[0].deviceID);
    data->functionVersion = strdup("BROADCAST");
    data->deviceCode = strdup(cfg.business[0].deviceID);
    data->serverAddress = strdup(cfg.business[0].server);
    data->serverPort = strdup(cfg.business[0].port);
}

// 初始化登录结果
void init_login_result(LoginResult* result) {
    result->cmd = NULL;
    result->resultId = 0;
    memset(&result->data, 0, sizeof(LoginData));
    result->msg = NULL;
}

// 释放登录结果
void free_login_result(LoginResult* result) {
    free(result->cmd);
    free(result->msg);
    free(result->data.codeVersion);
    free(result->data.coreVersion);
    free(result->data.ip);
    free(result->data.hardwareReleaseTime);
    free(result->data.hardwareVersion);
    free(result->data.password);
    free(result->data.netmask);
    free(result->data.address);
    free(result->data.gateway);
    free(result->data.userName);
    free(result->data.imei);
    free(result->data.functionVersion);
    free(result->data.deviceCode);
    free(result->data.serverAddress);
    free(result->data.serverPort);
}

// 登录成功
void login_result_success(LoginResult* result) {
    result->cmd = strdup("Login");
    result->resultId = 1;
    result->msg = strdup("login success");
    load_login_data(&result->data);
}

// 登录失败
void login_result_fail(LoginResult* result) {
    result->cmd = strdup("Login");
    result->resultId = 2;
    result->msg = strdup("login fail");
    load_login_data(&result->data);
}

// 初始化登录
void init_login(Login* login) {
    login->cmd = NULL;
    login->userName = NULL;
    login->password = NULL;
}

// 释放登录
void free_login(Login* login) {
    free(login->cmd);
    free(login->userName);
    free(login->password);
}

// 处理登录请求
int process_login_request(const Login* login, CSocket* pClient) {
    CAudioCfgBusiness cfg;
    cfg.load();
    LoginResult res;
    init_login_result(&res);
    if (strcmp(login->userName, "admin") == 0 && strcmp(login->password, cfg.business[0].serverPassword) == 0) {
        login_result_success(&res);
    } else {
        login_result_fail(&res);
    }

    json_t* j = json_pack("{s:s, s:i, s:o, s:s}", "cmd", res.cmd, "resultId", res.resultId, "data", login_data_to_json(&res.data), "msg", res.msg);
    char* s = json_dumps(j, JSON_INDENT(4));
    printf("%s\n", s);
    pClient->Send(s, strlen(s));
    free(s);
    json_decref(j);
    free_login_result(&res);
    return res.resultId;
}