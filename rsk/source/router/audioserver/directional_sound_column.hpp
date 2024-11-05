// directional_sound_column.hpp
#pragma once

#include "doorsbase.h"
#include "volume.hpp"
#include "audiocfg.hpp"
#include "json.hpp"
#include "gpio.hpp"
#include "hkVolume.hpp"
#include "hkTts.hpp"
#include "getDeviceBaseInfo.hpp"
#include "getaudiolist.hpp"
#include "fileUpload.hpp"
#include "file_recv.hpp"
#include "login.hpp"
#include "micRecord.hpp"
#include "epoll.hpp"
#include "easylogging++.h"
#include "easylogging++.cc"
#include "dodownload.sh"
#include "gpio.json"

extern std::atomic<int> g_playing_priority;
extern asns::CVolumeSet g_volumeSet;

class DeviceBaseInfo {
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
        relayStatus = CUtils::get_process_status("madplay") || PlayStatus::getInstance().getPlayState();
        g_volumeSet.load();
        volume = g_volumeSet.getVolume();
        storageType = 1;
    }

private:
    int relayStatus;
    int volume;
    int storageType;
};

namespace asns {
    class CBusiness {
    public:
        [[nodiscard]] static int SendTrue(std::shared_ptr<CSocket> pClient = nullptr) {
            std::string res = "01 E1";
            LOG(INFO) << "return: " << res;
            if (!pClient) {
                return Rs485::_uart_write(res.c_str(), res.length());
            }
            return pClient->Send(res.c_str(), res.length());
        }

        [[nodiscard]] static int SendFast(std::string_view err_code, std::shared_ptr<CSocket> pClient = nullptr) {
            std::string buf = "01 " + std::string(err_code);
            LOG(INFO) << "return: " << buf;
            if (!pClient) {
                return Rs485::_uart_write(buf.c_str(), buf.length());
            }
            return pClient->Send(buf.c_str(), buf.length());
        }

        [[nodiscard]] int Login(const std::vector<std::string>& m_str, std::shared_ptr<CSocket> pClient = nullptr) {
            CAudioCfgBusiness cfg;
            cfg.load();
            LOG(INFO) << m_str[4] << " " << cfg.business[0].serverPassword << " " << m_str[5];
            if (m_str[4] == "admin" && m_str[5] == cfg.business[0].serverPassword) {
                LOG(INFO) << "return login ok";
                return SendTrue(pClient);
            } else {
                LOG(INFO) << "return login error";
                return SendFast(asns::USER_PWD_ERROR, pClient);
            }
        }

        [[nodiscard]] int AudioPlay(const std::vector<std::string>& m_str, std::shared_ptr<CSocket> pClient = nullptr) {
            if (CUtils::get_process_status("madplay")) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            }
            std::string_view name = m_str[4].substr(0, m_str[4].find_first_of('.'));
            CAddColumnCustomAudioFileBusiness business;
            if (!CUtils::str_is_all_num(name) || std::stoi(std::string(name)) > 100 || std::stoi(std::string(name)) < 0) {
                return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR, pClient);
            } else if (!business.isNameEmpty(m_str[4])) {
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

        [[nodiscard]] int AudioStop(std::shared_ptr<CSocket> pClient = nullptr) {
            AudioPlayUtil::audio_stop();
            return SendTrue(pClient);
        }

        [[nodiscard]] int VolumeSet(const std::vector<std::string>& m_str, std::shared_ptr<CSocket> pClient = nullptr) {
            int volume = std::stoi(m_str[4]);
            if (volume > 7 || volume < 0) {
                return SendFast(asns::VOLUME_RANGE_ERROR, pClient);
            } else {
                CVolumeSet volumeSet;
                volumeSet.setVolume(volume);
                volumeSet.adjust(volume);
                volumeSet.saveToJson();
                return SendTrue(pClient);
            }
        }

        [[nodiscard]] int Reboot(std::shared_ptr<CSocket> pClient = nullptr) {
            SendTrue(pClient);
            CUtils::reboot();
            return 1;
        }

        [[nodiscard]] int GetDeviceStatus(std::shared_ptr<CSocket> pClient = nullptr) {
            DeviceBaseState deviceState;
            deviceState.do_success();
            nlohmann::json res = deviceState;
            std::string str = "01 E1 " + res.dump();
            if (!pClient) {
                return Rs485::_uart_work(str.c_str(), str.length());
            }
            return pClient->Send(str.c_str(), str.length());
        }

        [[nodiscard]] int Restore(std::shared_ptr<CSocket> pClient = nullptr) {
            CAudioCfgBusiness cfg;
            cfg.load();
            cfg.business[0].serverPassword = "Aa123456";
            cfg.business[0].server = "192.168.1.90";
            cfg.business[0].port = 7681;
            cfg.saveToJson();
            SendTrue(pClient);
            CUtils::restore(cfg.business[0].savePrefix);
            return 1;
        }

        [[nodiscard]] int FlashConfig(const std::vector<std::string>& m_str, std::shared_ptr<CSocket> pClient = nullptr) {
            Relay::getInstance().set_gpio_model(std::stoi(m_str[4]), std::stoi(m_str[5]));
            return SendTrue(pClient);
        }

        [[nodiscard]] int TtsPlay(const std::vector<std::string>& m_str, std::shared_ptr<CSocket> pClient = nullptr) {
            if (CUtils::get_process_status("madplay") || CUtils::get_process_status("ffplay") || PlayStatus::getInstance().getPlayState()) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            }
            std::string txt = CUtils::hex_to_string(m_str[4]);
            if (CUtils::statistical_character_count(txt) > 100) {
                return SendFast(asns::TTS_TXT_LENGTH_ERROR, pClient);
            }
            SendTrue(pClient);
            AudioPlayUtil::tts_loop_play(txt, asns::ASYNC_START);
            return 1;
        }

        [[nodiscard]] int TtsNumTimePlay(const std::vector<std::string>& m_str, std::shared_ptr<CSocket> pClient = nullptr) {
            if (CUtils::get_process_status("madplay") || CUtils::get_process_status("ffplay") ||
                CUtils::get_process_status("tts") || PlayStatus::getInstance().getPlayState()) {
                return SendFast(asns::ALREADY_PLAYED, pClient);
            }
            std::string txt = CUtils::hex_to_string(m_str[4]);
            if (CUtils::statistical_character_count(txt) > 100) {
                return SendFast(asns::TTS_TXT_LENGTH_ERROR, pClient);
            }
            LOG(INFO) << "tts size:" << txt.length() << "txt: " << txt;
            int playType = std::stoi(m_str[5]);
            int duration = std::stoi(m_str[6]);
            CUtils::async_wait(1, 0, 0, [=] {
                switch (playType) {
                    case 0: {
                        SendTrue(pClient);
                        AudioPlayUtil::tts_loop_play(txt);
                        return 0;
                    }
                    case 1: {
                        if (duration < 1) {
                            return SendFast(asns::NONSUPPORT_ERROR, pClient);
                        }
                        SendTrue(pClient);
                        AudioPlayUtil::tts_num_play(duration, txt);
                        break;
                    }
                    case 2: {
                        if (duration < 1) {
                            return SendFast(asns::NONSUPPORT_ERROR, pClient);
                        }
                        SendTrue(pClient);
                        AudioPlayUtil::tts_time_play(duration, txt);
                        break;
                    }
                    default:
                        return SendFast(asns::NONSUPPORT_ERROR, pClient);
                }
                return 1;
            });
            return 1;
        }
    };
}

//By GST ARMV8 GCC13.2 directional_sound_column.hpp