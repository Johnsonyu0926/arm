#pragma once

#include <string>
#include "json.hpp"
#include "mqtt_reTemp.hpp"
#include "utils.h"
#include "Rs485.hpp"

using json = nlohmann::json;

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CPtzOperateResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CPtzOperateResultData, null)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            if (c.data.operateCmd.empty()) {
                r.resultId = 2;
                r.result = "failed to operateCmd empty";
                return 2;
            }
            CUtils utils;
            std::string str = utils.hex_to_string(c.data.operateCmd);
            if (Rs485::_uart_work(str.c_str(), str.length()) != 1) {
                r.resultId = 2;
                r.result = "failed to open ttyS";
                return 2;
            }
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        std::nullptr_t null{nullptr};
    };

    class CPtzOperateData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CPtzOperateData, operateCmd)

        std::string operateCmd;
    };
}