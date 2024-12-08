#pragma once

#include "doorsbase.h"
#include "volume.hpp"
#include "audiocfg.hpp"
#include "add_column_custom_audio_file.hpp"
#include "talk.h"
#include "login.hpp"
#include "utils.h"
#include "tcpTransferThread.hpp"
#include "public.hpp"
#include "Relay.hpp"
#include "Rs485.hpp"
#include "spdlog/spdlog.h"

extern int g_playing_priority;
extern asns::CVolumeSet g_volumeSet;

class DeviceBaseInfo {
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
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        CUtils util;
        codeVersion = cfg.business[0].codeVersion;
        coreVersion = util.get_core_version();
        relayMode = Relay::getInstance().getGpioModel();
        ip = util.get_addr();
        storageType = util.is_ros_platform() ? 0 : 1;
        port = 34508;
        playStatus = PlayStatus::getInstance().getPlayState();
        g_volumeSet.load();
        volume = g_volumeSet.getVolume();
        relayStatus = Relay::getInstance().getGpioStatus();
        hardwareReleaseTime = util.get_hardware_release_time();
        spiFreeSpace = storageType ? 9752500 : 0;
        flashFreeSpace = util.get_available_Disk("/mnt");
        hardwareVersion = "7621";
        hardwareModelId = 2;
        password = cfg.business[0].serverPassword;
        temperature = 12;
        netmask = util.get_netmask();
        address = "01";
        gateway = util.get_gateway();
        userName = "admin";
        imei = cfg.business[0].deviceID;
        functionVersion = "COMMON";
        deviceCode = cfg.business[0].deviceID;
        serverAddress = cfg.business[0].server;
        serverPort = std::to_string(cfg.business[0].port);
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

    void do_success() {
        CUtils utils;
        relayStatus = CUtils::get_process_status("madplay") || PlayStatus::getInstance().getPlayState();
        g_volumeSet.load();
        volume = g_volumeSet.getVolume();
        storageType = 1;
    }

private:
    int relayStatus{0};
    int volume{0};
    int storageType{0};
};

namespace asns {
    class CBusiness {
    public:
        static int SendTrue(CSocket *pClient = nullptr) {
            std::string res = "01 E1";
            spdlog::info("return: {}", res);
            if (pClient == nullptr) {
                return Rs485::_uart_write(res.c_str(), res.length());
            }
            return pClient->Send(res.c_str(), res.length());
        }

        static int SendFast(const std::string &err_code, CSocket *pClient = nullptr) {
            std::string buf = "01 " + err_code;
            spdlog::info("return: {}", buf);
            if (pClient == nullptr) {
                return Rs485::_uart_write(buf.c_str(), buf.length());
            }
            return pClient->Send(buf.c_str(), buf.length());
        }

        int Login(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CAudioCfgBusiness cfg;
            cfg.load();
            spdlog::info("{} {} {}", m_str[4], cfg.business[0].serverPassword, m_str[5]);
            if (m_str[4] == "admin" && m_str[5] == cfg.business[0].serverPassword) {
                spdlog::info("return login ok");
                return SendTrue(pClient);
            } else {
                spdlog::info("return login error");
                return SendFast(asns::USER_PWD_ERROR, pClient);
            }
        }

