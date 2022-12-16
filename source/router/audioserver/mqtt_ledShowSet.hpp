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
    class CLedShowSetResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CLedShowSetResultData, ledShowRecordId)
        template<typename Quest, typename Result,typename T>
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            ledShowRecordId = c.data.ledShowRecordId;
        }

    public:
        long ledShowRecordId;
    };


    class CLedShowSetData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CLedShowSetData, ledCommand, showContent, ledShowRecordId)

        int do_req() {
            rs::set_send_dir();
            rs::_uart_write(ledCommand.c_str(), ledCommand.length());
            return 1;
        }

    public:
        std::string ledCommand;
        std::string showContent;
        long ledShowRecordId;
    };
}