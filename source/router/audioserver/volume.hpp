#ifndef __CVOLUME_H__
#define __CVOLUME_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include "json.hpp"
#include "utils.h"

using namespace std;
using json = nlohmann::json;

namespace asns {
    const std::string VOLUME_JSON_FILE = "/cfg/volume.json";

    class CVolumeSetResult {

    private:
        string cmd;
        int resultId;
        string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CVolumeSetResult, cmd, resultId, msg)

    public:
        int do_success() {
            cmd = "VolumeSet";
            resultId = 1;
            msg = "VolumeSet handle success";
        };
    };

    class CVolumeSet {
    private:
        string cmd;
        int volume;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CVolumeSet, cmd, volume)

    private:
        int vo[8] = {0, 100, 105, 110, 115, 120, 125, 130};
        int vo_ros[8] = {0, 16, 18, 20, 26, 28, 30, 32};
        std::string filePath;
    public:
        CVolumeSet() : volume(3) {
            CAudioCfgBusiness business;
            business.load();
            filePath = business.business[0].savePrefix + VOLUME_JSON_FILE;
        }

        std::string getFilePath() const {
            return filePath;
        }

        ~CVolumeSet() {}

        int getVolume() { return volume; }

        void setVolume(const int v) { volume = v; }

    public:
        int addj(int v) {
            char cmd[128];
            CUtils utils;
            if (utils.is_ros_platform()) {
                sprintf(cmd, "dspset /dev/ttyS2 dac0gan %d", vo_ros[v]);
            } else {
                sprintf(cmd, "amixer set Headphone Playback %d", vo[v]);
            }
            system(cmd);
        }

    public:
        int saveToJson() {
            json j;
            j["volume"] = volume;
            std::ofstream o(filePath);
            o << std::setw(4) << j << std::endl;
            char buf [64] = {0};
            sprintf(buf,"cm set_val sys dac0gan %d",vo_ros[volume]);
            system(buf);
            return 0;
        }

        int load() {
            std::ifstream i(filePath);
            if (!i) {
                cout << "no volume file , use default volume. file name is:" << filePath << endl;
                return 0;
            }
            json j;
            i >> j;
            try {
                volume = j.at("volume");
            }
            catch (json::parse_error &ex) {
                std::cerr << "parse error at byte " << ex.byte << std::endl;
                return -1;
            }
            //cout << "addjust the config volume:" << volume << endl;
            //addj(volume);
            return 1;
        }

        int do_req(CSocket *pClient) {
            if (volume > 7 || volume < 0) {
                cout << "volume level error." << volume << endl;
                return -1;
            }
            cout << "volume:" << volume << ",trans to:" << vo[volume] << endl;
            addj(volume);
            CVolumeSetResult res;
            res.do_success();
            json j = res;
            std::string s = j.dump();
            cout << "volume set resp and save:" << s << endl;
            saveToJson();
            pClient->Send(s.c_str(), s.length());
            return 1;
        }
    };

} // namespace tcpserver
#endif