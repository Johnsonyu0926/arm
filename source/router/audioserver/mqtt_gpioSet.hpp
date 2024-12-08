#pragma once

#include <vector>
#include "json.hpp"
#include "utils.h"
#include "Relay.hpp"

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CGpioSetResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CGpioSetResultData, v5, v12, v24)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            switch (c.data.portList[0].val) {
                case 0:
                    Relay::getInstance().set_gpio_off();
                    break;
                case 1:
                    Relay::getInstance().set_gpio_on();
                    break;
                default:
                    r.resultId = 2;
                    r.result = "Protocol error";
                    return 2;
            }
            v5 = Relay::getInstance().getGpioStatus();
            v12 = 0;
            v24 = 0;
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        int v5{0};
        int v12{0};
        int v24{0};
    };

    class CGpioSetData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CGpioSetData, port, val)

        int port{0};
        int val{0};
    };

    class CGpioSet {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CGpioSet, portList)

        std::vector<CGpioSetData> portList;
    };
}