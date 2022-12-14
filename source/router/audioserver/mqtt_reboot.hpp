#pragma once

#include "json.hpp"

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    class CRebootResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CRebootResultData, null)

        template<typename Quest, typename Result>
        void do_success(const CReQuest<Quest, Result> &c) {}

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