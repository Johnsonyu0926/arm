#pragma once

#include <string>
#include "volume.hpp"
#include "json.hpp"
#include <iostream>
#include <thread>
#include "testFile.hpp"

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CAudioStreamStartResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioStreamStartResultData, null)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            CUtils utils;
            if (CUtils::get_process_status("madplay") || CUtils::get_process_status("ffplay") || PlayStatus::getInstance().getPlayState()) {
                r.resultId = 2;
                r.result = "Already played";
                return 2;
            }
            TestFileBusiness bus;
            LOG(INFO) << "volume: " << c.data.volume;
            std::string streamUrl = c.data.streamPath + c.data.roomId;
            std::string cmd = fmt::format(bus.getFfmpegCmd(), streamUrl);
            LOG(INFO) << "system: " << cmd;
            system(cmd.c_str());

            if (c.data.volume > 0) {
                CVolumeSet volumeSet;
                volumeSet.addj(c.data.volume);
            }

            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        std::nullptr_t null{nullptr};
    };

    class CAudioStreamStartData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioStreamStartData, streamPath, roomId, priority, volume)

        std::string streamPath;
        std::string roomId;
        int priority{0};
        int volume{-1};
    };
}