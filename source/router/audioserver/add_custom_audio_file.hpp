#ifndef __ADD_CUSTOM_AUDIO_FILE_H__
#define __ADD_CUSTOM_AUDIO_FILE_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include "utils.h"
#include "json.hpp"
#include "doorsbase.h"
#include "audiocfg.hpp"

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
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAddCustomAudioFileData, customAudioID, customAudioName, filePath, filePathType)

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

        int play(string prefix, string endTime) {
            if (strlen(filename) <= 0) {
                parseFile();
            }

            //cout << "custom audio file data : playing " << customAudioID << ",file" << customAudioName << endl;

            // background play. and monitor....
            //
            char cmd[256] = {0};
            sprintf(cmd, "madplay %s/%s", prefix.c_str(), getFileName());
            char background_cmd[256] = {0};
            sprintf(background_cmd, "%s&", cmd);
            system(background_cmd); // background play
            CUtils utils;

            CSTime tnow;

            tnow.GetCurTime();
            int h, m, s;
            sscanf(endTime.c_str(), "%d:%d:%d", &h, &m, &s);
            CSTime t1(tnow.m_nYear, tnow.m_nMon, tnow.m_nDay, h, m, s);

            while (1) {

                tnow.GetCurTime();

                int exist = utils.get_process_status(cmd);
                if (!exist) {
                    cout << "play finish of cmd:" << cmd << endl;
                    break;
                }
                if (tnow.m_time <= t1.m_time) {
                    // current time is match. waiting
                    cout << "playing:" << cmd << endl;
                    sleep(1);
                } else {
                    // out of time. stop if it is playing...
                    system("killall -9 madplay");
                    cout << "out of time. play stop! last cmd is:" << cmd << endl;
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
                    cout << "exist node , " << endl;
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
            return 0;
        }

        int download(CAddCustomAudioFileData node) {
            char cmd[1024];
            node.parseFile();
            sprintf(cmd, "/usr/bin/dodownload.sh \"%s\" \"%s/%s\"",
                    (char *) node.getFilePath(),
                    savePrefix.c_str(),
                    (char *) node.getFileName());
            DS_TRACE("NLOHMANN_DEFINE_TYPE_INTRUSIVE:" << cmd);
            system(cmd);
            return 0;
        }

        int load() {
            std::ifstream i(filePath);
            if (!i) {
                return 0;
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
            //cout << "load " << ADD_CUSTOM_AUDIO_FILE << "  success! " << endl;
            //cout << "count:" << business.size() << endl;
            return 0;
        }

        int play(int id, string endtime) {
            //cout << "business count:" << business.size() << endl;
            for (unsigned int i = 0; i < business.size(); i++) {
                CAddCustomAudioFileData data = business.at(i);
                if (data.getCustomAudioID() == id) {
                    data.play(savePrefix, endtime);
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
                    std::cout << cmd << std::endl;
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
