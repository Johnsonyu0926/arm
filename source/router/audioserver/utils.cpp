#include "utils.h"
#include <sstream>
#include <iomanip>

namespace utils {

    std::string to_hex(const std::vector<unsigned char>& data) {
        std::ostringstream oss;
        for (auto byte : data) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        return oss.str();
    }

    std::vector<unsigned char> from_hex(const std::string& hex) {
        std::vector<unsigned char> data;
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
            data.push_back(byte);
        }
        return data;
    }

} // namespace utils