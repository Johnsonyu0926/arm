#ifndef __ADD_CUSTOM_AUDIO_FILE_H__
#define __ADD_CUSTOM_AUDIO_FILE_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include "utils.h"
#include "json.hpp"
#include "doorsbase.h"
#include "audiocfg.hpp"
#include "public.hpp"

using namespace std;
using json = nlohmann::json;
// extern vector<int> playing;

namespace asns {
    const std::string ADD_CUSTOM_AUDIO_FILE = "/cfg/add_custom_audio_file.json";

    class CAddCustomAudioFileData {
    public:
        int customAudioID;
        string customAudioName;
        string filePath;
        string filePathType;
        char filename[128];

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAddCustomAudioFileData, customAudioID, customAudioName, filePath,
                                                    filePathType)

        const char *getCustomAudioName() { return customAudioName.c_str(); }

        char *parseFile() {
            char *p1 = strstr(filePath.c_str(), "?filename=");
            if (p1) {
                p1 += strlen("?filename=");
                char *p2 = strchr(p1, '&');
                if (p2) {
                    strncpy(filename, p1, p2 - p1);
                }
            }
            return filename;
        }

        const char *getFileName() { return filename; }

        const char *getFilePath() { return filePath.c_str(); }

        int getCustomAudioID() { return customAudioID; }

