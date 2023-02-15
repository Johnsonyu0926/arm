#pragma once

#include "json.hpp"
#include "mqtt_reTemp.hpp"
#include "utils.h"

using json = nlohmann::json;

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CLedShowSetResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CLedShowSetResultData, ledShowRecordId)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            json js = c.data.ledCommand;
            std::string str = js.dump();
            ledShowRecordId = c.data.ledShowRecordId;
            CUtils utils;
            if(utils.uart_write(str) != 1){
                r.resultId = 2;
                r.result = "failed to open ttyS";
                return 2;
            }
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    public:
        long ledShowRecordId;
    };


    class CLedShowSetData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CLedShowSetData, ledCommand, showContent, ledShowRecordId)

    public:
        std::vector<std::string> ledCommand;
        std::string showContent;
        long ledShowRecordId;
    };
}