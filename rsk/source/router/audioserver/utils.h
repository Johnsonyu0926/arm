#pragma once

#include <cstdio>
#include <cstdarg>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/statfs.h>
#include <string>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include <map>
#include <algorithm>
#include <net/if.h>
#include <arpa/inet.h>
#include <span>
#include "public.hpp"
#include "audiocfg.hpp"
#include "doorsbase.h"
#include "easylogging++.h"

class Singleton {
public:
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
    [[nodiscard]] std::string_view getConnIp() const noexcept { return m_connIp; }
private:
    Singleton() = default;
    std::string m_connIp;
};

class CUtils {
private:
    std::array<char, 1024> m_lan{};
    const std::map<std::string_view, std::string_view> m_month{
        {"Jan", "01"}, {"Feb", "02"}, {"Mar", "03"}, {"Apr", "04"},
        {"May", "05"}, {"Jun", "06"}, {"Jul", "07"}, {"Aug", "08"},
        {"Sep", "09"}, {"Oct", "10"}, {"Nov", "11"}, {"Dec", "12"}
    };

public:
    [[nodiscard]] int get_file_size(const std::string& path) const {
        struct stat st;
        if (stat(path.c_str(), &st) != 0) {
            LOG(ERROR) << "Failed to get file size for: " << path;
            return -1;
        }
        return st.st_size;
    }

    void get_dir_file_names(const std::string& path, std::vector<std::string>& files) const {
        if (auto pDir = opendir(path.c_str()); pDir) {
            while (auto ptr = readdir(pDir)) {
                if (ptr->d_name != std::string_view{"."} && ptr->d_name != std::string_view{".."}) {
                    files.emplace_back(ptr->d_name);
                }
            }
            closedir(pDir);
        }
    }

    [[nodiscard]] bool find_dir_file_exists(const std::string& path, const std::string& name) const {
        std::vector<std::string> files_name;
        get_dir_file_names(path, files_name);
        return std::find(files_name.begin(), files_name.end(), name) != files_name.end();
    }

    [[nodiscard]] int isIpWL() const {
        auto connIp = Singleton::getInstance().getConnIp();
        if (connIp.empty()) return 1;
        connIp = connIp.substr(0, connIp.find_last_of('.'));

        auto lanIp = is_ros_platform() ? get_by_cmd_res("cm get_val VLAN1 ipaddress | tail -1") : get_by_cmd_res("uci get network.lan.ipaddr");
        auto wanIp = is_ros_platform() ? get_by_cmd_res("cm get_val WAN1 ipaddress | tail -1") : get_by_cmd_res("uci get network.wan.ipaddr");

        return (connIp == lanIp.substr(0, lanIp.find_last_of('.')) || connIp == wanIp.substr(0, wanIp.find_last_of('.'))) ? 0 : 1;
    }

    [[nodiscard]] bool is_ros_platform() const {
        std::ifstream i("/bin/cm");
        return i.is_open();
    }

