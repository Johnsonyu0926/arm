#pragma once

#include "json.hpp"

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;
    template<typename T>
    class CResult;
    class CAudioStopResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioStopResultData, null)

        template<typename Quest, typename Result,typename T>
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {}

    private:
        std::nullptr_t null;
    };

    class CAudioStopData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioStopData, null)

        int do_req() {
            system("killall -9 madplay");
            system("killall -9 ffmpeg");
            Singleton::getInstance().setStatus(0);
            system("killall -9 aplay");
            return 1;
        }

    private:
        std::nullptr_t null;
    };
}