#pragma once

#include "json.hpp"
#include "mqtt_reTemp.hpp"
#include "rs485Manage.hpp"

using json = nlohmann::json;

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CPtzOperateResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CPtzOperateResultData, null)

        template<typename Quest, typename Result,typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            if(rs::_uart_work(c.data.operateCmd.c_str(), c.data.operateCmd.length()) != 1){
                r.resultId = 2;
                r.result = "failed to open ttyS";
                return 2;
            }
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    public:
        std::nullptr_t null;
    };


    class CPtzOperateData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CPtzOperateData, operateCmd)

    public:
        std::string operateCmd;
    };
}