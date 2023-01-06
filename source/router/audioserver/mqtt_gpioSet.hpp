#pragma once

#include "json.hpp"

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CGpioSetResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CGpioSetResultData, v5, v12, v24)

        template<typename Quest, typename Result, typename T>
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {}

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
        int do_req() {}
    public:
        std::vector<CGpioSetData> portList;
    };
}