    [[nodiscard]] std::string get_by_cmd_res(const std::string& cmd) const {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            LOG(ERROR) << "Failed to run command: " << cmd;
            return "";
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    [[nodiscard]] std::string get_lan_ip() const {
        return is_ros_platform() ? get_by_cmd_res("cm get_val VLAN1 ipaddress | tail -1") : get_by_cmd_res("uci get network.lan.ipaddr");
    }

    [[nodiscard]] std::string get_lan_netmask() const {
        return is_ros_platform() ? get_by_cmd_res("cm get_val VLAN1 ipmask | tail -1") : get_by_cmd_res("uci get network.lan.netmask");
    }

    [[nodiscard]] std::string get_lan_gateway() const {
        return is_ros_platform() ? get_by_cmd_res("cm get_val VLAN1 gateway | tail -1") : get_by_cmd_res("uci get network.lan.gateway");
    }

    [[nodiscard]] std::string get_wan_ip() const {
        return is_ros_platform() ? get_by_cmd_res("cm get_val WAN1 ipaddress | tail -1") : get_by_cmd_res("uci get network.wan.ipaddr");
    }

    [[nodiscard]] std::string get_wan_netmask() const {
        return is_ros_platform() ? get_by_cmd_res("cm get_val WAN1 ipmask | tail -1") : get_by_cmd_res("uci get network.wan.netmask");
    }

    [[nodiscard]] std::string get_wan_gateway() const {
        return is_ros_platform() ? get_by_cmd_res("cm get_val WAN1 gateway | tail -1") : get_by_cmd_res("uci get network.wan.gateway");
    }

    [[nodiscard]] std::string get_upload_result(const std::string& url, const std::string& path, const std::string& name) const {
        std::string cmd = "curl --location --request GET " + url + " -f --output " + path + name + " 2>&1";
        std::string res = get_by_cmd_res(cmd);
        LOG(INFO) << "cmd:" << cmd << " res:" << res;

        if (res.empty() || res.find("error") != std::string::npos || res.find("Connection refused") != std::string::npos ||
            res.find("No route to host") != std::string::npos || res.find("Host is unreachable") != std::string::npos ||
            res.find("Failed to connect") != std::string::npos || res.find("Couldn't connect to server") != std::string::npos) {
            return "file upload fail";
        }
        return get_file_size(path + name) > 0 ? "success" : "file upload fail";
    }

    template<typename F, typename... Args>
    void async_wait(int count, int after, int interval, F&& f, Args&&... args) const {
        std::thread([after, f = std::forward<F>(f), count, interval, ...args = std::forward<Args>(args)]() mutable {
            std::this_thread::sleep_for(std::chrono::seconds(after));
            if (count == 0) {
                while (true) {
                    std::invoke(f, args...);
                    std::this_thread::sleep_for(std::chrono::seconds(interval));
                }
            } else {
                for (int i = 0; i < count; ++i) {
                    std::invoke(f, args...);
                    std::this_thread::sleep_for(std::chrono::seconds(interval));
                }
            }
        }).detach();
    }

    [[nodiscard]] std::vector<std::string> string_split(std::string_view str_v, std::string_view del_ims = " ") const {
        std::vector<std::string> output;
        size_t first = 0;
        while (first < str_v.size()) {
            const auto second = str_v.find_first_of(del_ims, first);
            if (first != second) {
                output.emplace_back(str_v.substr(first, second - first));
            }
            if (second == std::string_view::npos) {
                break;
            }
            first = second + 1;
        }
        return output;
    }

    [[nodiscard]] int statistical_character_count(std::string_view str) const {
        int LowerCase = 0, UpperCase = 0, space = 0, digit = 0, character = 0, chinese = 0;
        for (const char c : str) {
            if (std::islower(c)) {
                ++LowerCase;
            } else if (std::isupper(c)) {
                ++UpperCase;
            } else if (std::isdigit(c)) {
                ++digit;
            } else if (static_cast<unsigned char>(c) >= 0x80) {
                ++chinese;
            } else if (std::isspace(c)) {
                ++space;
            } else {
                ++character;
            }
        }
        return UpperCase + LowerCase + digit + character + (chinese / 3) + space;
    }

    int change_password(std::string_view password) const {
        if (is_ros_platform()) {
            std::string cmd = "webs -P " + std::string(password);
            system(cmd.c_str());
            cmd = "cm set_val sys serverpassword " + std::string(password);
            system(cmd.c_str());
        }
        return 1;
    }

    void bit_rate_32_to_48(const std::string& path) const {
        // Implementation for bit rate conversion
    }

    void udp_multicast_send(std::string_view ip, uint16_t port, std::string_view msg) const {
        sockaddr_in server{};
        ip_mreqn group{};

        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        server.sin_family = AF_INET;
        inet_pton(AF_INET, get_lan_ip().c_str(), &server.sin_addr.s_addr);
        server.sin_port = htons(port);

        bind(sock, reinterpret_cast<struct sockaddr*>(&server), sizeof(server));

        inet_pton(AF_INET, ip.data(), &group.imr_multiaddr.s_addr);
        inet_pton(AF_INET, "0.0.0.0", &group.imr_address);

        group.imr_ifindex = if_nametoindex(isIpWL() == 0 ? "br0" : "WAN1");
        setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &group.imr_multiaddr.s_addr, sizeof(group.imr_multiaddr.s_addr));

        sockaddr_in client{};
        client.sin_family = AF_INET;
        inet_pton(AF_INET, ip.data(), &client.sin_addr.s_addr);
        client.sin_port = htons(port);

        sendto(sock, msg.data(), msg.length(), 0, reinterpret_cast<struct sockaddr*>(&client), sizeof(client));
        LOG_EVERY_N(10, INFO) << "udp_multicast_send: " << ip << " " << port << " msg: " << msg;
        close(sock);
    }

    static void cmd_system(const std::string& cmd) {
        system(cmd.c_str());
    }
};

//BY GST ARMV8 GCC 13.2