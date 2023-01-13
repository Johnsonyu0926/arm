#pragma once

#include <iostream>
#include <fstream>
#include "audiocfg.hpp"
#include "json.hpp"
#include "add_custom_audio_file.hpp"

using json = nlohmann::json;

namespace asns {
    const std::string ADD_COLUMN_CUSTOM_AUDIO_FILE = "/cfg/add_column_custom_audio_file.json";

    class CAddColumnCustomAudioFileData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAddColumnCustomAudioFileData, type, fileName, size)

        void setName(const std::string &name) {
            fileName = name;
        }

        std::string getName() const {
            return fileName;
        }

    public:
        int type;
        std::string fileName;
        int size;
    };

    class CAddColumnCustomAudioFileBusiness {
    public:
        CAddColumnCustomAudioFileBusiness() {
            CAudioCfgBusiness business;
            business.load();
            filePath = business.business[0].savePrefix + ADD_COLUMN_CUSTOM_AUDIO_FILE;
        }

        std::string getFilePath() const {
            return filePath;
        }

        int Columnload() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                return 0;
            }
            json js;
            i >> js;
            business = js;
        }

        int load() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                return 0;
            }
            json js;
            i >> js;
            business = js;

            CAddCustomAudioFileBusiness bus;
            bus.load();
            for (const auto &it: bus.business) {
                CAddColumnCustomAudioFileData data;
                data.setName(it.customAudioName);
                business.push_back(data);
            }
        }

        void saveJson() {
            std::ofstream o(filePath);
            json js = business;
            o << js << std::endl;
        }

        int isNameEmpty(const std::string &name) {
            load();
            for (const auto &it: business) {
                if (it.getName() == name) {
                    return 1;
                }
            }

            CAddCustomAudioFileBusiness bus;
            bus.load();
            for (const auto &it: bus.business) {
                if (it.filename == name) {
                    return 1;
                }
            }
            return 0;
        }

        int deleteAudio(std::string name) {
            this->Columnload();
            for (auto it = business.begin(); it != business.end(); ++it) {
                if (it->getName() == name) {
                    char cmd[256];
                    CAudioCfgBusiness cfg;
                    cfg.load();

                    sprintf(cmd, "rm %s%s", cfg.getAudioFilePath().c_str(), name.c_str());
                    system(cmd);
                    business.erase(it);
                    this->saveJson();
                    return 1;
                }
            }
            return 0;
        }

        void getAudioList(){

        }

    public:
        std::vector<CAddColumnCustomAudioFileData> business;
        std::string filePath;
    };

} // namespace asms