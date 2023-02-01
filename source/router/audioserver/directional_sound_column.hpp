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
        hardwareReleaseTime = "2022.12.09";
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
        volume = g_volumeSet.getVolume();
        storageType = "1";
    }

private:
    std::string relayStatus;
    std::string volume;
    std::string storageType;
};

namespace asns {
    const int TCPNETWORKSET = 101;
    const int UPDATEPWD = 102;
    const int NETWORKSET = 104;
    const int LOGIN = 103;
    const int SETDEVICEADDRRESS = 0;
    const int AUDIOPLAY = 1;
    const int AUDIOSTOP = 2;
    const int VOLUMSET = 3;
    const int REBOOT = 4;
    const int GETDEVICESTATUS = 5;
    const int TTSPLAY = 6;
    const int LIGHTSWITCH = 7;
    const int FILEUPLOAD = 8;
    const int RESTORE = 9;
    const int AUDIONUMORTPLAY = 10;
    const int TTSNUMORTPLAY = 11;
    const int GETDEVICEBASEINFO = 12;
    const int RECORD = 13;
    const int REMOTEUPGRADE = 14;
    const int GETAUDIOLIST = 15;
    const int LIGHTCONFIG = 16;
    const int RECORDBEGIN = 18;
    const int RECORDEND = 19;
    const int AUDIOFILEUPLOAD = 20;
    const int REMOTEFILEUPGRADE = 21;

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
            return SendFast("F6", pClient);
        }
    }

    int SettingDeviceAddress(const std::vector<std::string> &m_str, CSocket *pClient) {
    }

    // AA 04 01 01 23.mp3 BB EF
    int AudioPlay(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        if (utils.get_process_status("madplay")) {
            return SendFast("F22", pClient);
        }
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        CAddColumnCustomAudioFileBusiness busines;
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21", pClient);
        } else if (!busines.isNameEmpty(m_str[4])) {
            //音频文件查询不存在
            return SendFast("F23", pClient);
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            /*设备正在播放中，触发失败*/
            return SendFast("F22", pClient);
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
            return SendFast("F31", pClient);
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
            return SendFast("F22", pClient);
        }
        std::string txt = utils.hex_to_string(m_str[4]);
        std::string cmd = "tts -l xiaoyan -t " + txt + " -f /tmp/output.pcm";
        system(cmd.c_str());
        SendTrue(pClient);
        Singleton::getInstance().setStatus(1);
        std::thread([&] {
            while (Singleton::getInstance().getStatus()) {
                system("aplay -t raw -c 1 -f S16_LE -r 16000 /tmp/output.pcm");
            }
        }).detach();
    }

    int TtsNumTimePlay(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
            return SendFast("F22", pClient);
        }

        int playType = std::stoi(m_str[5]);
        int duration = std::stoi(m_str[6]);

        std::string txt = utils.hex_to_string(m_str[4]);
        std::string cmd = "tts -l xiaoyan -t " + txt + " -f /tmp/output.pcm";
        system(cmd.c_str());
        switch (playType) {
            case 0: {
                Singleton::getInstance().setStatus(1);
                std::thread([&] {
                    while (Singleton::getInstance().getStatus()) {
                        system("aplay -t raw -c 1 -f S16_LE -r 16000 /tmp/output.pcm");
                    }
                }).detach();
                break;
            }
            case 1: {
                if (duration < 1) {
                    return SendFast("F4", pClient);
                }
                std::string cmd = "aplay -t raw -c 1 -f S16_LE -r 16000 ";
                for (int i = 0; i < duration; ++i) {
                    cmd += "/tmp/output.pcm ";
                }
                cmd += "&";
                std::cout << "cmd: " << cmd << std::endl;
                system(cmd.c_str());
                break;
            }
            case 2: {
                if (duration < 1) {
                    return SendFast("F4", pClient);
                }
                Singleton::getInstance().setStatus(1);
                utils.async_wait(1, duration, 0, [&] {
                    Singleton::getInstance().setStatus(0);
                    system("killall -9 aplay");
                });
                std::thread([&] {
                    while (Singleton::getInstance().getStatus()) {
                        system("aplay -t raw -c 1 -f S16_LE -r 16000 /tmp/output.pcm");
                    }
                }).detach();
                break;
            }
            default:
                return SendFast("F4", pClient);
        }
        return SendTrue(pClient);
    }

    int AudioNumberOrTimePlay(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        if (utils.get_process_status("madplay")) {
            return SendFast("F22", pClient);
        }
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        CAddColumnCustomAudioFileBusiness busines;
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21", pClient);
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            return SendFast("F22", pClient);
        } else if (!busines.isNameEmpty(m_str[4])) {
            return SendFast("F23", pClient);
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
                        return SendFast("F4", pClient);
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
                        return SendFast("F4", pClient);
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
                    return SendFast("F4", pClient);
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
            return SendFast("F6", pClient);
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
            return SendFast("F6", pClient);
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
            return SendFast("F25", pClient);
        } else if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].serverPassword) {
            return SendFast("F6", pClient);
        } else {
            cfg.business[0].serverPassword = m_str[6];
            cfg.saveToJson();
            CUtils utils;
            if (utils.is_ros_platform()) {
                char buf[64] = {0};
                sprintf(buf, "cm set_val sys serverpassword %s", m_str[5].c_str());
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
        std::string imei = "11111";
        if (utils.get_process_status("madplay")) {
            return SendFast("F22", pClient);
        }
        int time = std::stod(m_str[4]);
        if (time > 30 || time < 0) {
            return SendFast("F5", pClient);
        }
        system("arecord -f cd /tmp/record.mp3 &");
        std::thread([&] {
            std::this_thread::sleep_for(std::chrono::seconds(time));
            system("killall -9 arecord");
        }).join();
        std::string res = utils.get_doupload_result(m_str[5].c_str(), imei);
        std::cout << "result:" << res << std::endl;
        system("rm /tmp/record.mp3");
        if (res.empty() || res.find("error") != std::string::npos) {
            return SendFast("F5", pClient);
        } else if (res.find("true") != std::string::npos) {
            json js;
            js["downloadUrl"] = m_str[5];
            std::string res = "01 E1 " + js.dump();
            return pClient->Send(res.c_str(), res.length());
        } else {
            return SendFast("F5", pClient);
        }
    }

    int RecordBegin(const std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
            return SendFast("F22", pClient);
        }
        system("arecord -f cd /tmp/record.mp3 &");
        return SendTrue(pClient);
    }

    /*int RecordEnd(const std::vector<std::string> &m_str, CSocket *pClient) {
        std::string audioPath = "/tmp/record.mp3";
        system("killall -9 arecord");
        std::ifstream fs(audioPath, std::fstream::in | std::fstream::binary);
        CUtils utils;
        int file_size = utils.get_file_size(audioPath);
        Server server(34509);
        int port = server.bind();
        if (port < 0) {
            SendFast("F5", pClient);
            return 0;
        }
        server.listen();
        Epoll epoll;
        epoll.insert(server.get_fd(), EPOLLIN | EPOLLET, false);
        std::string res = "01 E1 " + m_str[4] + " " + std::to_string(file_size) + " " + std::to_string(port);
        pClient->Send(res.c_str(), res.length());
        while (true) {
            int num = epoll.wait(-1);
            if (num == 0) {
                std::cout << "timeout" << std::endl;
                break;
            }
            for (int i = 0; i < num; ++i) {
                int cur_fd = epoll.get_event_fd(i);
                if (cur_fd == server.get_fd()) {
                    int connfd = server.accept();
                    epoll.set_no_blocking(connfd);
                    bool flag = epoll.insert(connfd, EPOLLET | EPOLLIN, true);
                    if (flag == false) {
                        std::cout << "Connection error" << std::endl;
                        break;
                    }
                } else if (epoll.get_event(i) & EPOLLIN) {
                    char buf[8192] = {0};
                    while (!fs.eof()) {
                        fs.read(buf, sizeof(buf));
                        std::cout << buf << std::endl;
                        server.write(cur_fd, buf, fs.gcount());
                    }
                    fs.close();
                    return SendTrue(pClient);
                } else {
                    std::cout << "..." << std::endl;
                }
            }
        }
    }*/

    int RecordEnd(const std::vector<std::string> &m_str, CSocket *pClient) {
        std::string audioPath = "/tmp/record.mp3";
        system("killall -9 arecord");

        CUtils utils;
        int file_size = utils.get_file_size(audioPath);
        if (file_size <= 0) {
            return SendFast("F5", pClient);
        }
        utils.async_wait(1, 0, 0, [&, file_size, pClient] {
            CSocket socket;
            if (!socket.Create(TCP)) {
                return SendFast("F5", pClient);
            }
            int opt = 1;
            setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(opt));
            for (int i = 34509; i < 34608; ++i) {
                if (socket.Bind(i)) {
                    std::string res = "01 E1 " + m_str[4] + " " + std::to_string(file_size) + " " + std::to_string(i);
                    pClient->Send(res.c_str(), res.length());
                    break;
                } else if (i == 34608) {
                    DS_TRACE("fatal , bind error!\n");
                    return SendFast("F5", pClient);
                }
            }
            socket.Listen();
            while (1) {
                CSocket *pTcp = new CSocket;
                fd_set rset;
                FD_ZERO(&rset);
                FD_SET(socket.m_hSocket, &rset);
                struct timeval timeout;
                timeout.tv_sec = 30;
                timeout.tv_usec = 0;
                int n = select(socket.m_hSocket + 1, &rset, NULL, NULL, &timeout);
                if (n < 0) {
                    DS_TRACE("fatal , select error!\n");
                    return SendFast("F5", pClient);
                } else if (n == 0) {
                    DS_TRACE("timeout!\n");
                    return SendFast("F5", pClient);
                } else if (n > 0) {
                    DS_TRACE("server select n = " << n);
                }
                socket.Accept(pTcp);
                DS_TRACE("Got the no." << " connection :" << pTcp->GetRemoteIp() << ":"
                                       << ntohs(pTcp->GetPeerPort()));
                char buf[8192] = {0};
                std::fstream fs(audioPath, std::fstream::in | std::fstream::binary);
                while (!fs.eof()) {
                    fs.read(buf, sizeof(buf));
                    std::cout << sizeof(buf) << " ";
                    pTcp->Send(buf, fs.gcount());
                }
                fs.close();
                socket.Close();
                system("rm /tmp/record.mp3");
                return SendTrue(pClient);
            }
        });
    }


    int FileUpload(std::vector<std::string> &m_str, CSocket *pClient) {
        std::string name = m_str[5].substr(0, m_str[5].find_first_of('.'));
        std::string suffix = m_str[5].substr(m_str[5].find_first_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21", pClient);
        }
        if (suffix.compare("mp3") != 0) {
            return SendFast("F27", pClient);
        }
        std::string temp = name + "." + suffix;
        CUtils utils;
        CAudioCfgBusiness cfg;
        cfg.load();
        std::string path = cfg.getAudioFilePath() + temp;
        std::string res = utils.get_upload_result(m_str[4].c_str(), cfg.getAudioFilePath().c_str(), temp.c_str());
        std::cout << "res:-----" << res << std::endl;
        if (res.find("error") != std::string::npos) {
            return SendFast("F5", pClient);
        } else if (res.find("Failed to connect") != std::string::npos) {
            return SendFast("F5", pClient);
        } else if (res.find("Couldn't connect to server") != std::string::npos) {
            return SendFast("F5", pClient);
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

    /*int AudioFileUpload(std::vector<std::string> &m_str, CSocket *pClient) {
        std::string name = m_str[6].substr(0, m_str[6].find_first_of('.'));
        std::string suffix = m_str[6].substr(m_str[6].find_first_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21", pClient);
        }
        if (suffix.compare("mp3") != 0) {
            return SendFast("F27", pClient);
        }
        std::string temp = name + "." + suffix;
        CUtils utils;
        CAudioCfgBusiness cfg;
        cfg.load();
        std::string path = cfg.getAudioFilePath() + temp;
        Server server(34509);
        int port = server.bind();
        if (port < 0) {
            return SendFast("F5", pClient);
        }
        server.listen();
        Epoll epoll;
        epoll.insert(server.get_fd(), EPOLLIN | EPOLLET, false);
        std::string res = "01 E1 " + m_str[5] + " " + std::to_string(port);
        pClient->Send(res.c_str(), res.length());
        utils.async_wait(1, 0, 0, [&] {
            while (true) {
                int num = epoll.wait(30 * 1000);
                if (num == 0) {
                    std::cout << "timeout..." << std::endl;
                    break;
                }
                for (int i = 0; i < num; ++i) {
                    int cur_fd = epoll.get_event_fd(i);
                    if (cur_fd == server.get_fd()) {
                        int connfd = server.accept();
                        epoll.set_no_blocking(connfd);
                        bool flag = epoll.insert(connfd, EPOLLET | EPOLLIN, true);
                        if (!flag) {
                            std::cout << "Connection error" << std::endl;
                            break;
                        }
                    } else if (epoll.get_event(i) & EPOLLIN) {
                        std::fstream fs(path, std::fstream::out | std::fstream::binary);
                        int file_size = std::atoi(m_str[4].c_str());
                        char buf[8192] = {0};
                        while (true) {
                            int len = server.read(cur_fd, buf, sizeof(buf));
                            if (len > 0) {
                                fs.write(buf, len);
                                file_size -= len;
                                if (file_size <= 0) {
                                    break;
                                }
                            }
                        }
                        fs.close();
                        CAddColumnCustomAudioFileData node;
                        node.type = 32;
                        node.setName(temp);
                        node.size = utils.get_file_size(path);
                        CAddColumnCustomAudioFileBusiness business;
                        business.load();
                        business.business.push_back(node);
                        business.saveJson();
                        SendTrue(pClient);
                        return 1;
                    } else {
                        std::cout << "..." << std::endl;
                    }
                }
            }
        });
        return 1;
    }*/
    int AudioFileUpload(std::vector<std::string> &m_str, CSocket *pClient) {
        std::string name = m_str[6].substr(0, m_str[6].find_first_of('.'));
        std::string suffix = m_str[6].substr(m_str[6].find_first_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21", pClient);
        }
        if (suffix.compare("mp3") != 0) {
            return SendFast("F27", pClient);
        }
        std::string temp = name + "." + suffix;
        CUtils utils;
        CAudioCfgBusiness cfg;
        std::string path = cfg.getAudioFilePath() + temp;
        utils.async_wait(1, 0, 0, [&, path, pClient] {
            CSocket socket;
            if (!socket.Create(TCP)) {
                return SendFast("F5", pClient);
            }
            int opt = 1;
            setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(opt));
            for (int i = 34509; i < 34608; ++i) {
                if (socket.Bind(i)) {
                    std::string res = "01 E1 " + m_str[5] + " " + std::to_string(i);
                    pClient->Send(res.c_str(), res.length());
                    break;
                } else if (i == 34608) {
                    DS_TRACE("fatal , bind error!\n");
                    return SendFast("F5", pClient);
                }
            }
            socket.Listen();
            while (1) {
                CSocket *pTcp = new CSocket;
                fd_set rset;
                FD_ZERO(&rset);
                FD_SET(socket.m_hSocket, &rset);
                struct timeval timeout;
                timeout.tv_sec = 30;
                timeout.tv_usec = 0;
                int n = select(socket.m_hSocket + 1, &rset, NULL, NULL, &timeout);
                if (n < 0) {
                    DS_TRACE("fatal , select error!\n");
                    return SendFast("F5", pClient);
                } else if (n == 0) {
                    DS_TRACE("timeout!\n");
                    return SendFast("F5", pClient);
                } else if (n > 0) {
                    DS_TRACE("server select n = " << n);
                }
                socket.Accept(pTcp);
                DS_TRACE("Got the no." << " connection :" << pTcp->GetRemoteIp() << ":"
                                       << ntohs(pTcp->GetPeerPort()));
                std::cout << "path: " << path << std::endl;
                std::fstream fs(path, std::fstream::out | std::fstream::binary | std::fstream::trunc);
                int file_size = std::atoi(m_str[4].c_str());
                char buf[8192] = {0};
                while (true) {
                    int len = pTcp->Recv(buf, sizeof(buf));
                    std::cout << "recv len :" << len << std::endl;
                    if (len > 0) {
                        fs.write(buf, len);
                        file_size -= len;
                        if (file_size <= 0) {
                            std::cout << "read end" << std::endl;
                            break;
                        }
                    } else if (len == 0) {
                        std::cout << "read end" << std::endl;
                        break;
                    } else if (len < 0) {
                        fs.close();
                        return SendFast("F5", pClient);
                    }
                }
                fs.close();
                std::cout << "begin json" << std::endl;
                CAddColumnCustomAudioFileBusiness business;
                if (!business.exist(temp)) {
                    CAddColumnCustomAudioFileData node;
                    node.type = 32;
                    node.setName(temp);
                    node.size = utils.get_file_size(path);
                    business.business.push_back(node);
                    business.saveJson();
                } else {
                    business.updateSize(temp, std::atoi(m_str[4].c_str()));
                }
                socket.Close();
                return SendTrue(pClient);
            }
        });
    }

    /*int RemoteFileUpgrade(std::vector<std::string> &m_str, CSocket *pClient) {
        Server server(34509);
        int port = server.bind();
        if (port < 0) {
            return SendFast("F5", pClient);
        }
        server.listen();
        Epoll epoll;
        epoll.insert(server.get_fd(), EPOLLIN | EPOLLET, false);
        std::string res = "01 E1 " + m_str[5] + " " + std::to_string(port);
        pClient->Send(res.c_str(), res.length());
        while (true) {
            int num = epoll.wait(30 * 1000);
            if (num == 0) {
                std::cout << "timeout" << std::endl;
                break;
            }
            for (int i = 0; i < num; ++i) {
                int cur_fd = epoll.get_event_fd(i);
                if (cur_fd == server.get_fd()) {
                    int connfd = server.accept();
                    epoll.set_no_blocking(connfd);
                    bool flag = epoll.insert(connfd, EPOLLET | EPOLLIN, true);
                    if (!flag) {
                        std::cout << "Connection error" << std::endl;
                        break;
                    }
                } else if (epoll.get_event(i) & EPOLLIN) {
                    std::fstream fs("/var/run/version/SONICCOREV100R001.bin", std::fstream::out | std::fstream::binary);
                    int file_size = std::atoi(m_str[4].c_str());
                    char buf[1024] = {0};
                    while (true) {
                        int len = server.read(cur_fd, buf, sizeof(buf));
                        if (len > 0) {
                            fs.write(buf, len);
                            file_size -= len;
                            if (file_size <= 0)break;
                        }
                    }
                    fs.close();
                    SendTrue(pClient);
                    thread([&] {
                        system("webs -U /var/run/version/SONICCOREV100R001.bin");
                        system("reboot");
                    }).detach();
                    return 0;
                } else {
                    std::cout << "..." << std::endl;
                }
            }
        }
        return 1;
    }*/

    int RemoteFileUpgrade(std::vector<std::string> &m_str, CSocket *pClient) {
        CUtils utils;
        utils.async_wait(1, 0, 0, [&, pClient] {
            CSocket socket;
            socket.Create(TCP);
            int opt = 1;
            setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(opt));
            for (int i = 34509; i < 34608; ++i) {
                if (socket.Bind(i)) {
                    std::string res = "01 E1 " + m_str[5] + " " + std::to_string(i);
                    pClient->Send(res.c_str(), res.length());
                    break;
                } else if (i == 34608) {
                    DS_TRACE("fatal , bind error!\n");
                    return SendFast("F5", pClient);
                }
            }
            socket.Listen();
            while (1) {
                CSocket *pTcp = new CSocket;

                fd_set rset;
                FD_ZERO(&rset);
                FD_SET(socket.m_hSocket, &rset);
                struct timeval timeout;
                timeout.tv_sec = 30;
                timeout.tv_usec = 0;
                int n = select(socket.m_hSocket + 1, &rset, NULL, NULL, &timeout);
                if (n < 0) {
                    DS_TRACE("fatal , select error!\n");
                    return SendFast("F5", pClient);
                } else if (n == 0) {
                    DS_TRACE("timeout!\n");
                    return SendFast("F5", pClient);
                } else if (n > 0) {
                    DS_TRACE("server select n = " << n);
                }
                socket.Accept(pTcp);
                DS_TRACE("Got the no." << " connection :" << pTcp->GetRemoteIp() << ":" << ntohs(pTcp->GetPeerPort()));
                std::fstream fs("/var/run/version/SONICCOREV100R001.bin", std::fstream::out | std::fstream::binary);
                int file_size = std::atoi(m_str[4].c_str());
                char buf[8192] = {0};
                while (true) {
                    int len = pTcp->Recv(buf, sizeof(buf));
                    if (len > 0) {
                        fs.write(buf, len);
                        std::cout << len << " ";
                        file_size -= len;
                        if (file_size <= 0)break;
                    } else if (len == 0) {
                        break;
                    } else if (len < 0) {
                        fs.close();
                        return SendFast("F5", pClient);
                    }
                }
                fs.close();
                socket.Close();
                std::cout << "begin up read size:" << utils.get_file_size("/var/run/version/SONICCOREV100R001.bin")
                          << std::endl;
                thread([&] {
                    std::string res = utils.get_by_cmd_res("webs -U /var/run/version/SONICCOREV100R001.bin");
                    std::cout << "cmd res:" << res << std::endl;
                    if (res.find("OK") != std::string::npos) {
                        SendTrue(pClient);
                        system("reboot");
                    } else {
                        system("rm /var/run/version/SONICCOREV100R001.bin");
                        return SendFast("F5", pClient);
                    }
                }).join();
            }
        });
    }
}