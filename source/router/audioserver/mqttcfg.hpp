#pragma once

#include "json.hpp"
#include <vector>

using json = nlohmann::json;


namespace asns {
    const std::string MQTTCFG_FILE_NAME = "/mnt/cfg/mqtt.cfg";

    class CMqttCfgData {
    public:
        std::string name;
        std::string password;
        int port;
        std::string server;
        std::string imei;
        std::string env;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CMqttCfgData, name, password, port, server, imei, env)
    };

    class CMqttCfgBusiness {
    public:
        std::vector<CMqttCfgData> business;

    public:
        int saveToJson() {
            json j;
            j["data"] = business;
            std::ofstream o(MQTTCFG_FILE_NAME);
            o << std::setw(4) << j << std::endl;
        }


        int load() {
            std::ifstream i(MQTTCFG_FILE_NAME);
            if (!i) {
                cout << "error in load! failed to open:" << MQTTCFG_FILE_NAME << endl;
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
            return 0;
        }
    };
}
