#pragma once

#include "json.hpp"

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    class CPlanPlayResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CPlanPlayResultData, null)

        template<typename Quest, typename Result>
        void do_success(const CReQuest <Quest, Result> &c) {}

    private:
        std::nullptr_t null;
    };

    class CPlanPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CPlanPlayData, fileName, content, audioSubTaskId, playDuration,
                                                    playStatus, playType, storageType, timeType)

        int do_req() {

            return 1;
        }

    private:
        std::string fileName;
        std::string content;
        int audioSubTaskId;
        int playDuration;
        int playStatus;
        int playType;
        int storageType;
        int timeType;
    };
}