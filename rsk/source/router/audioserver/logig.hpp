// login.hpp
#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <nlohmann/json.hpp>
#include "audiocfg.hpp"
#include "utils.h"
#include "volume.hpp"
#include "Relay.hpp"
#include "AudioPlayStatus.hpp"

namespace asns {

extern CVolumeSet g_volumeSet;

class NetworkConfig {
public:
    std::string ip;
    std::string netmask;
    std::string gateway;

    void loadLanConfig() {
        CUtils utils;
        ip = utils.get_lan_ip();
        netmask = utils.get_lan_netmask();
        gateway = utils.get_lan_gateway();
    }

    void loadWanConfig() {
        CUtils utils;
        ip = utils.get_wan_ip();
        netmask = utils.get_wan_netmask();
        gateway = utils.get_wan_gateway();
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(NetworkConfig, ip, netmask, gateway)
};

class DeviceInfo {
public:
    NetworkConfig lan;
    NetworkConfig wan;
    std::string codeVersion;
    std::string coreVersion;
    int relayMode;
    int storageType;
    int port;
    int playStatus;
    int volume;
    int relayStatus;
    std::string hardwareReleaseTime;
    int spiFreeSpace;
    int flashFreeSpace;
    std::string hardwareVersion;
    int hardwareModelId;
    std::string password;
    int temperature;
    std::string address;
    std::string userName;
    std::string imei;
    std::string functionVersion;
    std::string deviceCode;
    std::string serverAddress;
    std::string serverPort;

    void loadDeviceInfo() {
        CUtils util;
        CAudioCfgBusiness cfg;
        cfg.load();
        lan.loadLanConfig();
        wan.loadWanConfig();
        codeVersion = cfg.business[0].codeVersion;
        coreVersion = util.get_core_version();
        relayMode = Relay::getInstance().getGpioModel();
        storageType = 0;
        port = 34508;
        playStatus = PlayStatus::getInstance().getPlayState();
        g_volumeSet.load();
        volume = g_volumeSet.getVolume();
        relayStatus = Relay::getInstance().getGpioStatus();
        hardwareReleaseTime = util.get_hardware_release_time();
        spiFreeSpace = 9752500;
        flashFreeSpace = util.get_available_Disk("/mnt");
        hardwareVersion = util.get_res_by_cmd("uname -r");
        hardwareModelId = 2;
        password = cfg.business[0].serverPassword;
        temperature = 12;
        address = "01";
        userName = "admin";
        imei = cfg.business[0].deviceID;
        functionVersion = "BROADCAST";
        deviceCode = cfg.business[0].deviceID;
        serverAddress = cfg.business[0].server;
        serverPort = std::to_string(cfg.business[0].port);
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DeviceInfo, lan, wan, codeVersion, coreVersion, relayMode, storageType, port,
                                   playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                   flashFreeSpace, hardwareVersion, password, temperature, address,
                                   userName, imei, functionVersion, deviceCode, serverAddress, serverPort,
                                   hardwareModelId)
};

class LoginData {
public:
    std::string codeVersion;
    std::string coreVersion;
    int relayMode;
    std::string ip;
    int storageType;
    int port;
    int playStatus;
    int volume;
    int relayStatus;
    std::string hardwareReleaseTime;
    int spiFreeSpace;
    int flashFreeSpace;
    std::string hardwareVersion;
    int hardwareModelId;
    std::string password;
    int temperature;
    std::string netmask;
    std::string address;
    std::string gateway;
    std::string userName;
    std::string imei;
    std::string functionVersion;
    std::string deviceCode;
    std::string serverAddress;
    std::string serverPort;

    void loadLoginData() {
        CUtils util;
        CAudioCfgBusiness cfg;
        cfg.load();
        codeVersion = cfg.business[0].codeVersion;
        coreVersion = util.get_core_version();
        relayMode = Relay::getInstance().getGpioModel();
        ip = util.get_addr();
        storageType = 0;
        port = 34508;
        playStatus = PlayStatus::getInstance().getPlayState();
        g_volumeSet.load();
        volume = g_volumeSet.getVolume();
        relayStatus = Relay::getInstance().getGpioStatus();
        hardwareReleaseTime = util.get_hardware_release_time();
        spiFreeSpace = 9752500;
        flashFreeSpace = util.get_available_Disk("/mnt");
        hardwareVersion = util.get_res_by_cmd("uname -r");
        hardwareModelId = 2;
        password = cfg.business[0].serverPassword;
        temperature = 12;
        netmask = util.get_netmask();
        address = "01";
        gateway = util.get_gateway();
        userName = "admin";
        imei = cfg.business[0].deviceID;
        functionVersion = "BROADCAST";
        deviceCode = cfg.business[0].deviceID;
        serverAddress = cfg.business[0].server;
        serverPort = std::to_string(cfg.business[0].port);
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(LoginData, codeVersion, coreVersion, relayMode, ip, storageType, port,
                                   playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                   flashFreeSpace, hardwareVersion, password, temperature, netmask, address,
                                   gateway, userName, imei, functionVersion, deviceCode, serverAddress, serverPort,
                                   hardwareModelId)
};

class LoginResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(LoginResult, cmd, resultId, data, msg)

    void success() {
        cmd = "Login";
        resultId = 1;
        msg = "login success";
        data.loadLoginData();
    }

    void fail() {
        cmd = "Login";
        resultId = 2;
        msg = "login fail";
        data.loadLoginData();
    }

private:
    std::string cmd;
    int resultId;
    LoginData data;
    std::string msg;
};

class Login {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Login, cmd, userName, password)

    [[nodiscard]] int processRequest(CSocket* pClient) const {
        CAudioCfgBusiness cfg;
        cfg.load();
        LoginResult res;
        if (userName == "admin" && password == cfg.business[0].serverPassword) {
            res.success();
        } else {
            res.fail();
        }

        nlohmann::json j = res;
        std::string s = j.dump();
        std::cout << s << std::endl;
        pClient->Send(s.c_str(), s.length());
        return res.resultId;
    }

private:
    std::string cmd;
    std::string userName;
    std::string password;
};

} // namespace asns

//BY GST ARMV8 GCC 13.2