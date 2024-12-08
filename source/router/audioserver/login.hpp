#ifndef __CLOGIN_H__
#define __CLOGIN_H__

#include <iostream>
#include <string>
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "volume.hpp"
#include "Relay.hpp"
#include "AudioPlayStatus.hpp"
#include "spdlog/spdlog.h"

using json = nlohmann::json;

extern asns::CVolumeSet g_volumeSet;

namespace asns {

    class Lan {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Lan, ip, netmask, gateway)

        void do_success() {
            CUtils utils;
            ip = utils.get_wan_ip();
            netmask = utils.get_wan_netmask();
            gateway = utils.get_wan_gateway();
        }

    private:
        std::string ip;
        std::string netmask;
        std::string gateway;
    };

    class CUdpMsg {
    public:
        Lan lan;
        std::string codeVersion;
        std::string coreVersion;
        int relayMode{0};
        int storageType{0};
        int port{0};
        int playStatus{0};
        int volume{0};
        int relayStatus{0};
        std::string hardwareReleaseTime;
        int spiFreeSpace{0};
        int flashFreeSpace{0};
        std::string hardwareVersion;
        int hardwareModelId{0};
        std::string password;
        int temperature{0};
        std::string address;
        std::string userName;
        std::string imei;
        std::string functionVersion;
        std::string deviceCode;
        std::string serverAddress;
        std::string serverPort;

        void do_success() {
            CUtils util;
            CAudioCfgBusiness cfg;
            cfg.load();
            lan.do_success();
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

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUdpMsg, lan, codeVersion, coreVersion, relayMode, storageType, port,
                                       playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                       flashFreeSpace, hardwareVersion, password, temperature, address,
                                       userName, imei, functionVersion, deviceCode, serverAddress, serverPort,
                                       hardwareModelId)
    };

    class CLoginData {
    public:
        std::string codeVersion;
        std::string coreVersion;
        int relayMode{0};
        std::string ip;
        int storageType{0};
        int port{0};
        int playStatus{0};
        int volume{0};
        int relayStatus{0};
        std::string hardwareReleaseTime;
        int spiFreeSpace{0};
        int flashFreeSpace{0};
        std::string hardwareVersion;
        int hardwareModelId{0};
        std::string password;
        int temperature{0};
        std::string netmask;
        std::string address;
        std::string gateway;
        std::string userName;
        std::string imei;
        std::string functionVersion;
        std::string deviceCode;
        std::string serverAddress;
        std::string serverPort;

        void do_success() {
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

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CLoginData, codeVersion, coreVersion, relayMode, ip, storageType, port,
                                       playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                       flashFreeSpace, hardwareVersion, password, temperature, netmask, address,
                                       gateway, userName, imei, functionVersion, deviceCode, serverAddress, serverPort,
                                       hardwareModelId)
    };

    class CLoginResult {
    private:
        std::string cmd;
        int resultId{0};
        CLoginData data;
        std::string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CLoginResult, cmd, resultId, data, msg)

        void do_success() {
            cmd = "Login";
            resultId = 1;
            msg = "login success";
            data.do_success();
        }

        void do_fail() {
            cmd = "Login";
            resultId = 2;
            msg = "login fail";
            data.do_success();
        }
    };

    class CLogin {
    private:
        std::string cmd;
        std::string userName;
        std::string password;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CLogin, cmd, userName, password)

        int do_req(CSocket *pClient) {
            CAudioCfgBusiness cfg;
            cfg.load();
            CLoginResult res;
            if (userName == "admin" && password == cfg.business[0].serverPassword) {
                res.do_success();
            } else {
                res.do_fail();
            }
            json j = res;
            std::string s = j.dump();
            spdlog::info("Login response: {}", s);
            pClient->Send(s.c_str(), s.length());
            return res.resultId == 1 ? 1 : 0;
        }
    };
} // namespace asns

#endif // __CLOGIN_H__