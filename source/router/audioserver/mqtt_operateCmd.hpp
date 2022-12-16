#pragma once

#include "json.hpp"
#include "mqtt_reTemp.hpp"
#include "rs485Manage.hpp"

using json = nlohmann::json;

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    class CPtzOperateResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CPtzOperateResultData, null)

        template<typename Quest, typename Result>
        void do_success(const CReQuest<Quest, Result> &c) {}

    public:
        std::nullptr_t null;
    };


    class CPtzOperateData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CPtzOperateData, operateCmd)

        int do_req() {
            rs::set_send_dir();
            rs::_uart_write(operateCmd.c_str(), operateCmd.length());
            return 1;
        }

    public:
        std::string operateCmd;
    };
}