        int AudioPlay(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CUtils utils;
            if (CUtils::get_process_status("madplay")) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            }
            std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
            CAddColumnCustomAudioFileBusiness busines;
            if (!utils.str_is_all_num(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
                return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR, pClient);
            } else if (!busines.isNameEmpty(m_str[4])) {
                return SendFast(asns::AUDIO_FILE_NOT_EXITS, pClient);
            } else if (g_playing_priority != NON_PLAY_PRIORITY) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            } else {
                if (CUtils::get_process_status("ffplay")) {
                    AudioPlayUtil::audio_stop();
                }
                CAudioCfgBusiness cfg;
                cfg.load();
                AudioPlayUtil::audio_loop_play(cfg.getAudioFilePath() + m_str[4], asns::ASYNC_START);
                return SendTrue(pClient);
            }
        }

        int AudioStop(CSocket *pClient = nullptr) {
            AudioPlayUtil::audio_stop();
            return SendTrue(pClient);
        }

        int VolumeSet(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            int volume = std::stoi(m_str[4]);
            if (volume > 7 || volume < 0) {
                return SendFast(asns::VOLUME_RANGE_ERROR, pClient);
            } else {
                CVolumeSet volumeSet;
                volumeSet.setVolume(volume);
                volumeSet.addj(volume);
                volumeSet.saveToJson();
                return SendTrue(pClient);
            }
        }

        int Reboot(CSocket *pClient = nullptr) {
            SendTrue(pClient);
            CUtils::reboot();
            return 1;
        }

        int GetDeviceStatus(CSocket *pClient = nullptr) {
            DeviceBaseState deviceState;
            deviceState.do_success();
            json res = deviceState;
            std::string str = "01 E1 " + res.dump();
            if (pClient == nullptr) {
                return Rs485::_uart_work(str.c_str(), str.length());
            }
            return pClient->Send(str.c_str(), str.length());
        }

        int Restore(CSocket *pClient = nullptr) {
            CUtils utils;
            CAudioCfgBusiness cfg;
            cfg.load();
            cfg.business[0].serverPassword = "Aa123456";
            cfg.business[0].server = "192.168.1.90";
            cfg.business[0].port = 7681;
            cfg.saveToJson();
            SendTrue(pClient);
            utils.restore(cfg.business[0].savePrefix);
            return 1;
        }

        int FlashConfig(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            Relay::getInstance().set_gpio_model(std::stoi(m_str[4]), std::stoi(m_str[5]));
            return SendTrue(pClient);
        }

        int TtsPlay(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CUtils utils;
            if (CUtils::get_process_status("madplay") || CUtils::get_process_status("ffplay") || PlayStatus::getInstance().getPlayState()) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            }
            std::string txt = utils.hex_to_string(m_str[4]);
            if (utils.statistical_character_count(txt) > 100) {
                return SendFast(asns::TTS_TXT_LENGTH_ERROR, pClient);
            }
            SendTrue(pClient);
            AudioPlayUtil::tts_loop_play(txt, asns::ASYNC_START);
            return 1;
        }

        int TtsNumTimePlay(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CUtils utils;
            if (CUtils::get_process_status("madplay") || CUtils::get_process_status("ffplay") ||
                CUtils::get_process_status("tts") || PlayStatus::getInstance().getPlayState()) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            }
            std::string txt = utils.hex_to_string(m_str[4]);
            if (utils.statistical_character_count(txt) > 100) {
                return SendFast(asns::TTS_TXT_LENGTH_ERROR, pClient);
            }
            spdlog::info("tts size: {} txt: {}", txt.length(), txt);
            int playType = std::stoi(m_str[5]);
            int duration = std::stoi(m_str[6]);
            CUtils::async_wait(1, 0, 0, [=] {
                switch (playType) {
                    case 0:
                        SendTrue(pClient);
                        AudioPlayUtil::tts_loop_play(txt);
                        return 0;
                    case 1:
                        if (duration < 1) {
                            return SendFast(asns::NONSUPPORT_ERROR, pClient);
                        }
                        SendTrue(pClient);
                        AudioPlayUtil::tts_num_play(duration, txt);
                        break;
                    case 2:
                        if (duration < 1) {
                            return SendFast(asns::NONSUPPORT_ERROR, pClient);
                        }
                        SendTrue(pClient);
                        AudioPlayUtil::tts_time_play(duration, txt);
                        break;
                    default:
                        return SendFast(asns::NONSUPPORT_ERROR, pClient);
                }
                return 1;
            });
            return 1;
        }

        int AudioNumberOrTimePlay(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CUtils utils;
            if (CUtils::get_process_status("madplay")) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            }
            std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
            CAddColumnCustomAudioFileBusiness busines;
            if (!utils.str_is_all_num(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
                return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR, pClient);
            } else if (g_playing_priority != NON_PLAY_PRIORITY) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            } else if (!busines.isNameEmpty(m_str[4])) {
                return SendFast(asns::AUDIO_FILE_NOT_EXITS, pClient);
            } else {
                int duration = std::stoi(m_str[6]);
                CAudioCfgBusiness cfg;
                cfg.load();
                if (CUtils::get_process_status("ffplay")) {
                    AudioPlayUtil::audio_stop();
                }
                switch (std::stoi(m_str[5])) {
                    case 0:
                        AudioPlayUtil::audio_loop_play(cfg.getAudioFilePath() + m_str[4], asns::ASYNC_START);
                        break;
                    case 1:
                        if (duration < 1) {
                            return SendFast(asns::NONSUPPORT_ERROR, pClient);
                        }
                        AudioPlayUtil::audio_num_play(duration, cfg.getAudioFilePath() + m_str[4], asns::ASYNC_START);
                        break;
                    case 2:
                        if (duration < 1) {
                            return SendFast(asns::NONSUPPORT_ERROR, pClient);
                        }
                        AudioPlayUtil::audio_time_play(duration, cfg.getAudioFilePath() + m_str[4], asns::ASYNC_START);
                        break;
                    default:
                        return SendFast(asns::NONSUPPORT_ERROR, pClient);
                }
                return SendTrue(pClient);
            }
        }

        int GetDeviceBaseInfo(CSocket *pClient = nullptr) {
            DeviceBaseInfo devInfo;
            devInfo.do_success();
            json res = devInfo;
            std::string str = "01 E1 " + res.dump();
            spdlog::info("GetDeviceBaseInfo Res: {}", str);
            if (pClient == nullptr) {
                return Rs485::_uart_work(str.c_str(), str.length());
            }
            return pClient->Send(str.c_str(), str.length());
        }

        int NetworkSet(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CAudioCfgBusiness cfg;
            cfg.load();
            if (m_str[4] != "admin" || m_str[5] != cfg.business[0].serverPassword) {
                return SendFast(asns::USER_PWD_ERROR, pClient);
            } else {
                CUtils utils;
                SendTrue(pClient);
                utils.network_set(m_str[7], m_str[6], m_str[8]);
            }
            return 1;
        }

        int TCPNetworkSet(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CAudioCfgBusiness cfg;
            cfg.load();
            if (m_str[4] != "admin" || m_str[5] != cfg.business[0].serverPassword) {
                return SendFast(asns::USER_PWD_ERROR, pClient);
            } else {
                CUtils utils;
                SendTrue(pClient);
                utils.network_set(m_str[7], m_str[6]);
                return 1;
            }
        }

        int UpdatePwd(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CAudioCfgBusiness cfg;
            cfg.load();
            if (m_str[6].length() < 8) {
                return SendFast(asns::OPERATION_NEW_PWD_COMPLEXITY_ERROR, pClient);
            } else if (m_str[4] != "admin" || m_str[5] != cfg.business[0].serverPassword) {
                return SendFast(asns::USER_PWD_ERROR, pClient);
            } else {
                cfg.business[0].serverPassword = m_str[6];
                cfg.saveToJson();
                CUtils utils;
                utils.change_password(m_str[6].c_str());
                return SendTrue(pClient);
            }
        }

        int GetAudioList(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            asns::CAddColumnCustomAudioFileBusiness busines;
            busines.load();
            json res = busines.business;
            std::string str = "01 E1 " + res.dump();
            if (pClient == nullptr) {
                return Rs485::_uart_work(str.c_str(), str.length());
            }
            return pClient->Send(str.c_str(), str.length());
        }

        int Record(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CAudioCfgBusiness bus;
            bus.load();
            std::string imei = bus.business[0].deviceID;
            if (CUtils::get_process_status("ffmpeg")) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            }
            int time = std::stod(m_str[4]);
            if (time > 30 || time < 0) {
                return SendFast(asns::RECORD_TIME_ERROR, pClient);
            }
            std::string url = m_str[5];
            CUtils::async_wait(1, 0, 0, [=] {
                CUtils utils;
                std::string res = utils.record_upload(time, url, imei);
                if (res.find("true") != std::string::npos) {
                    SendTrue(pClient);
                } else {
                    SendFast(asns::POST_ERROR, pClient);
                }
            });
            return 1;
        }

        int RecordBegin(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CUtils utils;
            if (CUtils::get_process_status("ffmpeg")) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            }
            utils.record_start(asns::ASYNC_START);
            return SendTrue(pClient);
        }

        int RecordEnd(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CUtils utils;
            utils.record_stop();
            int file_size = utils.get_file_size(asns::RECORD_PATH);
            if (file_size <= 0) {
                return SendFast(asns::RECORD_SIZE_ERROR, pClient);
            }
            auto pServer = std::make_unique<TcpTransferThread>();
            pServer->SetPort(asns::BEGINPORT);
            pServer->SetClient(pClient);
            pServer->SetVecStr(m_str);
            pServer->CreateThread();
            return 1;
        }
        int FileUpload(std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CUtils utils;
            if (utils.get_available_Disk("/mnt") < asns::DISK_SIZE) {
                return SendFast(asns::LACK_SPACE, pClient);
            }
            std::string name = m_str[5].substr(0, m_str[5].find_first_of('.'));
            std::string suffix = m_str[5].substr(m_str[5].find_first_of('.') + 1);
            std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
            if (!utils.str_is_all_num(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
                return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR, pClient);
            }
            if (suffix != "mp3") {
                return SendFast(asns::AUDIO_SUFFIX_ERROR, pClient);
            }
            std::string temp = name + "." + suffix;
            CAudioCfgBusiness cfg;
            cfg.load();
            std::string path = cfg.getAudioFilePath() + temp;
            std::string res = utils.get_upload_result(m_str[4].c_str(), cfg.getAudioFilePath().c_str(), temp.c_str());
            if (res != "success") {
                return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
            } else {
                utils.bit_rate_32_to_48(path);
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

        int AudioFileUpload(std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            CUtils utils;
            int diskSize = utils.get_available_Disk("/mnt");
            if (diskSize < asns::DISK_SIZE) {
                return SendFast(asns::LACK_SPACE, pClient);
            }
            int audioSize = std::stoi(m_str[4]);
            if (audioSize > asns::DISK_SIZE * 1024 || (diskSize - (audioSize / 1024)) < asns::DISK_SIZE) {
                return SendFast(asns::AUDIO_SIZE_LARGE, pClient);
            }
            std::string name = m_str[6].substr(0, m_str[6].find_first_of('.'));
            std::string suffix = m_str[6].substr(m_str[6].find_first_of('.') + 1);
            std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
            if (!utils.str_is_all_num(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
                return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR, pClient);
            }
            if (suffix != "mp3") {
                return SendFast(asns::AUDIO_SUFFIX_ERROR, pClient);
            }

            auto pServer = std::make_unique<TcpTransferThread>();
            pServer->SetPort(asns::BEGINPORT);
            pServer->SetClient(pClient);
            pServer->SetVecStr(m_str);
            pServer->CreateThread();
            return 1;
        }

        int RemoteFileUpgrade(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            auto pServer = std::make_unique<TcpTransferThread>();
            pServer->SetPort(asns::BEGINPORT);
            pServer->SetClient(pClient);
            pServer->SetVecStr(m_str);
            pServer->CreateThread();
            return 1;
        }

        int DspManagement(const std::vector<std::string> &m_str, CSocket *pClient = nullptr) {
            const std::string name = m_str[4];
            const std::string sw = m_str[5];
            if (name != "lineen" || (sw != "0" && sw != "1")) {
                return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
            }
            char buf[64] = {0};
            sprintf(buf, "dspset /dev/ttyS2 %s %s", name.c_str(), sw.c_str());
            CUtils::cmd_system(buf);
            sprintf(buf, "cm set_val sys %s %s", name.c_str(), sw.c_str());
            CUtils::cmd_system(buf);
            return SendTrue(pClient);
        }
    };
} // namespace asns