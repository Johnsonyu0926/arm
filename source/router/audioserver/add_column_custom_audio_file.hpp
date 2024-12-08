#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <optional>
#include <string>
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

        void setType(int type) {
            this->type = type;
        }

        void setSize(int size) {
            this->size = size;
        }

    public:
        int type{0};
        std::string fileName;
        int size{0};
    };

    class CAddColumnCustomAudioFileBusiness {
    public:
        CAddColumnCustomAudioFileBusiness() {
            CAudioCfgBusiness business;
            business.load();
            if (!business.business.empty()) {
                filePath = business.business[0].savePrefix + ADD_COLUMN_CUSTOM_AUDIO_FILE;
            }
        }

        std::string getFilePath() const {
            return filePath;
        }

        void updateSize(const std::string& name, int size) {
            if (Columnload()) {
                for (auto &it : business) {
                    if (it.getName() == name) {
                        it.setSize(size);
                        saveJson();
                        break;
                    }
                }
            }
        }

        bool Columnload() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                return false;
            }
            json js;
            try {
                i >> js;
                LOG(INFO) << js.dump();
                business = js.get<std::vector<CAddColumnCustomAudioFileData>>();
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                i.close();
                return false;
            }
            i.close();
            return true;
        }

        bool load() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                return false;
            }
            json js;
            try {
                i >> js;
                business = js.get<std::vector<CAddColumnCustomAudioFileData>>();
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                i.close();
                return false;
            }
            i.close();
            CUtils utils;
            CAudioCfgBusiness cfgBusiness;
            cfgBusiness.load();
            CAddCustomAudioFileBusiness bus;
            bus.load();
            for (const auto &it : bus.business) {
                CAddColumnCustomAudioFileData data;
                data.setName(it.customAudioName);
                std::string path = cfgBusiness.getAudioFilePath() + it.customAudioName;
                data.setType(asns::AUDIO_FILE_TYPE);
                data.setSize(utils.get_file_size(path));
                this->business.push_back(data);
            }
            return true;
        }

        void saveJson() const {
            std::ofstream o(filePath);
            if (!o.is_open()) {
                LOG(ERROR) << "Failed to open file for writing: " << filePath;
                return;
            }
            json js = business;
            o << js << std::endl;
            o.close();
        }

        bool isNameEmpty(const std::string &name) {
            if (load()) {
                for (const auto &it : business) {
                    if (it.getName() == name) {
                        return true;
                    }
                }

                CAddCustomAudioFileBusiness bus;
                bus.load();
                for (const auto &it : bus.business) {
                    if (it.filename == name) {
                        return true;
                    }
                }
            }
            return false;
        }

        bool deleteAudio(const std::string& name) {
            if (Columnload()) {
                for (auto it = business.begin(); it != business.end(); ++it) {
                    if (it->getName() == name) {
                        CAudioCfgBusiness cfg;
                        cfg.load();

                        std::string cmd = "rm " + cfg.getAudioFilePath() + name;
                        if (system(cmd.c_str()) == -1) {
                            LOG(ERROR) << "Failed to execute command: " << cmd;
                            return false;
                        }
                        business.erase(it);
                        saveJson();
                        return true;
                    }
                }
            }
            return false;
        }

        bool exist(const std::string& name) {
            if (Columnload()) {
                for (const auto &busines : business) {
                    if (busines.getName() == name) {
                        return true;
                    }
                }
            }
            return false;
        }

    public:
        std::vector<CAddColumnCustomAudioFileData> business;
        std::string filePath;
    };

} // namespace asns