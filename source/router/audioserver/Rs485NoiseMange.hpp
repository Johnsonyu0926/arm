#pragma once

#include <numeric>
#include <thread>
#include "AcquisitionNoise.hpp"
#include "utils.h"
#include "Rs485.hpp"
#include "PlayStatus.hpp"
#include "spdlog/spdlog.h"

class Rs485NoiseMange {
public:
    static int handle_receive(const unsigned char buf[]) {
        uint16_t crc = CUtils::crc16((const uint8_t *) buf, 5);
        if ((crc & 0x00FF) != buf[5] || ((crc & 0xFF00) >> 8) != buf[6]) {
            spdlog::warn("CRC error");
            return 0;
        }
        AcquisitionNoise::getInstance().noiseDeque.push_front(((buf[3] << 8) | buf[4]) / 10.0);
        if (static_cast<int>(AcquisitionNoise::getInstance().noiseDeque.size()) > AcquisitionNoise::getInstance().getCalcCycle()) {
            AcquisitionNoise::getInstance().noiseDeque.pop_back();
        }
        if (!AcquisitionNoise::getInstance().noiseDeque.empty() && AcquisitionNoise::getInstance().getMonitorStatus()) {
            AcquisitionNoise::getInstance().setDecibel(
                    round(std::accumulate(AcquisitionNoise::getInstance().noiseDeque.begin(),
                                          AcquisitionNoise::getInstance().noiseDeque.end(), 0.0) /
                          static_cast<double>(AcquisitionNoise::getInstance().noiseDeque.size()) * 10) / 10);
        }
        spdlog::info("noiseDeque.size: {} decibel: {}", AcquisitionNoise::getInstance().noiseDeque.size(),
                     AcquisitionNoise::getInstance().getDecibel());
        return 1;
    }

    static int startCollectingNoise() {
        int fd = Rs485::_uart_open();
        if (fd < 0) {
            spdlog::error("Failed to open UART");
            return 0;
        }
        int read_count = 0;
        unsigned char buf[255] = {0};
        tcflush(fd, TCIOFLUSH);
        fcntl(fd, F_SETFL, 0);    // 串口阻塞
        while (AcquisitionNoise::getInstance().getMonitorStatus()) {
            spdlog::info("-------wait ---------- ");
            std::this_thread::sleep_for(std::chrono::seconds(AcquisitionNoise::getInstance().getFrequency()));
            if (PlayStatus::getInstance().getPlayState()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
            Rs485::noise_write(fd);
            tcflush(fd, TCIOFLUSH);
            memset(buf, 0, sizeof(buf));
            read_count = Rs485::select_read(fd, buf, sizeof(buf), 1000);
            if (read_count < 0) {
                spdlog::error("Failed to read! errno = {} strerror= {}", errno, strerror(errno));
                break;
            }
            for (int i = 0; i < read_count; i++) {
                spdlog::info("{:02x}", buf[i]);
            }
            handle_receive(buf);
        }
        Rs485::uart_close();
        spdlog::info("Close noise fd: {}", fd);
        return 1;
    }
};