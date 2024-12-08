#ifndef __AUDIO_CFG_HPP__
#define __AUDIO_CFG_HPP__

#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

namespace asns {
    const std::string AUDIOCFG_FILE_NAME = "/mnt/cfg/audio.cfg";

    class CAudioCfgData {
    public:
        int iBdVal{0};
        int serverType{0};
        std::string codeVersion;
        std::string server;
        int port{0};
        std::string deviceID;
        std::string password;
        std::string serverPassword;
        std::string serial;
        std::string subSerial;
        std::string devName;
        std::string savePrefix;
        std::string env;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioCfgData, iBdVal, serverType, codeVersion, server, port, deviceID, password,
                                       serverPassword, serial, subSerial, devName, savePrefix, env)
    };

    class CAudioCfgBusiness {
    public:
        std::vector<CAudioCfgData> business;

    public:
        void saveToJson() const {
            json j;
            j["data"] = business;
            std::ofstream o(AUDIOCFG_FILE_NAME);
            if (!o.is_open()) {
                spdlog::error("Failed to open file for writing: {}", AUDIOCFG_FILE_NAME);
                return;
            }
            o << std::setw(4) << j << std::endl;
            o.close();
        }

        int load() {
            std::ifstream i(AUDIOCFG_FILE_NAME);
            if (!i.is_open()) {
                spdlog::error("Error in load! Failed to open: {}", AUDIOCFG_FILE_NAME);
                return -1;
            }
            json j;
            try {
                i >> j;
                business = j.at("data").get<std::vector<CAudioCfgData>>();
            } catch (json::parse_error &ex) {
                spdlog::error("Parse error at byte {}", ex.byte);
                i.close();
                return -1;
            }
            i.close();
            return 0;
        }

        std::string getAudioFilePath() {
            if (load() != 0) {
                return {};
            }
            return business[0].savePrefix + "/audiodata/";
        }
    };

}
#endif