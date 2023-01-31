#pragma once

#include "json.hpp"

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;
    template<typename T>
    class CResult;
    class CRebootResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CRebootResultData, null)

        template<typename Quest, typename Result,typename T>
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {}

    private:
        std::nullptr_t null;
    };

    class CRebootData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CRebootData, null)

        int do_req() {
            system("reboot");
            return 1;
        }

    private:
        std::nullptr_t null;
    };
}