#ifndef __CSERVERSET_H__
#define __CSERVERSET_H__

#include <iostream>
#include "json.hpp"
#include "utils.h"
#include "spdlog/spdlog.h"
#include "CryptoUtils.hpp"

namespace asns {

    class CServerSetResult {
    private:
        std::string cmd;
        int resultId{0};
        std::string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CServerSetResult, cmd, resultId, msg)

        void do_success() {
            cmd = "ServerSet";
            resultId = 1;
            msg = "ServerSet handle success";
        }
    };

    class CServerSet {
    private:
        std::string cmd;
        std::string serverAddress;
        std::string serverPort;
        std::string deviceCode;
        std::string password;
        const std::string encryptionKey = "0123456789abcdef0123456789abcdef"; // 32-byte key for AES-256

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CServerSet, cmd, serverAddress, serverPort, deviceCode, password)

        int do_req(CSocket *pClient);
    };

} // namespace asns

#endif // __CSERVERSET_H__