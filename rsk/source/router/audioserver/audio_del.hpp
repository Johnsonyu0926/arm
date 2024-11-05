// audio_del.hpp
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "add_custom_audio_file.hpp"
#include "add_column_custom_audio_file.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "getaudiolist.hpp"

namespace asns {

class CDeleteAudioResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CDeleteAudioResult, cmd, resultId, data, msg)

    void do_success() {
        cmd = "AudioDelete";
        resultId = 1;
        msg = "AudioDelete handle success";
        CAddCustomAudioFileBusiness audios;
        audios.load();
        CAudioCfgBusiness cfg;
        cfg.load();
        CUtils utils;
        data.reserve(audios.business.size());
        for (const auto& business : audios.business) {
            CGetAudioData v;
            v.storageType = 0;
            v.type = 32;
            v.fileName = business.customAudioName;
            v.size = utils.get_size(cfg.getAudioFilePath().c_str(), business.filename);
            v.audioId = business.customAudioID;
            data.push_back(std::move(v));
        }
    }

private:
    std::string cmd;
    int resultId;
    std::string msg;
    std::vector<CGetAudioData> data;
};

class CDeleteAudio {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CDeleteAudio, cmd, deleteName, storageType)

    [[nodiscard]] int do_del(const std::string& name, int type) const {
        CAddCustomAudioFileBusiness audios;
        CAddColumnCustomAudioFileBusiness business;
        audios.deleteAudio(name);
        business.deleteAudio(name);
        return 0;
    }

    [[nodiscard]] int do_req(std::shared_ptr<CSocket> pClient) {
        do_del(deleteName, storageType);
        CDeleteAudioResult res;
        res.do_success();
        nlohmann::json j = res;
        std::string s = j.dump();
        return pClient->Send(s.c_str(), s.length());
    }

private:
    std::string cmd;
    std::string deleteName;
    int storageType;
};

} // namespace asns

//By GST ARMV8 GCC13.2 audio_del.hpp