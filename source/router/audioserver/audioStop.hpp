#pragma once

#include "json.hpp"

namespace asns {
    class CAudioStopResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioStopResult, cmd, resultId, msg);

        int do_success() {
            cmd = "AudioStop";
            resultId = 1;
            msg = "stop play success";
        }

    private:
        std::string cmd;
        int resultId;
        std::string msg;
    };

    class CAudioStop {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioStop, cmd)

        int do_req(CSocket *pClient) {
            system("killall -9 madplay");
            CAudioStopResult audioStopResult;
            audioStopResult.do_success();
            json js = audioStopResult;
            std::string str = js.dump();
            pClient->Send(str.c_str(), str.length());
        }

    private:
        std::string cmd;

    };
}