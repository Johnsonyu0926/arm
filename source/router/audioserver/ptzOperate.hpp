#pragma once

#include "json.hpp"
#include "rs485.c"

namespace asns {

    class CPtzOperate {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CPtzOperate, cmd, operateCmd)

        int do_req() {
            _uart_work(operateCmd.c_str(),operateCmd.length());
        }

    private:
        std::string cmd;
        std::string operateCmd;
    };
}