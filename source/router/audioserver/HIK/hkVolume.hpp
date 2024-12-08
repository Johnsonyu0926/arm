#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "volume.hpp"
#include "json.hpp"
#include "utils.h"

using json = nlohmann::json;

namespace asns {
    class CHKVolumeInfo {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CHKVolumeInfo, audioOutID, volume)

        int audioOutID{0};
        int volume{0};
    };

    class CHKVolume {
    public:
        CHKVolume() = default;
        ~CHKVolume() = default;

        void parse(const std::string &res) {
            try {
                json js = json::parse(res);
                if (js.contains("data") && js["data"].contains("Value") && js["data"]["Value"].contains("BroadcastAudioCfgList")) {
                    info = js["data"]["Value"]["BroadcastAudioCfgList"].get<std::vector<CHKVolumeInfo>>();
                } else {
                    LOG(WARNING) << "not found data key.";
                    return;
                }
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                return;
            }
            volume = (info[0].volume < 10 && info[0].volume > 0) ? 10 : info[0].volume;
            volume = (volume / 10) * 10;
            volumeSet(volume);
        }

        void volumeSet(int volume) {
            CUtils utils;
            if (utils.is_ros_platform()) {
                CVolumeSet v;
                v.setVolume(vo_ros[volume]);
                v.addj(vo_ros[volume]);
                v.saveToJson();
            } else {
                std::string cmd = "amixer set Headphone Playback " + std::to_string(vo[volume]);
                system(cmd.c_str());
            }
        }

    private:
        int volume{0};
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