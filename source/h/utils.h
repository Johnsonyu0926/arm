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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include <net/if.h>
#include <map>
#include <sstream>
#include <algorithm>
#include "public.hpp"
#include "audiocfg.hpp"
#include "doorsbase.h"
#include "easylogging++.h"

class Singleton {
public:
    ~Singleton() = default;
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
    static Singleton &getInstance() {
        static Singleton instance;
        return instance;
    }
    std::string getConnIp() const {
        return m_connIp;
    }
private:
    Singleton() = default;
public:
    std::string m_connIp;
};

class CUtils {
private:
    char m_lan[1024]{};
    std::map<std::string, std::string> m_month = {{"Jan", "01"}, {"Feb", "02"}, {"Mar", "03"}, {"Apr", "04"},
                                                  {"May", "05"}, {"Jun", "06"}, {"Jul", "07"}, {"Aug", "08"},
                                                  {"Sep", "09"}, {"Oct", "10"}, {"Nov", "11"}, {"Dec", "12"}};
public:
    int get_file_size(const std::string &path) {
        int fd = open(path.c_str(), O_RDWR);
        if (fd < 0) {
            LOG(ERROR) << "open fail !" << path;
            return -1;
        }
        struct stat st;
        fstat(fd, &st);
        close(fd);
        return st.st_size;
    }

