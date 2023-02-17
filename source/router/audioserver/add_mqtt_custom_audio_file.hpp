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
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAddMqttCustomAudioFileData, fileName, audioUploadRecordId)

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
                DS_TRACE("ifstream open fail");
                return 0;
            }
            json js;
            try {
                i >> js;
                DS_TRACE("mqtt load json:" << js.dump().c_str());
                business = js;
            } catch (json::parse_error &ex) {
                std::cerr << "parse error at byte " << ex.byte << std::endl;
                return 0;
            }
        }

        void saveJson() {
            std::ofstream o(filePath);
            if (!o.is_open()) {
                DS_TRACE("ofstream open fail");
                return;
            }
            json js = business;
            DS_TRACE("mqtt saveJson :" << js.dump().c_str());
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
                    DS_TRACE(cmd);
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