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
            CUtils utils;
            if (utils.is_ros_platform()) {
                CVolumeSet v;
                v.setVolume(vo_ros[volume]);
                v.addj(vo_ros[volume]);
                v.saveToJson();
            }else{
                char buf[64] = {0};
                sprintf(buf, "amixer set Headphone Playback %d", vo[volume]);
                system(buf);
            }
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
                                  {10,  1},
                                  {20,  1},
                                  {30,  2},
                                  {40,  2},
                                  {50,  3},
                                  {60,  4},
                                  {70,  5},
                                  {80,  6},
                                  {90,  6},
                                  {100, 7}};
    };
}