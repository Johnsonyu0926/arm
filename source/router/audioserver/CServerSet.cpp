#include "CServerSet.h"
#include "CAudioCfgBusiness.h"

namespace asns {

    int CServerSet::do_req(CSocket *pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();

        // Encrypt the server settings
        std::vector<unsigned char> encryptedServerAddress = CryptoUtils::encrypt(serverAddress, encryptionKey);
        std::vector<unsigned char> encryptedServerPort = CryptoUtils::encrypt(serverPort, encryptionKey);
        std::vector<unsigned char> encryptedDeviceCode = CryptoUtils::encrypt(deviceCode, encryptionKey);
        std::vector<unsigned char> encryptedPassword = CryptoUtils::encrypt(password, encryptionKey);

        // Convert encrypted data to hex string for storage
        cfg.business[0].server = utils::to_hex(encryptedServerAddress);
        cfg.business[0].port = std::stoi(serverPort); // Port is typically not encrypted
        cfg.business[0].deviceID = utils::to_hex(encryptedDeviceCode);
        cfg.business[0].password = utils::to_hex(encryptedPassword);
        cfg.saveToJson();

        CUtils utils;
        if (utils.is_ros_platform()) {
            std::string buf;
            buf = "cm set_val sys server " + serverAddress;
            system(buf.c_str());
            buf = "cm set_val sys port " + serverPort;
            system(buf.c_str());
            buf = "cm set_val sys password " + password;
            system(buf.c_str());
            buf = "cm set_val sys deviceid " + deviceCode;
            system(buf.c_str());
        }
        spdlog::info("serverAddress: {}, serverPort: {}, deviceCode: {}, password: {}", serverAddress, serverPort, deviceCode, password);
        CServerSetResult res;
        res.do_success();
        json j = res;
        std::string s = j.dump();
        pClient->Send(s.c_str(), s.length());
        utils.cmd_system("asctrl.sh restart");
        return 1;
    }

} // namespace asns