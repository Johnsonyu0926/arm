#pragma once

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
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {}

    private:
        std::nullptr_t null;
    };

    class CAudioStreamStartData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioStreamStartData, streamPath, roomId, priority, volume)

        int do_req() {
            CUtils utils;
            if (utils.get_process_status("madplay")) {
                return 5;
            }
            TestFileBusiness bus;
            std::cout << "volume:" << volume << std::endl;
            if (volume > 0) {
                CVolumeSet volumeSet;
                volumeSet.addj(volume);
                std::string streamUrl = streamPath + roomId;
                char buf[256] = {0};
                sprintf(buf, bus.getFfmpegCmd().c_str(), streamUrl.c_str());
                std::cout << "system:" << buf << std::endl;
                system(buf);

            } else {
                std::string streamUrl = streamPath + roomId;
                char buf[256] = {0};
                sprintf(buf, bus.getFfmpegCmd().c_str(), streamUrl.c_str());
                std::cout << "system:" << buf << std::endl;
                system(buf);
            }
            return 1;
        }

    private:
        std::string streamPath;
        std::string roomId;
        int priority;
        int volume{-1};
    };
}