        int play(string prefix, string endTime, const int priority) {
            if (strlen(filename) <= 0) {
                parseFile();
            }
            CUtils utils;
            DS_TRACE("custom audio file data : playing " << customAudioID << ",file" << customAudioName.c_str());

            // background play. and monitor....
            //
            char cmd[256] = {0};
            sprintf(cmd, "madplay %s%s", prefix.c_str(), getFileName());
            char background_cmd[256] = {0};
            sprintf(background_cmd, "%s&", cmd);
            system(background_cmd); // background play
            DS_TRACE("cmd: " << background_cmd);
            PlayStatus::getInstance().setPlayId(asns::TIMED_TASK_PLAYING);
            PlayStatus::getInstance().setPriority(priority);

            CSTime tnow;

            tnow.GetCurTime();
            int h, m, s;
            sscanf(endTime.c_str(), "%d:%d:%d", &h, &m, &s);
            CSTime t1(tnow.m_nYear, tnow.m_nMon, tnow.m_nDay, h, m, s);

            while (1) {

                tnow.GetCurTime();

                int exist = utils.get_process_status(cmd);
                if (!exist) {
                    DS_TRACE("play finish of cmd:" << cmd);
                    break;
                }
                if (tnow.m_time <= t1.m_time) {
                    // current time is match. waiting
                    DS_TRACE("playing:" << cmd);
                    sleep(1);
                } else {
                    // out of time. stop if it is playing...
                    DS_TRACE("out of time. play stop! last cmd is:" << cmd);
                    utils.audio_stop();
                }
                sleep(2);
            }
            return 0;
        }
    };

    class CSpeechSynthesisBusiness {
    public:
        int play(string endTime, const int priority, const std::string &txt, const std::string &voiceType,const int model) {
            CUtils utils;
            if (voiceType.compare("male") == 0) {
                utils.txt_to_audio(txt, 50, MALE);
            } else {
                utils.txt_to_audio(txt, 50, FEMALE);
            }
            if(model == 1){
                utils.tts_num_play(1,ASYNC_START);
                return 0;
            }
            utils.tts_num_play(1, ASYNC_START);
            CSTime tnow;

            tnow.GetCurTime();
            int h, m, s;
            sscanf(endTime.c_str(), "%d:%d:%d", &h, &m, &s);
            CSTime t1(tnow.m_nYear, tnow.m_nMon, tnow.m_nDay, h, m, s);

            while (1) {
                tnow.GetCurTime();
                int exist = utils.get_process_status("aplay");
                if (!exist) {
                    DS_TRACE("play finish of tts");
                    break;
                }
                if (tnow.m_time <= t1.m_time) {
                    // current time is match. waiting
                    DS_TRACE("playing tts");
                    sleep(1);
                } else {
                    // out of time. stop if it is playing...
                    DS_TRACE("out of time. play stop! last tts");
                    utils.audio_stop();
                }
                sleep(2);
            }
            return 0;
        }
    };

    class CAddCustomAudioFileBusiness {
    public:
        vector <CAddCustomAudioFileData> business;
        std::string filePath;
    public:
        string savePrefix;

        CAddCustomAudioFileBusiness() {
            CAudioCfgBusiness business;
            business.load();
            filePath = business.business[0].savePrefix + ADD_CUSTOM_AUDIO_FILE;
        }

        std::string getFilePath() const {
            return filePath;
        }

        int parseRequest(string data) {
            cout << data << endl;
            json j = json::parse(data, nullptr, false);
            try {
                business = j.at("data");
            }
            catch (json::parse_error &ex) {
                std::cerr << "parse error at byte " << ex.byte << std::endl;
                return -1;
            }
            return 0;
        }

        int add(string data) {
            cout << data << endl;
            json j = json::parse(data, nullptr, false);
            try {
                CAddCustomAudioFileData node = j.at("data");

                if (exist(node)) {
                    DS_TRACE("exist node ");
                    return 0;
                }
                append(node);
            }
            catch (json::parse_error &ex) {
                std::cerr << "parse error at byte " << ex.byte << std::endl;
                return -1;
            }
            return 0;
        }

        int exist(CAddCustomAudioFileData node) {
            for (unsigned int i = 0; i < business.size(); i++) {
                CAddCustomAudioFileData data = business.at(i);
                if (data.getCustomAudioID() == node.getCustomAudioID()) {
                    return 1;
                }
            }
            return 0;
        }

        int exist(std::string &name) {
            load();
            for (auto it = business.begin(); it != business.end(); ++it) {
                if (it->customAudioName == name) {
                    return 1;
                }
            }
            return 0;
        }

        int append(CAddCustomAudioFileData node) {
            business.push_back(node);
            download(node);
            saveToJson();
            return 0;
        }

        int saveToJson() {
            json j;
            j["data"] = business;
            std::ofstream o(filePath);
            o << std::setw(4) << j << std::endl;
            o.close();
            return 0;
        }

        int download(CAddCustomAudioFileData node) {
            char cmd[1024];
            node.parseFile();
            sprintf(cmd, "dodownload.sh \"%s\" \"%s/%s\"",
                    (char *) node.getFilePath(),
                    savePrefix.c_str(),
                    (char *) node.getFileName());
            DS_TRACE("===========NLOHMANN_DEFINE_TYPE_INTRUSIVE=========:" << cmd);
            system(cmd);
            return 0;
        }

        int load() {
            std::ifstream i(filePath);
            if (!i) {
                return 0;
            }
            json j;
            try {
                i >> j;
                business = j.at("data");
            }
            catch (json::parse_error &ex) {
                std::cerr << "parse error at byte " << ex.byte << std::endl;
                return -1;
            }
            DS_TRACE("load " << ADD_CUSTOM_AUDIO_FILE.c_str() << "  success! ");
            DS_TRACE("count:" << business.size());
            return 0;
        }

        int play(int id, string endtime, const int priority) {
            DS_TRACE("business count:" << business.size());
            for (unsigned int i = 0; i < business.size(); i++) {
                CAddCustomAudioFileData data = business.at(i);
                DS_TRACE("CustomAudioID： " << data.getCustomAudioID() << " id: " << id);
                if (data.getCustomAudioID() == id) {
                    DS_TRACE("savePrefix： " << savePrefix.c_str() << " endtime " << endtime.c_str() << "priority" << priority);
                    data.play(savePrefix, endtime, priority);
                }
            }
            return 0;
        }

        int deleteAudio(std::string name) {
            this->load();
            for (auto it = business.begin(); it != business.end(); ++it) {
                if (it->customAudioName == name) {
                    char cmd[256];
                    CAudioCfgBusiness cfg;
                    cfg.load();

                    sprintf(cmd, "rm %s%s", cfg.getAudioFilePath().c_str(), name.c_str());
                    DS_TRACE("cmd: " << cmd);
                    system(cmd);
                    business.erase(it);
                    this->saveToJson();
                    return 1;
                }
            }
            return 0;
        }
    };
}
#endif
