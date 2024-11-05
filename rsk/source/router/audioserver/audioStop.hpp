// audiostop.hpp
#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "utils.h"

namespace asns {

class CAudioStopResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioStopResult, cmd, resultId, msg)

    void do_success() {
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

    [[nodiscard]] int do_req(std::shared_ptr<CSocket> pClient) const {
        AudioPlayUtil::audio_stop();
        CAudioStopResult audioStopResult;
        audioStopResult.do_success();
        return send_response(pClient, audioStopResult);
    }

private:
    std::string cmd;

    [[nodiscard]] static int send_response(const std::shared_ptr<CSocket>& pClient, const CAudioStopResult& audioStopResult) {
        nlohmann::json js = audioStopResult;
        std::string str = js.dump();
        return pClient->Send(str.c_str(), str.length());
    }
};

} // namespace asns

//By GST ARMV8 GCC13.2 audiostop.hpp