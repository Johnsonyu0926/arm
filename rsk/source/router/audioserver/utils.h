#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/statfs.h>
#include <string>
#include <dirent.h>
#include <cstring>
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
#include "public.hpp"
#include "audiocfg.hpp"
#include "doorsbase.h"
#include "easylogging++.h"
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdint.h>
#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/statfs.h>
#include <string>
#include <dirent.h>
#include <cstring>
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
#include "public.hpp"
#include "audiocfg.hpp"
#include "doorsbase.h"
#include "easylogging++.h"

#ifndef UTILS_H
#define UTILS_H



#include <stdbool.h>
#include <stdint.h>
bool CUtils_is_ros_platform();
void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void));
const char *CUtils_get_lan_ip();
const char *CUtils_get_lan_netmask();
const char *CUtils_get_lan_gateway();
const char *CUtils_get_wan_ip();
const char *CUtils_get_wan_netmask();
const char *CUtils_get_wan_gateway();
const char *CUtils_get_core_version();
const char *CUtils_get_addr();
const char *CUtils_get_hardware_release_time();
const char *CUtils_get_res_by_cmd(const char *cmd);
const char *CUtils_get_netmask();
const char *CUtils_get_gateway();
unsigned long long CUtils_get_available_Disk(const char *path);
bool CUtils_get_process_status(const char *process);
bool CUtils_find_dir_file_exists(const char *dir, const char *file);
void CUtils_cmd_system(const char *cmd);
uint16_t CUtils_crc16(const uint8_t *data, int len);
bool CUtils_is_ros_platform();
void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void));
const char *CUtils_get_lan_ip();
const char *CUtils_get_lan_netmask();
const char *CUtils_get_lan_gateway();
const char *CUtils_get_wan_ip();
const char *CUtils_get_wan_netmask();
const char *CUtils_get_wan_gateway();
const char *CUtils_get_core_version();
const char *CUtils_get_addr();
const char *CUtils_get_hardware_release_time();
const char *CUtils_get_res_by_cmd(const char *cmd);
const char *CUtils_get_netmask();
const char *CUtils_get_gateway();
unsigned long long CUtils_get_available_Disk(const char *path);
bool CUtils_get_process_status(const char *process);
bool CUtils_find_dir_file_exists(const char *dir, const char *file);
void CUtils_cmd_system(const char *cmd);
uint16_t CUtils_crc16(const uint8_t *data, int len);
void CUtils_hex_to_string(const char *hex, char *str);

#endif // UTILS_H

