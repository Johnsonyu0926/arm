#pragma once

#include "json.hpp"
#include "utils.h"

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CAudioStopResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioStopResultData, null)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            CUtils utils;
            utils.audio_stop();
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        std::nullptr_t null;
    };

    class CAudioStopData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioStopData, null)

    private:
        std::nullptr_t null;
    };
}