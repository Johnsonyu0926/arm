#pragma once
#include "json.hpp"
#include "add_custom_audio_file.hpp"
#include "add_column_custom_audio_file.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "getaudiolist.hpp"
//{"cmd":"AudioDelete","deleteName":"23.mp3","storageType":1}
namespace asns
{

    class CDeleteAudioResult
    {
    private:
        string cmd;
        int resultId;
        string msg;
        vector<CGetAudioData> data;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CDeleteAudioResult, cmd, resultId, data, msg)

    public:
        int do_success()
        {
            cmd = "AudioDelete";
            resultId = 1;
            msg = "AudioDelete handle success";

            CAddCustomAudioFileBusiness audios;
            audios.load();
            for (int i = 0; i < audios.business.size(); i++)
            {
                CGetAudioData v;
                v.storageType = 0;
                v.type = 32;
                audios.business[i].parseFile();
                v.fileName = audios.business[i].customAudioName;
                CAudioCfgBusiness cfg;
                cfg.load();
                CUtils utils;
                v.size = utils.get_size(cfg.getAudioFilePath().c_str(), audios.business[i].filename);
                v.audioId = audios.business[i].customAudioID;
                data.push_back(v);
            }
        }
    };
    class CDeleteAudio
    {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CDeleteAudio, cmd, deleteName, storageType)

        int do_del(std::string &name, int type)
        {

            CAddCustomAudioFileBusiness audios;
            audios.load();
            for (auto it = audios.business.begin(); it != audios.business.end(); ++it)
            {
                if (it->customAudioName == name)
                {
                    char cmd[256];
                    CAudioCfgBusiness cfg;
                    cfg.load();

                    sprintf(cmd, "rm %s%s", cfg.getAudioFilePath().c_str(), name.c_str());
                    std::cout << cmd << std::endl;
                    system(cmd);
                    audios.business.erase(it);
                    audios.saveToJson();
                    return 1;
                }
            }
            CAddColumnCustomAudioFileBusiness bussines;
            bussines.Columnload();
            for (auto it = bussines.business.begin(); it != bussines.business.end(); ++it)
            {
                if (it->getName() == name)
                {
                    char cmd[256];
                    CAudioCfgBusiness cfg;
                    cfg.load();

                    sprintf(cmd, "rm %s%s", cfg.getAudioFilePath().c_str(), name.c_str());
                    system(cmd);
                    bussines.business.erase(it);
                    bussines.saveJson();
                    return 1;
                }
            }
            return 0;
        }
        int do_req(CSocket *pClient)
        {
            do_del(deleteName, storageType);
            CDeleteAudioResult res;
            res.do_success();
            json j = res;
            std::string s = j.dump();
            pClient->Send(s.c_str(), s.length());
            return 1;
        }

    private:
        std::string cmd;
        std::string deleteName;
        int storageType;
    };
}