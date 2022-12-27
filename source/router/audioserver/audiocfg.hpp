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
        string serial;
        string subSerial;
        string devName;
        string savePrefix;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioCfgData, iBdVal,serverType, codeVersion, server, port, deviceID, password, serial,
                                       subSerial,
                                       devName, savePrefix)
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
        }


        int load() {
            std::ifstream i(AUDIOCFG_FILE_NAME);
            if (!i) {
                cout << "error in load! failed to open:" << AUDIOCFG_FILE_NAME << endl;
                return -1;
            }
            json j;
            i >> j;
            try {
                business = j.at("data");
            }
            catch (json::parse_error &ex) {
                std::cerr << "parse error at byte " << ex.byte << std::endl;
                return -1;
            }
            //cout << "success load cfg from :" << AUDIOCFG_FILE_NAME << endl;
            //cout << "count:" << business.size() << endl;
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
