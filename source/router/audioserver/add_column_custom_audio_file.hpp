#pragma once

#include <iostream>
#include <fstream>
#include "audiocfg.hpp"
#include "json.hpp"
#include "utils.h"
#include "public.hpp"
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

        void setType(const int type){
            this->type = type;
        }
        void setSize(const int size){
            this->size = size;
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

        void updateSize(const std::string name, const int size) {
            Columnload();
            for (auto &it: business) {
                if (it.getName() == name) {
                    it.size = size;
                    saveJson();
                    break;
                }
            }

        }

        int Columnload() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                return 0;
            }
            json js;
            try {
                i >> js;
                std::cout << js.dump() << std::endl;
                business = js;
            } catch (json::parse_error &ex) {
                std::cout << "json parse error" << std::endl;
                return 0;
            }
        }

        int load() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                return 0;
            }
            json js;
            try {
                i >> js;
                business = js;
            } catch (json::parse_error &ex) {
                std::cout << "json parse error" << std::endl;
                return 0;
            }
            CUtils utils;
            CAudioCfgBusiness cfgBusiness;
            cfgBusiness.load();
            CAddCustomAudioFileBusiness bus;
            bus.load();
            for (const auto &it: bus.business) {
                CAddColumnCustomAudioFileData data;
                data.setName(it.customAudioName);
                std::string path = cfgBusiness.getAudioFilePath() + it.customAudioName;
                data.setType(asns::AUDIO_FILE_TYPE);
                data.setSize(utils.get_file_size(path));
                this->business.push_back(data);
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

        int exist(std::string &name) {
            Columnload();
            for (auto it = business.begin(); it != business.end(); ++it) {
                if (it->getName() == name) {
                    return 1;
                }
            }
            return 0;
        }

    public:
        std::vector<CAddColumnCustomAudioFileData> business;
        std::string filePath;
    };

} // namespace asms