#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "json.hpp"
#include "utils.h"
#include "Rs485.hpp"
#include "AcquisitionNoise.hpp"
#include "directional_sound_column.hpp"
#include "spdlog/spdlog.h"

class RSBusinessManage {
public:
    int do_str_req() {
        asns::CBusiness bus;
        int opcode = std::stoi(m_str[3]);
        spdlog::info("opcode: {}", opcode);
        switch (opcode) {
            case asns::TCPNETWORKSET:
                spdlog::info("TCPNetworkSet");
                bus.TCPNetworkSet(m_str);
                break;
            case asns::UPDATEPWD:
                spdlog::info("UpdatePwd");
                bus.UpdatePwd(m_str);
                break;
            case asns::NETWORKSET:
                spdlog::info("NetworkSet");
                bus.NetworkSet(m_str);
                break;
            case asns::LOGIN:
                spdlog::info("LOGIN");
                bus.Login(m_str);
                break;
            case asns::SETDEVICEADDRRESS:
                spdlog::info("设置设备地址");
                break;
            case asns::AUDIOPLAY:
                spdlog::info("AudioPlay");
                bus.AudioPlay(m_str);
                break;
            case asns::AUDIOSTOP:
                spdlog::info("AudioStop");
                bus.AudioStop();
                break;
            case asns::VOLUMSET:
                spdlog::info("VolumeSet");
                bus.VolumeSet(m_str);
                break;
            case asns::REBOOT:
                spdlog::info("Reboot");
                bus.Reboot();
                break;
            case asns::GETDEVICESTATUS:
                spdlog::info("GetDeviceStatus");
                bus.GetDeviceStatus();
                break;
            case asns::TTSPLAY:
                spdlog::info("TtsPlay");
                bus.TtsPlay(m_str);
                break;
            case asns::LIGHTSWITCH:
                spdlog::info("闪灯开关");
                break;
            case asns::FILEUPLOAD:
                spdlog::info("fileUpload");
                bus.FileUpload(m_str);
                break;
            case asns::RESTORE:
                spdlog::info("Restore");
                bus.Restore();
                break;
            case asns::AUDIONUMORTPLAY:
                spdlog::info("AudioNumberOrTimePlay");
                bus.AudioNumberOrTimePlay(m_str);
                break;
            case asns::TTSNUMORTPLAY:
                spdlog::info("Tts Number or Time Play");
                bus.TtsNumTimePlay(m_str);
                break;
            case asns::GETDEVICEBASEINFO:
                spdlog::info("GetDeviceBaseInfo");
                bus.GetDeviceBaseInfo();
                break;
            case asns::RECORD:
                spdlog::info("Record");
                bus.Record(m_str);
                break;
            case asns::REMOTEUPGRADE:
                spdlog::info("远程升级");
                break;
            case asns::GETAUDIOLIST:
                spdlog::info("GetAudioList");
                bus.GetAudioList(m_str);
                break;
            case asns::LIGHTCONFIG:
                spdlog::info("FlashConfig");
                bus.FlashConfig(m_str);
                break;
            case asns::RECORDBEGIN:
                spdlog::info("RecordBegin");
                bus.RecordBegin(m_str);
                break;
            case asns::RECORDEND:
                spdlog::info("RecordEnd");
                bus.RecordEnd(m_str);
                break;
            case asns::AUDIOFILEUPLOAD:
                spdlog::info("AudioFileUpload");
                bus.AudioFileUpload(m_str);
                break;
            case asns::REMOTEFILEUPGRADE:
                spdlog::info("RemoteFileUpgrade");
                bus.RemoteFileUpgrade(m_str);
                break;
            case asns::DSPMANAGE:
                spdlog::info("DSP Management");
                bus.DspManagement(m_str);
                break;
            default:
                spdlog::info("switch F4");
                asns::CBusiness::SendFast(asns::NONSUPPORT_ERROR);
                break;
        }
        return 1;
    }

    int handle_receive(const char *buf) {
        CUtils utils;
        m_str = utils.string_split(buf);
        if (m_str[0] != "AA" || m_str.back() != "EF") {
            asns::CBusiness::SendFast(asns::BEGIN_END_CODE_ERROR);
            return 0;
        } else if (std::stoi(m_str[1]) != static_cast<int>(m_str.size() - 3)) {
            asns::CBusiness::SendFast(asns::LENGTH_ERROR);
            return 0;
        } else if (m_str[m_str.size() - 2] != "BB") {
            asns::CBusiness::SendFast(asns::CHECK_CODE_ERROR);
            return 0;
        }
        return do_str_req();
    }

    int worker() {
        int fd = Rs485::_uart_open();
        if (fd < 0) {
            return -1;
        }

        int read_count = 0;
        char buf[1024] = {0};
        while (!AcquisitionNoise::getInstance().getMonitorStatus()) {
            memset(buf, 0, sizeof(buf));
            read_count = Rs485::_uart_read(buf, sizeof(buf));

            if (read_count < 0) {
                spdlog::info("failed to read! errno = {} strerror = {}", errno, strerror(errno));
                return 0;
            }
            spdlog::info("recv request: {} len: {}, handle it...", buf, read_count);
            handle_receive(buf);
        }
        return 1;
    }

private:
    std::vector<std::string> m_str;
};