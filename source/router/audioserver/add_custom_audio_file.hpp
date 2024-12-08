#ifndef __ADD_CUSTOM_AUDIO_FILE_H__
#define __ADD_CUSTOM_AUDIO_FILE_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>
#include <string>
#include "utils.h"
#include "json.hpp"
#include "doorsbase.h"
#include "audiocfg.hpp"
#include "public.hpp"
#include "AudioPlayStatus.hpp"
#include "AudioPlayUtils.hpp"

extern int g_updateJson;
using json = nlohmann::json;

namespace asns {
    const std::string ADD_CUSTOM_AUDIO_FILE = "/cfg/add_custom_audio_file.json";

    class CAddCustomAudioFileData {
    public:
        int customAudioID{0};
        std::string customAudioName;
        std::string filePath;
        std::string filePathType;
        char filename[128]{};

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAddCustomAudioFileData, customAudioID, customAudioName, filePath, filePathType)

        const char* getCustomAudioName() const { return customAudioName.c_str(); }

        char* parseFile() {
            char* p1 = strstr(filePath.c_str(), "?filename=");
            if (p1) {
                p1 += strlen("?filename=");
                char* p2 = strchr(p1, '&');
                if (p2) {
                    strncpy(filename, p1, p2 - p1);
                }
            }
            return filename;
        }

        const char* getFileName() const { return filename; }

        const char* getFilePath() const { return filePath.c_str(); }

        int getCustomAudioID() const { return customAudioID; }

