#ifndef __CUNSUPPORT_H__
#define __CUNSUPPORT_H__

#include <iostream>
#include "json.hpp"
#include "spdlog/spdlog.h"

namespace asns {

    class CUnsupport {
    private:
        std::string cmd;
        int resultId{0};
        std::string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUnsupport, cmd, resultId, msg)

        void do_success(const std::string& reqcmd) {
            cmd = reqcmd;
            resultId = 2;
            msg = "unsupport operation";
        }
    };

    class CUnsupportBusiness {
    private:
        std::string cmd;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUnsupportBusiness, cmd)

        void setCmd(const std::string& unsupport_cmd) { cmd = unsupport_cmd; }

        int do_req(CSocket *pClient) {
            spdlog::info("unsupport cmd: {}", cmd);
            CUnsupport res;
            res.do_success(cmd);
            json j = res;
            std::string s = j.dump();
            pClient->Send(s.c_str(), s.length());
            return 1;
        }
    };

} // namespace asns

#endif // __CUNSUPPORT_H__