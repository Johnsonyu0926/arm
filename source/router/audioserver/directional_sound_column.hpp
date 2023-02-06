#pragma once

#include "doorsbase.h"
#include "volume.hpp"
#include "audiocfg.hpp"
#include "add_column_custom_audio_file.hpp"
#include "talk.h"
#include "login.hpp"
#include "file_recv.hpp"
#include "epoll.hpp"
#include "utils.h"
#include "Singleton.hpp"
#include "tcpTransferThread.hpp"
#include "public.hpp"
#include <thread>

extern int g_playing_priority;
extern asns::CVolumeSet g_volumeSet;

class DeviceBaseInfo {
public:
    string codeVersion;
    string coreVersion;
    int relayMode;
    string ip;
    int storageType;
    int port;
    int playStatus;
    int volume;
    int relayStatus;
    string hardwareReleaseTime;
    int spiFreeSpace;
    int flashFreeSpace;
    string hardwareVersion;
    int hardwareModelId;
    string password;
    int temperature;
    string netmask;
    string address;
    string gateway;
    string userName;
    string imei;
    string functionVersion;
    string deviceCode;
    string serverAddress;
    string serverPort;

    void do_success() {
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        codeVersion = cfg.business[0].codeVersion; //"2.1.01"; //"1.2";
        coreVersion = "LuatOS-Air_V4010_RDA8910_BT_TTS_FLOAT";
        relayMode = 2;
        CUtils util;
        ip = util.get_lan_addr();
        storageType = util.is_ros_platform() ? 0 : 1;
        port = 34508;
        playStatus = 0;
        g_volumeSet.load();
        volume = g_volumeSet.getVolume();
        relayStatus = 1;
        hardwareReleaseTime = util.get_by_cmd_res("uname -a");
        spiFreeSpace = storageType ? 9752500 : 0;
        flashFreeSpace = util.get_available_Disk("/mnt");
        hardwareVersion = "7621";
        hardwareModelId = 2;
        password = cfg.business[0].serverPassword;
        temperature = 12;
        netmask = util.get_lan_netmask();
        address = "01";
        gateway = util.get_lan_gateway();
        userName = "admin";
        imei = cfg.business[0].deviceID;
        functionVersion = "COMMON";
        deviceCode = cfg.business[0].deviceID;
        serverAddress = cfg.business[0].server;
        serverPort = to_string(cfg.business[0].port);
    }

public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DeviceBaseInfo, codeVersion, coreVersion, relayMode, ip, storageType, port,
                                   playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                   flashFreeSpace, hardwareVersion, password, temperature, netmask, address,
                                   gateway, userName, imei, functionVersion, deviceCode, serverAddress, serverPort,
                                   hardwareModelId)
};

class DeviceBaseState {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DeviceBaseState, relayStatus, volume, storageType)

    int do_success() {
        CUtils utils;
        relayStatus = std::to_string(utils.get_process_status("madplay"));
        g_volumeSet.load();
        volume = std::to_string(g_volumeSet.getVolume());
        storageType = "1";
    }

private:
    std::string relayStatus;
    std::string volume;
    std::string storageType;
};

namespace asns {

    int SendTrue(CSocket *pClient) {
        std::string res = "01 E1";
        std::cout << "return: " << res << std::endl;
        return pClient->Send(res.c_str(), res.length());
    }

    int SendFast(const std::string err_code, CSocket *pClient) {
        std::string buf = "01 " + err_code;
        std::cout << "return: " << buf << std::endl;
        pClient->Send(buf.c_str(), buf.length());
        return 0;
    }

    bool isNumber(const string &str) {
        for (const char &c: str) {
            if (std::isdigit(c) == 0)
                return false;
        }
        return true;
    }

    int Login(const std::vector<std::string> &m_str, CSocket *pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();
        std::cout << m_str[4] << " " << cfg.business[0].serverPassword << " " << m_str[5] << std::endl;
        if (m_str[4].compare("admin") == 0 && m_str[5] == cfg.business[0].serverPassword) {
            std::cout << "return login ok" << std::endl;
            return SendTrue(pClient);
        } else {
            std::cout << "return login error" << std::endl;
            return SendFast(asns::USER_PWD_ERROR, pClient);
        }
    }

