#ifndef __CVOLUME_H__
#define __CVOLUME_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include "json.hpp"
#include "utils.h"
#include "spdlog/spdlog.h"

using json = nlohmann::json;

namespace asns {

    constexpr const char* VOLUME_JSON_FILE = "/cfg/volume.json";

    class CVolumeSetResult {
    private:
        std::string cmd;
        int resultId{0};
        std::string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CVolumeSetResult, cmd, resultId, msg)

        void do_success() {
            cmd = "VolumeSet";
            resultId = 1;
            msg = "VolumeSet handle success";
        }
    };

    class CVolumeSet {
    private:
        std::string cmd;
        int volume{3};
        int vo[8] = {0, 100, 105, 110, 115, 120, 125, 130};
        int vo_ros[8] = {0, 4, 8, 12, 17, 22, 27, 32};
        std::string filePath;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CVolumeSet, cmd, volume)

        CVolumeSet() {
            CAudioCfgBusiness business;
            business.load();
            filePath = business.business[0].savePrefix + VOLUME_JSON_FILE;
        }

        std::string getFilePath() const {
            return filePath;
        }

        int getVolume() const {
            return volume;
        }

        void setVolume(int v) {
            volume = v;
        }

        void addj(int v) const {
    CUtils utils;
    if (utils.is_ros_platform()) {
        char dacxCmd[128];
        char dac0Cmd[128];
        snprintf(dacxCmd, sizeof(dacxCmd), "dspset /dev/ttyS2 dacxgan %d", vo_ros[v]);
        snprintf(dac0Cmd, sizeof(dac0Cmd), "dspset /dev/ttyS2 dac0gan %d", vo_ros[v]);
        CUtils::cmd_system(dacxCmd);
        usleep(1000 * 100);
        CUtils::cmd_system(dac0Cmd);
    } else {
        char cmd[128];
        snprintf(cmd, sizeof(cmd), "amixer set Headphone Playback %d", vo[v]);
        CUtils::cmd_system(cmd);
    }
}
        int saveToJson() {
            json j;
            j["volume"] = volume;
            std::ofstream o(filePath);
            if (!o.is_open()) {
                spdlog::error("Failed to open file for writing: {}", filePath);
                return -1;
            }
            o << std::setw(4) << j << std::endl;
            char buf[64] = {0};
            sprintf(buf, "cm set_val sys dacxgan %d", volume);
            system(buf);
            sprintf(buf, "cm set_val sys dac0gan %d", volume);
            system(buf);
            o.close();
            return 0;
        }

        void configVolume(int vol) {
            volume = vol;
            addj(volume);
            saveToJson();
        }

        int load() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                spdlog::warn("No volume file, use default volume. File name is: {}", filePath);
                return 0;
            }
            try {
                json j;
                i >> j;
                volume = j.at("volume").get<int>();
            } catch (json::parse_error &ex) {
                spdlog::error("Parse error at byte {}", ex.byte);
                i.close();
                return -1;
            }
            i.close();
            return 1;
        }

        int do_req(CSocket *pClient) {
            if (volume > 7 || volume < 0) {
                spdlog::error("Volume level error: {}", volume);
                return -1;
            }
            spdlog::info("Volume: {}, trans to: {}", volume, vo[volume]);
            addj(volume);
            CVolumeSetResult res;
            res.do_success();
            json j = res;
            std::string s = j.dump();
            spdlog::info("Volume set resp and save: {}", s);
            saveToJson();
            pClient->Send(s.c_str(), s.length());
            return 1;
        }
    };

} // namespace asns

#endif // __CVOLUME_H__