        int play(const std::string& prefix, const std::string& endTime, int priority) {
            if (strlen(filename) <= 0) {
                parseFile();
            }
            CUtils utils;
            LOG(INFO) << "custom audio file data : playing " << customAudioID << ", file " << customAudioName;

            std::string cmd = "madplay " + prefix + getFileName();
            std::string background_cmd = cmd + "&";
            LOG(INFO) << "playing status: " << CUtils::get_process_status("madplay");
            system(background_cmd.c_str()); // background play
            LOG(INFO) << "cmd: " << background_cmd;
            PlayStatus::getInstance().setPlayId(asns::TIMED_TASK_PLAYING);
            PlayStatus::getInstance().setPriority(priority);

            CSTime tnow;
            tnow.GetCurTime();
            int h, m, s;
            sscanf(endTime.c_str(), "%d:%d:%d", &h, &m, &s);
            CSTime t1(tnow.m_nYear, tnow.m_nMon, tnow.m_nDay, h, m, s);

            while (true) {
                tnow.GetCurTime();
                int exist = CUtils::get_process_status(cmd.c_str());
                if (!exist) {
                    LOG(INFO) << "play finish of cmd: " << cmd;
                    break;
                }
                if (tnow.m_time <= t1.m_time) {
                    LOG(INFO) << "playing: " << cmd;
                    sleep(1);
                } else {
                    LOG(INFO) << "out of time. play stop! last cmd is: " << cmd;
                    AudioPlayUtil::audio_stop();
                }
                if (g_updateJson) {
                    LOG(INFO) << "get new json stop now audio";
                    AudioPlayUtil::audio_stop();
                }
                sleep(2);
            }
            return 0;
        }
    };

    class CSpeechSynthesisBusiness {
    public:
        int play(const std::string& endTime, int priority, const std::string& txt, const std::string& voiceType, int model) {
            if (model == SINGLE_PLAY) {
                AudioPlayUtil::tts_num_play(1, txt);
                return 0;
            }
            AudioPlayUtil::tts_num_play(1, txt, ASYNC_START, 50, FEMALE);
            CSTime tnow;
            tnow.GetCurTime();
            int h, m, s;
            sscanf(endTime.c_str(), "%d:%d:%d", &h, &m, &s);
            CSTime t1(tnow.m_nYear, tnow.m_nMon, tnow.m_nDay, h, m, s);

            while (true) {
                tnow.GetCurTime();
                int exist = CUtils::get_process_status("ffplay") || CUtils::get_process_status("tts.sh") || CUtils::get_process_status("madplay");
                if (!exist) {
                    LOG(INFO) << "play finish of tts";
                    break;
                }
                if (tnow.m_time <= t1.m_time) {
                    LOG(INFO) << "playing tts";
                    sleep(1);
                } else {
                    LOG(INFO) << "out of time. play stop! last tts";
                    AudioPlayUtil::audio_stop();
                }
                if (g_updateJson) {
                    LOG(INFO) << "get new json stop now tts";
                    AudioPlayUtil::audio_stop();
                }
                sleep(2);
            }
            return 0;
        }
    };

    class CAddCustomAudioFileBusiness {
    public:
        std::vector<CAddCustomAudioFileData> business;
        std::string filePath;
        std::string savePrefix;

        CAddCustomAudioFileBusiness() {
            CAudioCfgBusiness cfgBusiness;
            cfgBusiness.load();
            if (!cfgBusiness.business.empty()) {
                filePath = cfgBusiness.business[0].savePrefix + ADD_CUSTOM_AUDIO_FILE;
                savePrefix = cfgBusiness.business[0].savePrefix;
            }
        }

        std::string getFilePath() const {
            return filePath;
        }

        int parseRequest(const std::string& data) {
            json j = json::parse(data, nullptr, false);
            try {
                business = j.at("data").get<std::vector<CAddCustomAudioFileData>>();
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                return -1;
            }
            return 0;
        }

        int add(const std::string& data) {
            json j = json::parse(data, nullptr, false);
            try {
                CAddCustomAudioFileData node = j.at("data").get<CAddCustomAudioFileData>();
                if (exist(node)) {
                    LOG(INFO) << "exist node ";
                    return 0;
                }
                append(node);
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                return -1;
            }
            return 0;
        }

        bool exist(const CAddCustomAudioFileData& node) const {
            return std::any_of(business.begin(), business.end(), [&node](const auto& data) {
                return data.getCustomAudioID() == node.getCustomAudioID();
            });
        }

        bool exist(const std::string& name) {
            load();
            return std::any_of(business.begin(), business.end(), [&name](const auto& data) {
                return data.customAudioName == name;
            });
        }

        void append(const CAddCustomAudioFileData& node) {
            business.push_back(node);
            download(node);
            saveToJson();
        }

        void saveToJson() const {
            json j;
            j["data"] = business;
            std::ofstream o(filePath);
            if (!o.is_open()) {
                LOG(ERROR) << "Failed to open file for writing: " << filePath;
                return;
            }
            o << std::setw(4) << j << std::endl;
            o.close();
        }

        void download(const CAddCustomAudioFileData& node) const {
            std::string cmd = "dodownload.sh \"" + node.getFilePath() + "\" \"" + savePrefix + "/" + node.getFileName() + "\"";
            LOG(INFO) << "===========NLOHMANN_DEFINE_TYPE_INTRUSIVE=========:" << cmd;
            system(cmd.c_str());
        }

        bool load() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                return false;
            }
            json j;
            try {
                i >> j;
                business = j.at("data").get<std::vector<CAddCustomAudioFileData>>();
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                return false;
            }
            LOG(INFO) << "load " << ADD_CUSTOM_AUDIO_FILE << " success! ";
            LOG(INFO) << "count: " << business.size();
            return true;
        }

        int play(int id, const std::string& endtime, int priority) const {
            LOG(INFO) << "business count: " << business.size();
            for (const auto& data : business) {
                LOG(INFO) << "CustomAudioID: " << data.getCustomAudioID() << " id: " << id;
                if (data.getCustomAudioID() == id) {
                    LOG(INFO) << "savePrefix: " << savePrefix << " endtime: " << endtime << " priority: " << priority;
                    data.play(savePrefix, endtime, priority);
                }
            }
            return 0;
        }

        bool deleteAudio(const std::string& name) {
            load();
            auto it = std::find_if(business.begin(), business.end(), [&name](const auto& data) {
                return data.customAudioName == name;
            });
            if (it != business.end()) {
                CAudioCfgBusiness cfg;
                cfg.load();
                std::string cmd = "rm " + cfg.getAudioFilePath() + name;
                LOG(INFO) << "cmd: " << cmd;
                if (system(cmd.c_str()) == -1) {
                    LOG(ERROR) << "Failed to execute command: " << cmd;
                    return false;
                }
                business.erase(it);
                saveToJson();
                return true;
            }
            return false;
        }
    };
}
#endif