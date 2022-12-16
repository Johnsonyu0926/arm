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
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {}

    public:
        std::nullptr_t null;
    };


    class CPtzOperateData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CPtzOperateData, operateCmd)

        int do_req() {
            rs::set_send_dir();
            rs::_uart_write(operateCmd.c_str(), operateCmd.length());
            return 1;
        }

    public:
        std::string operateCmd;
    };
}