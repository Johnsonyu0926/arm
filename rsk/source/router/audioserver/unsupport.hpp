// unsupport.hpp
#pragma once

#include <string>
#include <string_view>
#include "json.hpp"
#include "CSocket.h"
#include "log.h"

namespace asns {

class CUnsupport {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUnsupport, cmd, resultId, msg)

    void do_success(std::string_view reqcmd) {
        cmd = reqcmd;
        resultId = 2;
        msg = "unsupport operation";
    }

private:
    std::string cmd;
    int resultId{};
    std::string msg;
};

class CUnsupportBusiness {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUnsupportBusiness, cmd)

    void setCmd(std::string_view unsupport_cmd) {
        cmd = unsupport_cmd;
    }

    [[nodiscard]] int do_req(CSocket* pClient) const {
        LOG(INFO) << "unsupport cmd: " << cmd;
        CUnsupport res;
        res.do_success(cmd);

        nlohmann::json j = res;
        std::string s = j.dump();
        pClient->Send(s.data(), s.length());

        return 1;
    }

private:
    std::string cmd;
};

} // namespace asns

//BY GST ARMV8 GCC 13.2