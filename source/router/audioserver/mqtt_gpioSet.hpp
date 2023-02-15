#pragma once

#include "json.hpp"
#include "utils.h"

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
            CUtils utils;
            switch (c.data.portList[0].val) {
                case 0:
                    utils.set_gpio_off();
                    break;
                case 1:
                    utils.set_gpio_on();
                    break;
                default:
                    r.resultId = 2;
                    r.result = "Protocol error";
                    return 2;
            }
            v5 = utils.get_gpio_state();
            v12 = 0;
            v24 = 0;
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    public:
        int v5;
        int v12;
        int v24;
    };

    class CGpioSetData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CGpioSetData, port, val)

    public:
        int port;
        int val;
    };

    class CGpioSet {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CGpioSet, portList)

    public:
        std::vector<CGpioSetData> portList;
    };
}