    void get_dir_file_names(const std::string &path, std::vector<std::string> &files) {
        DIR *pDir;
        dirent *ptr;
        if (!(pDir = opendir(path.c_str()))) return;
        while ((ptr = readdir(pDir)) != nullptr) {
            if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
                files.emplace_back(ptr->d_name);
            }
        }
        closedir(pDir);
    }

    bool find_dir_file_exists(const std::string &path, const std::string &name) {
        std::vector<std::string> files_name;
        get_dir_file_names(path, files_name);
        for (const auto &iter : files_name) {
            LOG(DEBUG) << iter << " ";
            if (name == iter) {
                return true;
            }
        }
        return false;
    }

    bool str_is_all_num(const std::string &str) {
        return std::all_of(str.begin(), str.end(), ::isdigit);
    }

    static unsigned long long get_available_Disk(const std::string &path) {
        struct statfs diskInfo;
        statfs(path.c_str(), &diskInfo);
        unsigned long long block_size = diskInfo.f_bsize; // 每个block里包含的字节数
        unsigned long long available_disk = diskInfo.f_bavail * block_size; // 可用空间大小
        return (available_disk >> 10); // 返回可用大小 kb
    }

    static int get_process_status(const char *cmd) {
        FILE *fp;
        char buf[256] = {0};
        char tmp[1024] = {0};
        snprintf(buf, sizeof(buf), "ps | grep \"%s\" | grep -v grep", cmd);
        int exist = 0;
        fp = popen(buf, "r");
        if (fp) {
            int nread = fread(tmp, 1, sizeof(tmp), fp);
            if (nread > 0) {
                exist = 1;
            }
            pclose(fp);
        }
        return exist;
    }

    int isIpWL() {
        std::string lanIp;
        std::string wanIp;
        if (is_ros_platform()) {
            lanIp = get_by_cmd_res("cm get_val VLAN1 ipaddress | tail -1");
            wanIp = get_by_cmd_res("cm get_val WAN1 ipaddress | tail -1");
        } else {
            lanIp = get_by_cmd_res("uci get network.lan.ipaddr");
            wanIp = get_by_cmd_res("uci get network.wan.ipaddr");
        }
        std::string connIp = Singleton::getInstance().getConnIp();
        if (connIp.empty()) {
            return 1;
        }
        connIp = connIp.substr(0, connIp.find_last_of('.'));
        if (connIp.compare(lanIp.substr(0, lanIp.find_last_of('.'))) == 0) {
            return 0;
        } else if (connIp.compare(wanIp.substr(0, wanIp.find_last_of('.'))) == 0) {
            return 1;
        } else {
            return 1;
        }
    }

    int is_ros_platform() {
        std::ifstream i("/bin/cm");
        if (!i.is_open()) {
            return 0;
        }
        i.close();
        return 1;
    }

    char *get_ros_addr() {
        char cmd[64] = {0};
        if (isIpWL() == 0) {
            strcpy(cmd, "cm get_val VLAN1 ipaddress | tail -1");
        } else {
            strcpy(cmd, "cm get_val WAN1 ipaddress|tail -1");
        }
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_ros_gateway() {
        char cmd[64] = {0};
        if (isIpWL() == 0) {
            strcpy(cmd, "cm get_val VLAN1 gateway | tail -1");
        } else {
            strcpy(cmd, "cm get_val WAN1 gateway|tail -1");
        }
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_ros_netmask() {
        char cmd[64] = {0};
        if (isIpWL() == 0) {
            strcpy(cmd, "cm get_val VLAN1 ipmask | tail -1");
        } else {
            strcpy(cmd, "cm get_val WAN1 ipmask|tail -1");
        }
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_gateway() {
        if (is_ros_platform()) {
            get_ros_gateway();
        } else {
            char cmd[64] = {0};
            if (isIpWL() == 0) {
                strcpy(cmd, "uci get network.lan.gateway");
            } else {
                strcpy(cmd, "uci get network.wan.gateway");
            }
            get_addr_by_cmd(cmd);
        }
        return m_lan;
    }

    char *get_netmask() {
        if (is_ros_platform()) {
            get_ros_netmask();
        } else {
            char cmd[64] = {};
            if (isIpWL() == 0) {
                strcpy(cmd, "uci get network.lan.netmask");
            } else {
                strcpy(cmd, "uci get network.wan.netmask");
            }
            get_addr_by_cmd(cmd);
        }
        return m_lan;
    }

    char *get_addr() {
        if (is_ros_platform()) {
            get_ros_addr();
        } else {
            char cmd[64] = {0};
            if (isIpWL() == 0) {
                strcpy(cmd, "uci get network.lan.ipaddr");
            } else {
                strcpy(cmd, "uci get network.wan.ipaddr");
            }
            get_addr_by_cmd(cmd);
        }
        return m_lan;
    }

    std::string get_lan_ip() {
        if (is_ros_platform()) {
            return get_by_cmd_res("cm get_val VLAN1 ipaddress | tail -1");
        } else {
            return get_by_cmd_res("uci get network.lan.ipaddr");
        }
    }

    std::string get_lan_netmask() {
        if (is_ros_platform()) {
            return get_by_cmd_res("cm get_val VLAN1 ipmask | tail -1");
        } else {
            return get_by_cmd_res("uci get network.lan.netmask");
        }
    }

    std::string get_lan_gateway() {
        if (is_ros_platform()) {
            return get_by_cmd_res("cm get_val VLAN1 gateway | tail -1");
        } else {
            return get_by_cmd_res("uci get network.lan.gateway");
        }
    }

    std::string get_wan_ip() {
        if (is_ros_platform()) {
            return get_by_cmd_res("cm get_val WAN1 ipaddress | tail -1");
        } else {
            return get_by_cmd_res("uci get network.wan.ipaddr");
        }
    }

    std::string get_wan_netmask() {
        if (is_ros_platform()) {
            return get_by_cmd_res("cm get_val WAN1 ipmask | tail -1");
        } else {
            return get_by_cmd_res("uci get network.wan.netmask");
        }
    }

    std::string get_wan_gateway() {
        if (is_ros_platform()) {
            return get_by_cmd_res("cm get_val WAN1 gateway | tail -1");
        } else {
            return get_by_cmd_res("uci get network.wan.gateway");
        }
    }

    std::string get_doupload_result(const std::string &url, const std::string &imei) {
        char cmd[1024] = {0};
        sprintf(cmd,
                "curl --location --request POST '%s' \\\n"
                "--form 'FormDataUploadFile=@\"/tmp/record.mp3\"' \\\n"
                "--form 'imei=\"%s\"'", url.c_str(), imei.c_str());
        LOG(INFO) << "cmd:" << cmd;
        return get_by_cmd_res(cmd);
    }

    std::string get_by_cmd_res(const char *cmd) {
        FILE *fp = nullptr;
        char buf[1024] = {0};
        std::string res;
        if ((fp = popen(cmd, "r")) != nullptr) {
            while (fgets(buf, sizeof(buf), fp) != nullptr) {
                res += buf;
            }
            pclose(fp);
            fp = nullptr;
        }
        return res;
    }

    std::string get_upload_result(const std::string &url, const std::string &path, const std::string &name) {
        char cmd[1024] = {0};
        sprintf(cmd, "curl --location --request GET %s -f --output %s%s 2>&1", url.c_str(), path.c_str(), name.c_str());
        LOG(INFO) << "cmd:" << cmd;
        std::string res = get_by_cmd_res(cmd);
        LOG(INFO) << "res:" << res;
        if (res.empty()) {
            return "file upload fail";
        } else if (res.find("error") != std::string::npos) {
            return "Connection unreachable";
        } else if (res.find("Connection refused") != std::string::npos) {
            return "Connection refused";
        } else if (res.find("No route to host") != std::string::npos) {
            return "No route to host";
        } else if (res.find("Host is unreachable") != std::string::npos) {
            return "Host is unreachable";
        } else if (res.find("Failed to connect") != std::string::npos) {
            return "Failed to connect";
        } else if (res.find("Couldn't connect to server") != std::string::npos) {
            return "Couldn't connect to server";
        } else if (get_file_size(path + name) > 0) {
            return "success";
        } else {
            return "file upload fail";
        }
    }

    std::string get_res_by_cmd(const std::string &cmd) {
        char buf[1024] = {0};
        FILE *fp = nullptr;
        fp = popen(cmd.c_str(), "r");
        if (fp) {
            int nread = fread(buf, 1, sizeof(buf), fp);
            if (nread < 0) {
                LOG(WARNING) << "get_res_by_cmd fread error";
                pclose(fp);
                return buf;
            }
            char *p = strchr(buf, '\n');
            if (p) {
                *p = '\0';
            }
            p = strchr(buf, '\r');
            if (p) {
                *p = '\0';
            }
            pclose(fp);
        }
        return buf;
    }

    char *get_addr_by_cmd(const char *cmd) {
        memset(m_lan, 0, sizeof m_lan);
        FILE *fp;
        fp = popen(cmd, "r");
        if (fp) {
            int nread = fread(m_lan, 1, sizeof(m_lan), fp);
            if (nread < 0) {
                LOG(WARNING) << "fread err";
                pclose(fp);
                return m_lan;
            }
            if (strchr(m_lan, '.')) {
                char *p = strchr(m_lan, '\n');
                if (p) {
                    *p = '\0';
                }
                p = strchr(m_lan, '\r');
                if (p) {
                    *p = '\0';
                }
            } else {
                memset(m_lan, 0, sizeof(m_lan));
            }
            pclose(fp);
        }
        return m_lan;
    }

    float get_size(const char *prefix, const char *filename) {
        char full[256] = {0};
        sprintf(full, "%s%s", prefix, filename);
        LOG(INFO) << "full name: " << full;
        int fd = open(full, O_RDWR);
        if (fd < 0) {
            LOG(WARNING) << "open fail " << full;
            return -1;
        }
        struct stat st;
        fstat(fd, &st);
        close(fd);
        return st.st_size;
    }

    void clean_audio_server_file(const char *prefix) {
        char cmd[128] = {0};
        sprintf(cmd, "rm %s/audiodata/*", prefix);
        system(cmd);
        sprintf(cmd, "rm %s/cfg/*.json", prefix);
        system(cmd);
    }

    void openwrt_restore_network() {
        char uci[128] = {0};
        sprintf(uci, "uci set network.lan.ipaddr=%s", "192.168.1.100");
        system(uci);
        sprintf(uci, "uci set network.lan.gateway=%s", "192.168.1.1");
        system(uci);
        sprintf(uci, "uci set network.lan.netmask=%s", "255.255.255.0");
        system(uci);
        sprintf(uci, "uci commit network");
        system(uci);
        sprintf(uci, "/etc/init.d/network reload");
        system(uci);
    }

std::string hex_to_string(const std::string &str) {
        std::string result;
        for (size_t i = 0; i < str.length(); i += 2) {
            std::string byte = str.substr(i, 2);
            result.push_back(static_cast<char>(std::strtol(byte.c_str(), nullptr, 16)));
        }
        return result;
    }
    std::string string_to_hex(const std::string &input) {
        static const char *const lut = "0123456789ABCDEF";
        size_t len = input.length();
        std::string output;
        output.reserve(2 * len);
        for (size_t i = 0; i < len; ++i) {
            const unsigned char c = input[i];
            output.push_back(lut[c >> 4]);
            output.push_back(lut[c & 15]);
        }
        return output;
    }
    static std::string ustr_to_hex(const unsigned char ch[], const int len) {
        const std::string hex = "0123456789ABCDEF";
        std::stringstream ss;
        for (int i = 0; i < len; ++i) {
            ss << hex[ch[i] >> 4] << hex[ch[i] & 0xf];
        }
        return ss.str();
    }
    void udp_multicast_send(const std::string &ip, uint16_t port, const std::string &msg) {
        struct sockaddr_in server{};
        struct ip_mreqn group{};
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        bzero(&server, sizeof(server));
        server.sin_family = AF_INET;
        inet_pton(AF_INET, get_ros_addr(), &server.sin_addr.s_addr);
        server.sin_port = htons(port);
        ::bind(sock, (struct sockaddr *) &server, sizeof(server));
        inet_pton(AF_INET, ip.c_str(), &group.imr_multiaddr.s_addr);
        inet_pton(AF_INET, "0.0.0.0", &group.imr_address);
        if (isIpWL() == 0) {
            group.imr_ifindex = if_nametoindex("br0");
        } else {
            group.imr_ifindex = if_nametoindex("WAN1");
        }
        setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &group.imr_multiaddr.s_addr, sizeof(group.imr_multiaddr.s_addr));
        struct sockaddr_in client{};
        bzero(&client, sizeof(client));
        client.sin_family = AF_INET;
        inet_pton(AF_INET, ip.c_str(), &client.sin_addr.s_addr);
        client.sin_port = htons(port);
        sendto(sock, msg.c_str(), msg.length(), 0, (struct sockaddr *) &client, sizeof(client));
        LOG_EVERY_N(10, INFO) << "LOG_EVERY_N(10, INFO)" << ip << " " << port << " udp_multicast_send: " << msg;
        close(sock);
    }
    static void cmd_system(const std::string &cmd) {
        system(cmd.c_str());
    }
    template<typename callable, class... arguments>
    static void async_wait(const size_t count, const size_t after, const size_t interval, callable &&f, arguments &&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task
                (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        std::thread([after, task, count, interval]() {
            if (count == 0) {
                std::this_thread::sleep_for(std::chrono::seconds(after));
                while (true) {
                    task();
                    std::this_thread::sleep_for(std::chrono::seconds(interval));
                }
            } else {
                std::this_thread::sleep_for(std::chrono::seconds(after));
                for (size_t i = 0; i < count; ++i) {
                    task();
                    std::this_thread::sleep_for(std::chrono::seconds(interval));
                }
            }
        }).detach();
    }
    std::vector<std::string> string_split(const std::string &str_v, const std::string &del_ims = " ") {
        std::vector<std::string> output;
        size_t first = 0;
        while (first < str_v.size()) {
            const auto second = str_v.find_first_of(del_ims, first);
            if (first != second) {
                output.emplace_back(str_v.substr(first, second - first));
            }
            if (second == std::string::npos) {
                break;
            }
            first = second + 1;
        }
        return output;
    }
    bool check(const unsigned char c) {
        return c >= 0x80;
    }
    int statistical_character_count(const std::string &str) {
        int LowerCase = 0, UpperCase = 0, space = 0, digit = 0, character = 0, chinese = 0;
        for (const char &ch : str) {
            if (std::islower(ch)) {
                LowerCase++;
            } else if (std::isupper(ch)) {
                UpperCase++;
            } else if (std::isdigit(ch)) {
                digit++;
            } else if (check(ch)) {
                chinese++;
            } else if (ch == ' ') {
                space++;
            } else {
                character++;
            }
        }
        return UpperCase + LowerCase + digit + character + (chinese / 3) + space;
    }
    int change_password(const char *password) {
        if (is_ros_platform()) {
            char buf[64] = {0};
            snprintf(buf, sizeof(buf), "webs -P %s", password);
            system(buf);
            snprintf(buf, sizeof(buf), "cm set_val sys serverpassword %s", password);
            system(buf);
        }
        return 1;
    }
    void bit_rate_32_to_48(const std::string &path) {
        std::string cmd = "conv.sh " + path;
        LOG(INFO) << "cmd : " << cmd;
        system(cmd.c_str());
    }
    void volume_gain(const std::string &path, const std::string &suffix) {
        std::string cmd = "vol.sh " + path + " " + suffix;
        LOG(INFO) << "cmd : " << cmd;
        system(cmd.c_str());
    }
    void audio_clear() {
        system("rm /tmp/output.wav");
        system("rm /tmp/output.pcm");
        system("rm /tmp/vol.wav");
        system("rm /tmp/x.pcm");
        system("rm /tmp/new_mp3");
    }
    static void reboot() {
        system("reboot");
    }
    void record_start(const bool async = false) {
        if (async) {
            async_wait(1, 0, 0, [&] {
                cmd_system(asns::RECORD_CMD);
            });
        } else {
            cmd_system(asns::RECORD_CMD);
        }
    }
    void record_stop() {
        cmd_system("killall ffmpeg");
        int time = 0;
        for (int i = 0; i < 20; i++) {
            if (get_process_status("ffmpeg")) {
                usleep(1000 * 100);
                time++;
            } else {
                break;
            }
        }
        LOG(INFO) << "killall ffmpeg record write to flash time : " << time << "00 ms";
    }
    std::string record_upload(const int time, const std::string &url, const std::string &imei) {
        std::string cmd = "ffmpeg -t " + std::to_string(time + asns::RECORD_TIME_COMPENSATE) +
                          " -y -f alsa -sample_rate 44100 -i hw:0,0 -acodec libmp3lame -ar 8k /tmp/record.mp3";
        cmd_system(cmd);
        std::string res = get_doupload_result(url, imei);
        cmd_system("rm /tmp/record.mp3");
        return res;
    }
    void restore(const std::string &path = "") {
        if (is_ros_platform()) {
            clean_audio_server_file(path.c_str());
            cmd_system("cp /usr/lib/mp3/*.mp3 /mnt/audiodata/");
            cmd_system("cp /usr/lib/mp3/*.json /mnt/cfg/");
            system("cm default");
            system("reboot");
        } else {
            clean_audio_server_file(path.c_str());
            openwrt_restore_network();
        }
    }
    void network_set(const std::string &gateway, const std::string &ipAddress, const std::string &netMask) {
        if (is_ros_platform()) {
            char cm[128] = {0};
            snprintf(cm, sizeof(cm), "cm set_val WAN1 gateway %s", gateway.c_str());
            system(cm);
            snprintf(cm, sizeof(cm), "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
            system(cm);
            snprintf(cm, sizeof(cm), "cm set_val WAN1 ipmask %s", netMask.c_str());
            system(cm);
            system("reboot");
        } else {
            char uci[128] = {0};
            snprintf(uci, sizeof(uci), "uci set network.lan.ipaddr=%s", ipAddress.c_str());
            system(uci);
            snprintf(uci, sizeof(uci), "uci set network.lan.gateway=%s", gateway.c_str());
            system(uci);
            snprintf(uci, sizeof(uci), "uci set network.lan.netmask=%s", netMask.c_str());
            system(uci);
            snprintf(uci, sizeof(uci), "uci commit network");
            system(uci);
            snprintf(uci, sizeof(uci), "/etc/init.d/network reload &");
            system(uci);
        }
    }
    void network_set(const std::string &gateway, const std::string &ipAddress) {
        if (is_ros_platform()) {
            char cm[128] = {0};
            snprintf(cm, sizeof(cm), "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
            system(cm);
            snprintf(cm, sizeof(cm), "cm set_val WAN1 gateway %s", gateway.c_str());
            system(cm);
            system("reboot");
        } else {
            char uci[128] = {0};
            snprintf(uci, sizeof(uci), "uci set network.lan.ipaddr=%s", ipAddress.c_str());
            system(uci);
            snprintf(uci, sizeof(uci), "uci set network.lan.gateway=%s", gateway.c_str());
            system(uci);
            snprintf(uci, sizeof(uci), "uci commit network");
            system(uci);
            snprintf(uci, sizeof(uci), "/etc/init.d/network reload");
            system(uci);
        }
    }
    std::string get_core_version() {
        return string_split(get_by_cmd_res("webs -V"), ":")[1];
    }
    std::string get_hardware_release_time() {
        std::string ver = get_by_cmd_res("webs -V");
        std::string name = get_by_cmd_res("uname -a");
        std::vector<std::string> vecVer = string_split(ver, ":");
        std::vector<std::string> vecName = string_split(name, " ");
        std::vector<std::string> vecTime = string_split(vecName[8], ":");
        std::string v = vecVer[1].substr(0, vecVer[1].find_last_of('V'));
        std::string vn = vecVer[1].substr(vecVer[1].find_last_of('V'));
        std::string year = vecName[10].substr(vecName[10].length() - 2, 2);
        std::string month = m_month[vecName[6]];
        int day = std::atoi(vecName[7].c_str());
        int hour = std::atoi(vecTime[0].c_str());
        int min = std::atoi(vecTime[1].c_str());
        int sec = std::atoi(vecTime[2].c_str());
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "%02d_%02d%02d%02d", day, hour, min, sec);
        return vn + '_' + v + '_' + year + month + buf;
    }
    void heart_beat(const std::string &path) {
        cmd_system("echo $(date +\"%s\") > " + path);
    }
    static uint16_t crc16(const uint8_t *data, uint16_t length) {
        uint16_t crc = 0xffff; // Initial value
        while (length--) {
            crc ^= *data++; // crc ^= *data; data++;
            for (uint8_t i = 0; i < 8; ++i) {
                if (crc & 1) {
                    crc = (crc >> 1) ^ 0xA001; // 0xA001 = reverse 0x8005
                } else {
                    crc = (crc >> 1);
                }
            }
        }
        return crc;
    }
    static std::string fmt_float_to_str(const double val) {
        char buf[16] = {0};
        snprintf(buf, sizeof(buf), "%.01f", val);
        return buf;
    }
};