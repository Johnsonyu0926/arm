#pragma once

#include <iostream>
#include "volume.hpp"
#include "json.hpp"
#include "utils.h"

using json = nlohmann::json;

namespace asns {
    class CHKVolumeInfo {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CHKVolumeInfo, audioOutID, volume)

    public:
        int audioOutID;
        int volume;
    };

    class CHKVolume {
    public:
        explicit CHKVolume() = default;

        ~CHKVolume() = default;

        std::string parse(const std::string &res) {
            json js = json::parse(res);
            info = js.at("data").at("Value").at("BroadcastAudioCfgList");
            volume = info[0].volume < 10 && info[0].volume > 0 ? 10 : info[0].volume;
            volume /= 10;
            volume *= 10;
            this->volumeSet(volume);
        }

        bool volumeSet(const int volume) {
            char buf[64] = {0};
            CUtils utils;
            if (utils.is_ros_platform()) {
                sprintf(buf, "amixer set PCM playback %d", vo_ros[volume]);
                utils.cmd_system("cm set_val sys volume " + std::to_string(vo_ros[volume]));
            } else {
                sprintf(buf, "amixer set Headphone Playback %d", vo[volume]);
            }
            DS_TRACE(buf);
            system(buf);
        }

    private:
        int volume;
        std::vector<CHKVolumeInfo> info;
        std::map<int, int> vo{{0,   0},
                              {10,  100},
                              {20,  100},
                              {30,  100},
                              {40,  103},
                              {50,  105},
                              {60,  110},
                              {70,  115},
                              {80,  120},
                              {90,  125},
                              {100, 130}};
        std::map<int, int> vo_ros{{0,   0},
                                  {10,  35},
                                  {20,  35},
                                  {30,  35},
                                  {40,  37},
                                  {50,  40},
                                  {60,  42},
                                  {70,  44},
                                  {80,  46},
                                  {90,  48},
                                  {100, 50}};
    };
}