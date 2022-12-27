#pragma once

#include <iostream>
#include <fstream>
#include "json.hpp"
#include "add_custom_audio_file.hpp"
#include "audiocfg.hpp"

using json = nlohmann::json;

namespace asns {
    const std::string ADD_MQTT_CUSTOM_AUDIO_FILE = "/cfg/add_mqtt_custom_audio_file.json";

    class CAddMqttCustomAudioFileData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAddMqttCustomAudioFileData, fileName, audioUploadRecordId)

        void setName(const std::string &name) {
            fileName = name;
        }

        std::string getName() const {
            return fileName;
        }

        void setAudioUploadRecordId(const int id) {
            audioUploadRecordId = id;
        }

        int getAudioUploadRecordId() const {
            return audioUploadRecordId;
        }

    private:
        std::string fileName;
        int audioUploadRecordId;
    };

    class CAddMqttCustomAudioFileBusiness {
    public:
        CAddMqttCustomAudioFileBusiness() {
            CAudioCfgBusiness business;
            business.load();
            filePath = business.business[0].savePrefix + ADD_MQTT_CUSTOM_AUDIO_FILE;
        }

        std::string getFilePath() const {
            return filePath;
        }

        int mqttLoad() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                std::cout << "ifstream open fail" << std::endl;
                return 0;
            }
            json js;
            i >> js;
            std::cout << "mqtt load json:" << js.dump() << std::endl;
            business = js;
        }

        void saveJson() {
            std::ofstream o(filePath);
            if (!o.is_open()) {
                std::cout << "ofstream open fail" << std::endl;
                return;
            }
            json js = business;
            std::cout << "mqtt saveJson :" << js.dump() << std::endl;
            o << js << std::endl;
        }

        int deleteData(const std::string &name) {
            mqttLoad();
            for (auto it = business.begin(); it != business.end(); ++it) {
                if (it->getName() == name) {
                    char cmd[256];
                    CAudioCfgBusiness cfg;
                    cfg.load();
                    sprintf(cmd, "rm %s%s", cfg.getAudioFilePath().c_str(), name.c_str());
                    std::cout << cmd << std::endl;
                    system(cmd);
                    business.erase(it);
                    saveJson();
                    return 1;
                }
            }
            return 3;
        }

        bool exist(const std::string &name) {
            mqttLoad();
            for (auto it = business.begin(); it != business.end(); ++it) {
                if (it->getName() == name) {
                    return true;
                }
            }
            return false;
        }

    public:
        std::vector<CAddMqttCustomAudioFileData> business;
        std::string filePath;
    };

} // namespace asms