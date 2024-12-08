#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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

        void setAudioUploadRecordId(int id) {
            audioUploadRecordId = id;
        }

        int getAudioUploadRecordId() const {
            return audioUploadRecordId;
        }

    private:
        std::string fileName;
        int audioUploadRecordId{0};
    };

    class CAddMqttCustomAudioFileBusiness {
    public:
        CAddMqttCustomAudioFileBusiness() {
            CAudioCfgBusiness cfgBusiness;
            cfgBusiness.load();
            if (!cfgBusiness.business.empty()) {
                filePath = cfgBusiness.business[0].savePrefix + ADD_MQTT_CUSTOM_AUDIO_FILE;
            }
        }

        std::string getFilePath() const {
            return filePath;
        }

        bool mqttLoad() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                LOG(INFO) << "ifstream open fail";
                return false;
            }
            json js;
            try {
                i >> js;
                LOG(INFO) << "mqtt load json: " << js.dump();
                business = js.get<std::vector<CAddMqttCustomAudioFileData>>();
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                i.close();
                return false;
            }
            i.close();
            return true;
        }

        void saveJson() const {
            std::ofstream o(filePath);
            if (!o.is_open()) {
                LOG(INFO) << "ofstream open fail";
                return;
            }
            json js = business;
            LOG(INFO) << "mqtt saveJson: " << js.dump();
            o << js << std::endl;
            o.close();
        }

        bool deleteData(const std::string &name) {
            if (mqttLoad()) {
                auto it = std::find_if(business.begin(), business.end(), [&name](const auto& data) {
                    return data.getName() == name;
                });
                if (it != business.end()) {
                    CAudioCfgBusiness cfg;
                    cfg.load();
                    std::string cmd = "rm " + cfg.getAudioFilePath() + name;
                    LOG(INFO) << cmd;
                    if (system(cmd.c_str()) == -1) {
                        LOG(ERROR) << "Failed to execute command: " << cmd;
                        return false;
                    }
                    business.erase(it);
                    saveJson();
                    return true;
                }
            }
            return false;
        }

        bool exist(const std::string &name) {
            if (mqttLoad()) {
                return std::any_of(business.begin(), business.end(), [&name](const auto& data) {
                    return data.getName() == name;
                });
            }
            return false;
        }

    public:
        std::vector<CAddMqttCustomAudioFileData> business;
        std::string filePath;
    };

} // namespace asns