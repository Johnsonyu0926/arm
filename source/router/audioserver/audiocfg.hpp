#ifndef __AUDIO_CFG_HPP__
#define __AUDIO_CFG_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

namespace asns {
    const std::string AUDIOCFG_FILE_NAME = "/mnt/cfg/audio.cfg";

    class CAudioCfgData {
    public:
        int iBdVal;
        int serverType;
        string codeVersion;
        string server;
        int port;
        string deviceID;
        string password;
        string serverPassword;
        string serial;
        string subSerial;
        string devName;
        string savePrefix;
        string env;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioCfgData, iBdVal, serverType, codeVersion, server, port, deviceID, password,
                                       serverPassword,
                                       serial,
                                       subSerial,
                                       devName, savePrefix, env)
    };

    class CAudioCfgBusiness {
    public:
        vector <CAudioCfgData> business;

    public:
        int saveToJson() {
            json j;
            j["data"] = business;
            std::ofstream o(AUDIOCFG_FILE_NAME);
            o << std::setw(4) << j << std::endl;
            o.close();
        }


        int load() {
            std::ifstream i(AUDIOCFG_FILE_NAME);
            if (!i) {
                std::cerr << "error in load! failed to open:" << AUDIOCFG_FILE_NAME << std::endl;
                return -1;
            }
            json j;
            try {
                i >> j;
                business = j.at("data");
            }
            catch (json::parse_error &ex) {
                std::cerr << "parse error at byte " << ex.byte << std::endl;
                i.close();
                return -1;
            }
            i.close();
            return 0;
        }

        std::string getAudioFilePath() {
            this->load();
            std::string res = business[0].savePrefix + "/audiodata/";
            return res;
        }
    };

}
#endif