    int SettingDeviceAddress(const std::vector<std::string> &m_str, CSocket *pClient) {
    }

    // AA 04 01 01 23.mp3 BB EF
    int AudioPlay(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        if (utils.get_process_status("madplay")) {
            return SendFast(asns::ALREADY_PLAYED, pClient);
        }
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        CAddColumnCustomAudioFileBusiness busines;
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR, pClient);
        } else if (!busines.isNameEmpty(m_str[4])) {
            //音频文件查询不存在
            return SendFast(asns::AUDIO_FILE_NOT_EXITS, pClient);
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            /*设备正在播放中，触发失败*/
            return SendFast(asns::ALREADY_PLAYED, pClient);
        } else {
            if (utils.get_process_status("aplay")) {
                Singleton::getInstance().setStatus(0);
                system("killall -9 aplay");
            }
            char cmd[256] = {0};
            CAudioCfgBusiness cfg;
            cfg.load();
            sprintf(cmd, "madplay %s%s -r &", cfg.getAudioFilePath().c_str(), m_str[4].c_str());
            std::cout << "cmd: " << cmd << std::endl;
            std::thread([&] { system(cmd); }).detach();

            return SendTrue(pClient);
        }
    }

    int AudioStop(CSocket *pClient) {
        system("killall -9 madplay");
        Singleton::getInstance().setStatus(0);
        system("killall -9 aplay");
        g_playing_priority = NON_PLAY_PRIORITY;
        return SendTrue(pClient);
    }

    int VolumeSet(const std::vector<std::string> &m_str, CSocket *pClient) {
        int volume = std::stoi(m_str[4]);
        if (volume > 7 || volume < 0) {
            //音量值不在范围
            return SendFast(asns::VOLUME_RANGE_ERROR, pClient);
        } else {
            CVolumeSet volumeSet;
            volumeSet.setVolume(volume);
            volumeSet.addj(volume);
            volumeSet.saveToJson();
            return SendTrue(pClient);
        }
    }

    int Reboot(CSocket *pClient) {
        SendTrue(pClient);
        system("reboot");
        return 1;
    }

    int GetDeviceStatus(CSocket *pClient) {
        DeviceBaseState deviceState;
        deviceState.do_success();
        json res = deviceState;
        std::string str = "01 E1 " + res.dump();
        pClient->Send(str.c_str(), str.length());
    }

    int Restore(CSocket *pClient) {
        CUtils utils;
        CAudioCfgBusiness cfg;
        cfg.load();
        if (utils.is_ros_platform()) {
            cfg.business[0].serverPassword = "Aa123456";
            cfg.business[0].server = "192.168.1.90";
            cfg.business[0].port = 7681;
            cfg.saveToJson();
            SendTrue(pClient);
            system("cm default");
            cout << "restore ros platform config success. rebooting system now..." << endl;
            system("reboot");
            return 1;
        } else {
            cfg.business[0].serverPassword = "Aa123456";
            cfg.business[0].server = "192.168.1.90";
            cfg.business[0].port = 7681;
            //clean(cfg.business[0].savePrefix.c_str());
            utils.clean_audio_server_file(cfg.business[0].savePrefix.c_str());
            cfg.saveToJson();
            SendTrue(pClient);
            //network restore must be the last action!
            utils.openwrt_restore_network();
            cout << "restore success!\n" << endl;
        }
    }

    int TtsPlay(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
            return SendFast(asns::ALREADY_PLAYED, pClient);
        }
        std::string txt = utils.hex_to_string(m_str[4]);
        if (utils.statistical_character_count(txt) > 100) {
            return SendFast(asns::TTS_TXT_LENGTH_ERROR, pClient);
        }
        std::string cmd = "tts -t " + txt + " -f /tmp/output.pcm";
        system(cmd.c_str());
        SendTrue(pClient);
        Singleton::getInstance().setStatus(1);
        std::thread([&] {
            while (Singleton::getInstance().getStatus()) {
                system("aplay -t raw -c 1 -f S16_LE -r 16000 /tmp/output.pcm");
            }
        }).detach();
    }

    void tts(const int playType, const int duration, const std::string &txt, CSocket *pClient) {
        CUtils utils;
        std::string cmd = "tts -t " + txt + " -f /tmp/output.pcm";
        std::cout << "cmd" << cmd << std::endl;
        system(cmd.c_str());
        switch (playType) {
            case 0: {
                Singleton::getInstance().setStatus(1);
                while (Singleton::getInstance().getStatus()) {
                    system("aplay -t raw -c 1 -f S16_LE -r 16000 /tmp/output.pcm");
                }
                break;
            }
            case 1: {
                std::string cmd = "aplay -t raw -c 1 -f S16_LE -r 16000 ";
                for (int i = 0; i < duration; ++i) {
                    cmd += "/tmp/output.pcm ";
                }
                //cmd += "&";
                std::cout << "cmd: " << cmd << std::endl;
                system(cmd.c_str());
                break;
            }
            case 2: {
                Singleton::getInstance().setStatus(1);
                utils.async_wait(1, duration, 0, [&] {
                    Singleton::getInstance().setStatus(0);
                    system("killall -9 aplay");
                });
                while (Singleton::getInstance().getStatus()) {
                    system("aplay -t raw -c 1 -f S16_LE -r 16000 /tmp/output.pcm");
                }
                break;
            }
            default:
                SendFast(asns::NONSUPPORT_ERROR, pClient);
        }
        SendTrue(pClient);
    }

    int TtsNumTimePlay(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay") ||
            utils.get_process_status("tts")) {
            return SendFast(asns::ALREADY_PLAYED, pClient);
        }
        std::string txt = utils.hex_to_string(m_str[4]);
        if (utils.statistical_character_count(txt) > 100) {
            return SendFast(asns::TTS_TXT_LENGTH_ERROR, pClient);
        }
        std::cout << "tts size:" << txt.length() << "txt: " << txt << std::endl;
        int playType = std::stoi(m_str[5]);
        int duration = std::stoi(m_str[6]);
        if (duration < 1) {
            return SendFast(asns::NONSUPPORT_ERROR, pClient);
        }
        utils.async_wait(1, 0, 0, tts, playType, duration, txt, pClient);
    }

    int AudioNumberOrTimePlay(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        if (utils.get_process_status("madplay")) {
            return SendFast(asns::ALREADY_PLAYED, pClient);
        }
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        CAddColumnCustomAudioFileBusiness busines;
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR, pClient);
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            return SendFast(asns::ALREADY_PLAYED, pClient);
        } else if (!busines.isNameEmpty(m_str[4])) {
            return SendFast(asns::AUDIO_FILE_NOT_EXITS, pClient);
        } else {
            char command[256] = {0};
            int duration = std::stoi(m_str[6]);
            CAudioCfgBusiness cfg;
            cfg.load();
            if (utils.get_process_status("aplay")) {
                Singleton::getInstance().setStatus(0);
                system("killall -9 aplay");
            }
            switch (std::stoi(m_str[5])) {
                case 0: {
                    sprintf(command, "madplay %s%s -r &", cfg.getAudioFilePath().c_str(), m_str[4].c_str());
                    std::cout << "cmd: " << command << std::endl;
                    system(command);
                    break;
                }
                case 1: {
                    if (duration < 1) {
                        return SendFast(asns::NONSUPPORT_ERROR, pClient);
                    }
                    std::string cmd = "madplay ";
                    for (int i = 0; i < duration; ++i) {
                        cmd += cfg.getAudioFilePath() + m_str[4] + ' ';
                    }
                    cmd += "&";
                    std::cout << "cmd: " << cmd << std::endl;
                    system(cmd.c_str());
                    break;
                }
                case 2: {
                    if (duration < 1) {
                        return SendFast(asns::NONSUPPORT_ERROR, pClient);
                    }
                    int d = duration / (3600 * 24);
                    int t = duration % (3600 * 24) / 3600;
                    int m = duration % (3600 * 24) % 3600 / 60;
                    int s = duration % (3600 * 24) % 3600 % 60;
                    char buf[64] = {0};
                    sprintf(buf, "%d:%d:%d:%d", d, t, m, s);
                    sprintf(command, "madplay %s%s -r -t %s &", cfg.getAudioFilePath().c_str(), m_str[4].c_str(), buf);
                    std::cout << "cmd: " << command << std::endl;
                    system(command);
                    break;
                }
                default:
                    return SendFast(asns::NONSUPPORT_ERROR, pClient);
            }
            return SendTrue(pClient);
        }
    }

    int GetDeviceBaseInfo(CSocket *pClient) {
        DeviceBaseInfo devInfo;
        devInfo.do_success();
        json res = devInfo;
        std::string str = "01 E1 " + res.dump();
        std::cout << "GetDeviceBaseInfo Res: " << str << std::endl;
        pClient->Send(str.c_str(), str.length());
    }

    int NetworkSet(const std::vector<std::string> &m_str, CSocket *pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].serverPassword) {
            return SendFast(asns::USER_PWD_ERROR, pClient);
        } else {
            CUtils utils;
            const std::string &gateway = m_str[7];
            const std::string &ipAddress = m_str[6];
            const std::string &netMask = m_str[8];
            if (utils.is_ros_platform()) {
                SendTrue(pClient);
                char cm[128] = {0};
                sprintf(cm, "cm set_val WAN1 gateway %s", gateway.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "cm set_val WAN1 ipmask %s", netMask.c_str());
                std::cout << cm << std::endl;
                system(cm);
                system("reboot");
            } else {
                char uci[128] = {0};
                sprintf(uci, "uci set network.lan.ipaddr=%s", ipAddress.c_str());
                system(uci);
                sprintf(uci, "uci set network.lan.gateway=%s", gateway.c_str());
                system(uci);
                sprintf(uci, "uci set network.lan.netmask=%s", netMask.c_str());
                system(uci);
                sprintf(uci, "uci commit network");
                system(uci);
                sprintf(uci, "/etc/init.d/network reload &");
                SendTrue(pClient);
                system(uci);
            }
        }
    }

    int TCPNetworkSet(const std::vector<std::string> &m_str, CSocket *pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].serverPassword) {
            return SendFast(asns::USER_PWD_ERROR, pClient);
        } else {
            CUtils utils;
            const std::string &gateway = m_str[7];
            const std::string &ipAddress = m_str[6];
            if (utils.is_ros_platform()) {
                SendTrue(pClient);
                char cm[128] = {0};
                sprintf(cm, "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
                system(cm);
                sprintf(cm, "cm set_val WAN1 gateway %s", gateway.c_str());
                system(cm);
                system("reboot");
            } else {
                SendTrue(pClient);
                char uci[128] = {0};
                sprintf(uci, "uci set network.lan.ipaddr=%s", ipAddress.c_str());
                system(uci);
                sprintf(uci, "uci set network.lan.gateway=%s", gateway.c_str());
                system(uci);
                sprintf(uci, "uci commit network");
                system(uci);
                sprintf(uci, "/etc/init.d/network reload");
                system(uci);
            }
            return 1;
        }
    }

    int UpdatePwd(const std::vector<std::string> &m_str, CSocket *pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[6].length() < 8) {
            return SendFast(asns::OPERATION_NEW_PWD_COMPLEXITY_ERROR, pClient);
        } else if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].serverPassword) {
            return SendFast(asns::USER_PWD_ERROR, pClient);
        } else {
            cfg.business[0].serverPassword = m_str[6];
            cfg.saveToJson();
            CUtils utils;
            if (utils.is_ros_platform()) {
                char buf[64] = {0};
                sprintf(buf, "cm set_val sys serverpassword %s", m_str[6].c_str());
                system(buf);
            }
            return SendTrue(pClient);
        }
    }

    int GetAudioList(const std::vector<std::string> &m_str, CSocket *pClient) {
        asns::CAddColumnCustomAudioFileBusiness busines;
        busines.load();
        json res = busines.business;
        std::string str = "01 E1 " + res.dump();
        std::cout << str << std::endl;
        pClient->Send(str.c_str(), str.length());
    }

    // AA 05 01 13 5 http://192.168.85.1:8091/iot/1v1/api/v1/micRecordUpload BB EF
    int Record(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        CAudioCfgBusiness bus;
        bus.load();
        std::string imei = bus.business[0].deviceID;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
            return SendFast(asns::ALREADY_PLAYED, pClient);
        }
        int time = std::stod(m_str[4]);
        if (time > 30 || time < 0) {
            return SendFast(asns::RECORD_TIME_ERROR, pClient);
        }
        system("arecord -f cd /tmp/record.mp3 &");

        std::this_thread::sleep_for(std::chrono::seconds(time));
        system("killall -9 arecord");
        std::string res = utils.get_doupload_result(m_str[5].c_str(), imei);
        std::cout << "result:" << res << std::endl;
        system("rm /tmp/record.mp3");
        if (res.find("true") != std::string::npos) {
            return SendTrue(pClient);
        } else {
            return SendFast(asns::POST_ERROR, pClient);
        }
    }

    int RecordBegin(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
            return SendFast(asns::ALREADY_PLAYED, pClient);
        }
        system("arecord -f cd /tmp/record.mp3 &");
        return SendTrue(pClient);
    }

    int RecordEnd(const std::vector<std::string> &m_str, CSocket *pClient) {
        system("killall -9 arecord");
        CUtils utils;
        int file_size = utils.get_file_size(asns::RECORD_PATH);
        if (file_size <= 0) {
            return SendFast(asns::RECORD_SIZE_ERROR, pClient);
        }
        TcpTransferThread *pServer = new TcpTransferThread();
        pServer->SetPort(asns::BEGINPORT);
        pServer->SetClient(pClient);
        pServer->SetVecStr(m_str);
        pServer->CreateThread();
    }

    int FileUpload(std::vector<std::string> &m_str, CSocket *pClient) {
        std::string name = m_str[5].substr(0, m_str[5].find_first_of('.'));
        std::string suffix = m_str[5].substr(m_str[5].find_first_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR, pClient);
        }
        if (suffix.compare("mp3") != 0) {
            return SendFast(asns::AUDIO_SUFFIX_ERROR, pClient);
        }
        std::string temp = name + "." + suffix;
        CUtils utils;
        CAudioCfgBusiness cfg;
        cfg.load();
        std::string path = cfg.getAudioFilePath() + temp;
        std::string res = utils.get_upload_result(m_str[4].c_str(), cfg.getAudioFilePath().c_str(), temp.c_str());
        std::cout << "res:-----" << res << std::endl;
        if (res.find("error") != std::string::npos) {
            return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
        } else if (res.find("Failed to connect") != std::string::npos) {
            return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
        } else if (res.find("Couldn't connect to server") != std::string::npos) {
            return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
        } else {
            CAddColumnCustomAudioFileBusiness business;
            if (!business.exist(temp)) {
                CAddColumnCustomAudioFileData node;
                node.type = 32;
                node.setName(temp);
                node.size = utils.get_file_size(path);
                business.business.push_back(node);
                business.saveJson();
            } else {
                int size = utils.get_file_size(path);
                business.updateSize(temp, size);
            }
            SendTrue(pClient);
            return 1;
        }
    }

    int AudioFileUpload(std::vector<std::string> &m_str, CSocket *pClient) {
        std::string name = m_str[6].substr(0, m_str[6].find_first_of('.'));
        std::string suffix = m_str[6].substr(m_str[6].find_first_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR, pClient);
        }
        if (suffix.compare("mp3") != 0) {
            return SendFast(asns::AUDIO_SUFFIX_ERROR, pClient);
        }

        TcpTransferThread *pServer = new TcpTransferThread();
        pServer->SetPort(asns::BEGINPORT);
        pServer->SetClient(pClient);
        pServer->SetVecStr(m_str);
        pServer->CreateThread();
        std::cout << "AudioFileUpload end....." << std::endl;
        return 1;
    }

    int RemoteFileUpgrade(std::vector<std::string> &m_str, CSocket *pClient) {
        TcpTransferThread *pServer = new TcpTransferThread();
        pServer->SetPort(asns::BEGINPORT);
        pServer->SetClient(pClient);
        pServer->SetVecStr(m_str);
        pServer->CreateThread();
    }
}