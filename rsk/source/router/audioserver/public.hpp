// public.hpp
#pragma once

#include <string>
#include <string_view>
#include <array>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <filesystem>
#include <span>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace asns {

class Utils {
public:
    [[nodiscard]] static std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    [[nodiscard]] static std::string hex_to_string(std::string_view input) {
        static constexpr std::string_view lut = "0123456789ABCDEF";
        if (input.length() % 2 != 0) throw std::invalid_argument("odd length");

        std::string output;
        output.reserve(input.length() / 2);
        for (size_t i = 0; i < input.length(); i += 2) {
            char a = input[i];
            auto p = std::lower_bound(lut.begin(), lut.end(), a);
            if (*p != a) throw std::invalid_argument("not a hex digit");

            char b = input[i + 1];
            auto q = std::lower_bound(lut.begin(), lut.end(), b);
            if (*q != b) throw std::invalid_argument("not a hex digit");

            output.push_back(((p - lut.begin()) << 4) | (q - lut.begin()));
        }
        return output;
    }

    static void network_set(std::string_view gateway, std::string_view ipAddress, std::string_view netMask) {
        std::ostringstream oss;
        oss << "ifconfig eth0 " << ipAddress << " netmask " << netMask;
        exec(oss.str().c_str());
        oss.str("");
        oss << "route add default gw " << gateway;
        exec(oss.str().c_str());
    }
};

} // namespace asns

//BY GST ARMV8 GCC 13.2