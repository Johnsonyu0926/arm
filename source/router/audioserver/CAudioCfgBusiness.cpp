#include "CAudioCfgBusiness.h"
#include "CryptoUtils.hpp"
#include "utils.h"

namespace asns {

    void CAudioCfgBusiness::load() {
        // Load the configuration from JSON file
        // ...

        // Decrypt the server settings
        const std::string encryptionKey = "0123456789abcdef0123456789abcdef"; // 32-byte key for AES-256
        std::vector<unsigned char> encryptedServerAddress = utils::from_hex(business[0].server);
        std::vector<unsigned char> encryptedDeviceCode = utils::from_hex(business[0].deviceID);
        std::vector<unsigned char> encryptedPassword = utils::from_hex(business[0].password);

        business[0].server = CryptoUtils::decrypt(encryptedServerAddress, encryptionKey);
        business[0].deviceID = CryptoUtils::decrypt(encryptedDeviceCode, encryptionKey);
        business[0].password = CryptoUtils::decrypt(encryptedPassword, encryptionKey);
    }

    void CAudioCfgBusiness::saveToJson() {
        // Save the configuration to JSON file
        // ...
    }

} // namespace asns