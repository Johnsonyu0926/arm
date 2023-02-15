#pragma once

#include "json.hpp"
#include "utils.h"

namespace asns {

    class CPtzOperate {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CPtzOperate, cmd, operateCmd)

        int do_req() {
            CUtils utils;
            utils.uart_write(operateCmd);
        }

    private:
        std::string cmd;
        std::string operateCmd;
    };
}