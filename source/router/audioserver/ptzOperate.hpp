#pragma once

#include "json.hpp"
#include "rs485.c"

namespace asns {

    class CPtzOperate {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CPtzOperate, cmd, operateCmd)

        int do_req() {
            set_send_dir();
            _uart_write(operateCmd.c_str(), operateCmd.length());
        }

    private:
        std::string cmd;
        std::string operateCmd;
    };
}