bool CUtils_is_ros_platform();


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
    char m_lan[1024];
    std::map<std::string, std::string> m_month = {{"Jan", "01"},
                                                  {"Feb", "02"},
                                                  {"Mar", "03"},
                                                  {"Apr", "04"},
                                                  {"May", "05"},
                                                  {"Jun", "06"},
                                                  {"Jul", "07"},
                                                  {"Aug", "08"},
                                                  {"Sep", "09"},
                                                  {"Oct", "10"},
                                                  {"Nov", "11"},
                                                  {"Dec", "12"}};
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
        for (const auto & iter : files_name) {
            LOG(DEBUG) <<  iter << " ";
            if (name == iter) {
                return true;
            }
        }
        return false;
    }

    bool str_is_all_num(const std::string &str) {
        for (const char &c: str) {
            if (std::isdigit(c) == 0)
                return false;
        }
        return true;
    }

    static unsigned long long get_available_Disk(const std::string &path) {
        struct statfs diskInfo;
        statfs(path.c_str(), &diskInfo);

        unsigned long long block_size = diskInfo.f_bsize;                       //每个block里包含的字节数
        //unsigned long long total_size = block_size * diskInfo.f_blocks;         //总的字节数，f_blocks为block的数目
        //unsigned long long free_disk = diskInfo.f_bfree * block_size;           //剩余空间的大小
        unsigned long long available_disk = diskInfo.f_bavail * block_size;     //可用空间大小
        return (available_disk >> 10);                                         // 返回可用大小 kb
        /*printf("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n", total_size, total_size >> 10, total_size >> 20,
               total_size >> 30);
        printf("Disk_free = %llu MB = %llu GB\nDisk_available = %llu MB = %llu GB\n",
               free_disk >> 20, free_disk >> 30, available_disk >> 20, available_disk >> 30);*/
    }

    /* 1, process exist.
     * 0, not exist
     */
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

    int isIpWL(){
        std::string lanIp;
        std::string wanIp;
        if(is_ros_platform()){
            lanIp = get_by_cmd_res("cm get_val VLAN1 ipaddress | tail -1");
            wanIp = get_by_cmd_res("cm get_val WAN1 ipaddress | tail -1");
        }else {
            lanIp = get_by_cmd_res("uci get network.lan.ipaddr");
            wanIp = get_by_cmd_res("uci get network.wan.ipaddr");
        }
        std::string connIp = Singleton::getInstance().getConnIp();
        if(connIp.empty()){
            return 1;
        }
        connIp = connIp.substr(0, connIp.find_last_of('.'));
        if(connIp.compare(lanIp.substr(0, lanIp.find_last_of('.'))) == 0){
            return 0;
        }else if(connIp.compare(wanIp.substr(0, wanIp.find_last_of('.'))) == 0){
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
        if(isIpWL() == 0){
            strcpy(cmd, "cm get_val VLAN1 ipaddress | tail -1");
        } else {
            strcpy(cmd, "cm get_val WAN1 ipaddress|tail -1");
        }
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_ros_gateway() {
        char cmd[64] = {0};
        if(isIpWL() == 0){
            strcpy(cmd, "cm get_val VLAN1 gateway | tail -1");
        } else {
            strcpy(cmd, "cm get_val WAN1 gateway|tail -1");
        }
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_ros_netmask() {
        char cmd[64] = {0};
        if(isIpWL() == 0){
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
            if(isIpWL() == 0){
                strcpy(cmd, "uci get network.lan.gateway");
            } else {
                strcpy(cmd, "uci get network.wan.gateway");
            }
            get_addr_by_cmd(cmd);
        }
        //LOG(INFO) << "gateway = " << m_lan;
        return m_lan;
    }

    char *get_netmask() {
        if (is_ros_platform()) {
            get_ros_netmask();
        } else {
            char cmd[64] = {};
            if(isIpWL() == 0){
                strcpy(cmd, "uci get network.lan.netmask");
            }else{
                strcpy(cmd, "uci get network.wan.netmask");
            }
            get_addr_by_cmd(cmd);
        }
        //LOG(INFO) << "netmask = " << m_lan;
        return m_lan;
    }

    char *get_addr() {
        if (is_ros_platform()) {
            get_ros_addr();
        } else {
            char cmd[64] = {0};
            if(isIpWL() == 0){
                strcpy(cmd, "uci get network.lan.ipaddr");
            } else {
                strcpy(cmd, "uci get network.wan.ipaddr");
            }
            get_addr_by_cmd(cmd);
        }
        //DS_TRACE("address = " << m_lan;
        return m_lan;
    }

    std::string get_lan_ip(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val VLAN1 ipaddress | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.lan.ipaddr");
        }
    }

    std::string get_lan_netmask(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val VLAN1 ipmask | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.lan.netmask");
        }
    }

    std::string get_lan_gateway(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val VLAN1 gateway | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.lan.gateway");
        }
    }

    std::string get_wan_ip(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val WAN1 ipaddress | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.wan.ipaddr");
        }
    }

    std::string get_wan_netmask(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val WAN1 ipmask | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.wan.netmask");
        }
    }

    std::string get_wan_gateway(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val WAN1 gateway | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.wan.gateway");
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

    //curl是将消息写入到stderr,所以popen获取不到，需要把stderr重定向stdout 2>&1
    std::string get_upload_result(const std::string &url, const std::string &path, const std::string &name) {
        char cmd[1024] = {0};
        sprintf(cmd, "curl --location --request GET %s -f --output %s%s 2>&1", url.c_str(), path.c_str(), name.c_str());
        LOG(INFO) << "cmd:" << cmd;
        std::string res = get_by_cmd_res(cmd);
        LOG(INFO) << "res:" << res;
        //返回提示信息
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
                //DS_TRACE("ok address is: " << m_lan);
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
        //十六进制两个字符为原始字符一个字符
        for (size_t i = 0; i < str.length(); i += 2) {
            //每次切两个字符
            std::string byte = str.substr(i, 2);
            //将十六进制的string转成long再强转成int再转成char
            result.push_back(static_cast<char>(static_cast<int>(std::strtol(byte.c_str(), nullptr, 16))));//将处理完的字符压入result中
        }
        return result;
    }

    std::string string_to_hex(const std::string &input) {
        static const char *const lut = "0123456789ABCDEF";
        size_t len = input.length();
        std::string output;
        output.reserve(2 * len);//预分配两倍的空间
        for (size_t i = 0; i < len; ++i) {
            const unsigned char c = input[i];//存储第一个字符
            //该char字符的二进制右移四位获取十六进制的第一个字符
            output.push_back(lut[c >> 4]);
            //清除该char字符的高4位，保留低四位
            output.push_back(lut[c & 15]);
        }
        return output;
    }

    static std::string ustr_to_hex(const unsigned char ch[], const int len) {
	const std::string hex = "0123456789ABCDEF";
	std::stringstream ss;
	for(int i = 0; i < len; ++i) {
		ss << hex[ch[i] >> 4] << hex[ch[i] & 0xf];
	}
	return ss.str();
    }

    void udp_multicast_send(const std::string &ip, uint16_t port, const std::string &msg) {
        struct sockaddr_in server{};
        struct ip_mreqn group{};

        int sock = socket(AF_INET, SOCK_DGRAM, 0);                /* 构造用于UDP通信的套接字 */

        bzero(&server, sizeof(server));
        server.sin_family = AF_INET;                        /* IPv4 */
        inet_pton(AF_INET, get_ros_addr(), &server.sin_addr.s_addr);         /* 本地任意IP INADDR_ANY = 0 */
        server.sin_port = htons(port);
bool CUtils_is_ros_platform();
void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void));
const char *CUtils_get_lan_ip();
const char *CUtils_get_lan_netmask();
const char *CUtils_get_lan_gateway();
const char *CUtils_get_wan_ip();
const char *CUtils_get_wan_netmask();
const char *CUtils_get_wan_gateway();
const char *CUtils_get_core_version();
const char *CUtils_get_addr();
const char *CUtils_get_hardware_release_time();
const char *CUtils_get_res_by_cmd(const char *cmd);
const char *CUtils_get_netmask();
const char *CUtils_get_gateway();
unsigned long long CUtils_get_available_Disk(const char *path);
bool CUtils_get_process_status(const char *process);
bool CUtils_find_dir_file_exists(const char *dir, const char *file);
void CUtils_cmd_system(const char *cmd);
uint16_t CUtils_crc16(const uint8_t *data, int len);
void CUtils_change_password(const char *password);

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
    char m_lan[1024];
    std::map<std::string, std::string> m_month = {{"Jan", "01"},
                                                  {"Feb", "02"},
                                                  {"Mar", "03"},
                                                  {"Apr", "04"},
                                                  {"May", "05"},
                                                  {"Jun", "06"},
                                                  {"Jul", "07"},
                                                  {"Aug", "08"},
                                                  {"Sep", "09"},
                                                  {"Oct", "10"},
                                                  {"Nov", "11"},
                                                  {"Dec", "12"}};
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
        for (const auto & iter : files_name) {
            LOG(DEBUG) <<  iter << " ";
            if (name == iter) {
                return true;
            }
        }
        return false;
    }

    bool str_is_all_num(const std::string &str) {
        for (const char &c: str) {
            if (std::isdigit(c) == 0)
                return false;
        }
        return true;
    }

    static unsigned long long get_available_Disk(const std::string &path) {
        struct statfs diskInfo;
        statfs(path.c_str(), &diskInfo);

        unsigned long long block_size = diskInfo.f_bsize;                       //每个block里包含的字节数
        //unsigned long long total_size = block_size * diskInfo.f_blocks;         //总的字节数，f_blocks为block的数目
        //unsigned long long free_disk = diskInfo.f_bfree * block_size;           //剩余空间的大小
        unsigned long long available_disk = diskInfo.f_bavail * block_size;     //可用空间大小
        return (available_disk >> 10);                                         // 返回可用大小 kb
        /*printf("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n", total_size, total_size >> 10, total_size >> 20,
               total_size >> 30);
        printf("Disk_free = %llu MB = %llu GB\nDisk_available = %llu MB = %llu GB\n",
               free_disk >> 20, free_disk >> 30, available_disk >> 20, available_disk >> 30);*/
    }

    /* 1, process exist.
     * 0, not exist
     */
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

    int isIpWL(){
        std::string lanIp;
        std::string wanIp;
        if(is_ros_platform()){
            lanIp = get_by_cmd_res("cm get_val VLAN1 ipaddress | tail -1");
            wanIp = get_by_cmd_res("cm get_val WAN1 ipaddress | tail -1");
        }else {
            lanIp = get_by_cmd_res("uci get network.lan.ipaddr");
            wanIp = get_by_cmd_res("uci get network.wan.ipaddr");
        }
        std::string connIp = Singleton::getInstance().getConnIp();
        if(connIp.empty()){
            return 1;
        }
        connIp = connIp.substr(0, connIp.find_last_of('.'));
        if(connIp.compare(lanIp.substr(0, lanIp.find_last_of('.'))) == 0){
            return 0;
        }else if(connIp.compare(wanIp.substr(0, wanIp.find_last_of('.'))) == 0){
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
        if(isIpWL() == 0){
            strcpy(cmd, "cm get_val VLAN1 ipaddress | tail -1");
        } else {
            strcpy(cmd, "cm get_val WAN1 ipaddress|tail -1");
        }
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_ros_gateway() {
        char cmd[64] = {0};
        if(isIpWL() == 0){
            strcpy(cmd, "cm get_val VLAN1 gateway | tail -1");
        } else {
            strcpy(cmd, "cm get_val WAN1 gateway|tail -1");
        }
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_ros_netmask() {
        char cmd[64] = {0};
        if(isIpWL() == 0){
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
            if(isIpWL() == 0){
                strcpy(cmd, "uci get network.lan.gateway");
            } else {
                strcpy(cmd, "uci get network.wan.gateway");
            }
            get_addr_by_cmd(cmd);
        }
        //LOG(INFO) << "gateway = " << m_lan;
        return m_lan;
    }

    char *get_netmask() {
        if (is_ros_platform()) {
            get_ros_netmask();
        } else {
            char cmd[64] = {};
            if(isIpWL() == 0){
                strcpy(cmd, "uci get network.lan.netmask");
            }else{
                strcpy(cmd, "uci get network.wan.netmask");
            }
            get_addr_by_cmd(cmd);
        }
        //LOG(INFO) << "netmask = " << m_lan;
        return m_lan;
    }

    char *get_addr() {
        if (is_ros_platform()) {
            get_ros_addr();
        } else {
            char cmd[64] = {0};
            if(isIpWL() == 0){
                strcpy(cmd, "uci get network.lan.ipaddr");
            } else {
                strcpy(cmd, "uci get network.wan.ipaddr");
            }
            get_addr_by_cmd(cmd);
        }
        //DS_TRACE("address = " << m_lan;
        return m_lan;
    }

    std::string get_lan_ip(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val VLAN1 ipaddress | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.lan.ipaddr");
        }
    }

    std::string get_lan_netmask(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val VLAN1 ipmask | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.lan.netmask");
        }
    }

    std::string get_lan_gateway(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val VLAN1 gateway | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.lan.gateway");
        }
    }

    std::string get_wan_ip(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val WAN1 ipaddress | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.wan.ipaddr");
        }
    }

    std::string get_wan_netmask(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val WAN1 ipmask | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.wan.netmask");
        }
    }

    std::string get_wan_gateway(){
        if(is_ros_platform()){
            return get_addr_by_cmd("cm get_val WAN1 gateway | tail -1");
        }else{
            return get_addr_by_cmd("uci get network.wan.gateway");
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

    //curl是将消息写入到stderr,所以popen获取不到，需要把stderr重定向stdout 2>&1
    std::string get_upload_result(const std::string &url, const std::string &path, const std::string &name) {
        char cmd[1024] = {0};
        sprintf(cmd, "curl --location --request GET %s -f --output %s%s 2>&1", url.c_str(), path.c_str(), name.c_str());
        LOG(INFO) << "cmd:" << cmd;
        std::string res = get_by_cmd_res(cmd);
        LOG(INFO) << "res:" << res;
        //返回提示信息
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
                //DS_TRACE("ok address is: " << m_lan);
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
        //十六进制两个字符为原始字符一个字符
        for (size_t i = 0; i < str.length(); i += 2) {
            //每次切两个字符
            std::string byte = str.substr(i, 2);
            //将十六进制的string转成long再强转成int再转成char
            result.push_back(static_cast<char>(static_cast<int>(std::strtol(byte.c_str(), nullptr, 16))));//将处理完的字符压入result中
        }
        return result;
    }

    std::string string_to_hex(const std::string &input) {
        static const char *const lut = "0123456789ABCDEF";
        size_t len = input.length();
        std::string output;
        output.reserve(2 * len);//预分配两倍的空间
        for (size_t i = 0; i < len; ++i) {
            const unsigned char c = input[i];//存储第一个字符
            //该char字符的二进制右移四位获取十六进制的第一个字符
            output.push_back(lut[c >> 4]);
            //清除该char字符的高4位，保留低四位
            output.push_back(lut[c & 15]);
        }
        return output;
    }

    static std::string ustr_to_hex(const unsigned char ch[], const int len) {
	const std::string hex = "0123456789ABCDEF";
	std::stringstream ss;
	for(int i = 0; i < len; ++i) {
		ss << hex[ch[i] >> 4] << hex[ch[i] & 0xf];
	}
	return ss.str();
    }

    void udp_multicast_send(const std::string &ip, uint16_t port, const std::string &msg) {
        struct sockaddr_in server{};
        struct ip_mreqn group{};

        int sock = socket(AF_INET, SOCK_DGRAM, 0);                /* 构造用于UDP通信的套接字 */

        bzero(&server, sizeof(server));
        server.sin_family = AF_INET;                        /* IPv4 */
        inet_pton(AF_INET, get_ros_addr(), &server.sin_addr.s_addr);         /* 本地任意IP INADDR_ANY = 0 */
        server.sin_port = htons(port);

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/statfs.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char m_lan[1024];
} CUtils;

int CUtils_get_file_size(const char *path);
void CUtils_get_dir_file_names(const char *path, char ***files, int *count);
bool CUtils_find_dir_file_exists(const char *path, const char *name);
bool CUtils_str_is_all_num(const char *str);
unsigned long long CUtils_get_available_Disk(const char *path);
int CUtils_get_process_status(const char *cmd);
int CUtils_isIpWL();
int CUtils_is_ros_platform();
char *CUtils_get_ros_addr(CUtils *self);
char *CUtils_get_ros_gateway(CUtils *self);
char *CUtils_get_ros_netmask(CUtils *self);
char *CUtils_get_gateway(CUtils *self);
char *CUtils_get_netmask(CUtils *self);
char *CUtils_get_addr(CUtils *self);
char *CUtils_get_addr_by_cmd(CUtils *self, const char *cmd);
char *CUtils_get_res_by_cmd(const char *cmd);
void CUtils_cmd_system(const char *cmd);
void CUtils_change_password(const char *password);
void CUtils_bit_rate_32_to_48(const char *path);
void CUtils_volume_gain(const char *path, const char *suffix);
void CUtils_audio_clear();
void CUtils_reboot();
void CUtils_record_start(bool async);
void CUtils_record_stop();
char *CUtils_record_upload(int time, const char *url, const char *imei);
void CUtils_restore(const char *path);
void CUtils_network_set(const char *gateway, const char *ipAddress, const char *netMask);
void CUtils_network_set_ip(const char *gateway, const char *ipAddress);
char *CUtils_get_core_version();
char *CUtils_get_hardware_release_time();
void CUtils_heart_beat(const char *path);
uint16_t CUtils_crc16(const uint8_t *data, uint16_t length);
char *CUtils_fmt_float_to_str(double val);
bool CUtils_is_ros_platform();
void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void));
const char *CUtils_get_lan_ip();
const char *CUtils_get_lan_netmask();
const char *CUtils_get_lan_gateway();
const char *CUtils_get_wan_ip();
const char *CUtils_get_wan_netmask();
const char *CUtils_get_wan_gateway();
const char *CUtils_get_core_version();
const char *CUtils_get_addr();
const char *CUtils_get_hardware_release_time();
const char *CUtils_get_res_by_cmd(const char *cmd);
const char *CUtils_get_netmask();
const char *CUtils_get_gateway();
unsigned long long CUtils_get_available_Disk(const char *path);
bool CUtils_get_process_status(const char *process);
bool CUtils_find_dir_file_exists(const char *dir, const char *file);
void CUtils_cmd_system(const char *cmd);
uint16_t CUtils_crc16(const uint8_t *data, int len);
int CUtils_get_file_size(const char *path);
void CUtils_bit_rate_32_to_48(const char *path);
char *CUtils_get_by_cmd_res(const char *cmd);
void CUtils_reboot();

#endif // UTILS_H