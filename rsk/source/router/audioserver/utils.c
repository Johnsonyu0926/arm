#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <stdbool.h>
#include <stdint.h>
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

bool CUtils_is_ros_platform() {
    // 实现判断是否为ROS平台的函数
    return false; // 示例返回值
}
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>


bool CUtils_is_ros_platform() {
    // 实现判断是否为ROS平台的函数
    return false; // 示例返回值
}
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

bool CUtils_is_ros_platform() {
    // 实现判断是否为ROS平台的函数
    return false; // 示例返回值
}

void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void)) {
    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds;
    for (int i = 0; i < repeat; i++) {
        nanosleep(&req, NULL);
        callback();
    }
}

const char *CUtils_get_lan_ip() {
    // 实现获取LAN IP地址的函数
    return "192.168.1.2"; // 示例返回值
}

const char *CUtils_get_lan_netmask() {
    // 实现获取LAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_lan_gateway() {
    // 实现获取LAN网关的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_wan_ip() {
    // 实现获取WAN IP地址的函数
    return "10.0.0.2"; // 示例返回值
}

const char *CUtils_get_wan_netmask() {
    // 实现获取WAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_wan_gateway() {
    // 实现获取WAN网关的函数
    return "10.0.0.1"; // 示例返回值
}

const char *CUtils_get_core_version() {
    // 实现获取核心版本的函数
    return "core_version"; // 示例返回值
}

const char *CUtils_get_addr() {
    // 实现获取IP地址的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_hardware_release_time() {
    // 实现获取硬件发布时间的函数
    return "2023-01-01"; // 示例返回值
}

const char *CUtils_get_res_by_cmd(const char *cmd) {
    // 实现通过命令获取结果的函数
    return "result"; // 示例返回值
}

const char *CUtils_get_netmask() {
    // 实现获取子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_gateway() {
    // 实现获取网关的函数
    return "192.168.1.254"; // 示例返回值
}

unsigned long long CUtils_get_available_Disk(const char *path) {
    // 实现获取磁盘可用空间的函数
    return 10000000; // 示例返回值
}

bool CUtils_get_process_status(const char *process) {
    // 实现获取进程状态的函数
    return false; // 示例返回值
}

bool CUtils_find_dir_file_exists(const char *dir, const char *file) {
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(dir)) == NULL) {
        return false;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, file) == 0) {
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

void CUtils_cmd_system(const char *cmd) {
    system(cmd);
}

uint16_t CUtils_crc16(const uint8_t *data, int len) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
bool CUtils_is_ros_platform() {
    // 实现判断是否为ROS平台的函数
    return false; // 示例返回值
}

void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void)) {
    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds;
    for (int i = 0; i < repeat; i++) {
        nanosleep(&req, NULL);
        callback();
    }
}

const char *CUtils_get_lan_ip() {
    // 实现获取LAN IP地址的函数
    return "192.168.1.2"; // 示例返回值
}

const char *CUtils_get_lan_netmask() {
    // 实现获取LAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_lan_gateway() {
    // 实现获取LAN网关的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_wan_ip() {
    // 实现获取WAN IP地址的函数
    return "10.0.0.2"; // 示例返回值
}

const char *CUtils_get_wan_netmask() {
    // 实现获取WAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_wan_gateway() {
    // 实现获取WAN网关的函数
    return "10.0.0.1"; // 示例返回值
}

const char *CUtils_get_core_version() {
    // 实现获取核心版本的函数
    return "core_version"; // 示例返回值
}

const char *CUtils_get_addr() {
    // 实现获取IP地址的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_hardware_release_time() {
    // 实现获取硬件发布时间的函数
    return "2023-01-01"; // 示例返回值
}

const char *CUtils_get_res_by_cmd(const char *cmd) {
    // 实现通过命令获取结果的函数
    return "result"; // 示例返回值
}

const char *CUtils_get_netmask() {
    // 实现获取子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_gateway() {
    // 实现获取网关的函数
    return "192.168.1.254"; // 示例返回值
}

unsigned long long CUtils_get_available_Disk(const char *path) {
    // 实现获取磁盘可用空间的函数
    return 10000000; // 示例返回值
}

bool CUtils_get_process_status(const char *process) {
    // 实现获取进程状态的函数
    return false; // 示例返回值
}

bool CUtils_find_dir_file_exists(const char *dir, const char *file) {
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(dir)) == NULL) {
        return false;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, file) == 0) {
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

void CUtils_cmd_system(const char *cmd) {
    system(cmd);
}

uint16_t CUtils_crc16(const uint8_t *data, int len) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void CUtils_hex_to_string(const char *hex, char *str) {
    while (*hex) {
        char high = (*hex >= 'A') ? (*hex - 'A' + 10) : (*hex - '0');
        hex++;
        char low = (*hex >= 'A') ? (*hex - 'A' + 10) : (*hex - '0');
        hex++;
        *str++ = (high << 4) | low;
    }
    *str = '\0';
}
void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void)) {
    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds;
    for (int i = 0; i < repeat; i++) {
        nanosleep(&req, NULL);
        callback();
    }
}
bool CUtils_is_ros_platform() {
    // 实现判断是否为ROS平台的函数
    return false; // 示例返回值
}

void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void)) {
    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds;
    for (int i = 0; i < repeat; i++) {
        nanosleep(&req, NULL);
        callback();
    }
}

const char *CUtils_get_lan_ip() {
    // 实现获取LAN IP地址的函数
    return "192.168.1.2"; // 示例返回值
}

const char *CUtils_get_lan_netmask() {
    // 实现获取LAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_lan_gateway() {
    // 实现获取LAN网关的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_wan_ip() {
    // 实现获取WAN IP地址的函数
    return "10.0.0.2"; // 示例返回值
}

const char *CUtils_get_wan_netmask() {
    // 实现获取WAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_wan_gateway() {
    // 实现获取WAN网关的函数
    return "10.0.0.1"; // 示例返回值
}

const char *CUtils_get_core_version() {
    // 实现获取核心版本的函数
    return "core_version"; // 示例返回值
}

const char *CUtils_get_addr() {
    // 实现获取IP地址的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_hardware_release_time() {
    // 实现获取硬件发布时间的函数
    return "2023-01-01"; // 示例返回值
}

const char *CUtils_get_res_by_cmd(const char *cmd) {
    // 实现通过命令获取结果的函数
    return "result"; // 示例返回值
}

const char *CUtils_get_netmask() {
    // 实现获取子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_gateway() {
    // 实现获取网关的函数
    return "192.168.1.254"; // 示例返回值
}

unsigned long long CUtils_get_available_Disk(const char *path) {
    // 实现获取磁盘可用空间的函数
    return 10000000; // 示例返回值
}

bool CUtils_get_process_status(const char *process) {
    // 实现获取进程状态的函数
    return false; // 示例返回值
}

bool CUtils_find_dir_file_exists(const char *dir, const char *file) {
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(dir)) == NULL) {
        return false;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, file) == 0) {
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

void CUtils_cmd_system(const char *cmd) {
    system(cmd);
}

uint16_t CUtils_crc16(const uint8_t *data, int len) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void CUtils_reboot() {
    system("reboot");
}
const char *CUtils_get_lan_ip() {
    // 实现获取LAN IP地址的函数
    return "192.168.1.2"; // 示例返回值
}

const char *CUtils_get_lan_netmask() {
    // 实现获取LAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_lan_gateway() {
    // 实现获取LAN网关的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_wan_ip() {
    // 实现获取WAN IP地址的函数
    return "10.0.0.2"; // 示例返回值
}

const char *CUtils_get_wan_netmask() {
    // 实现获取WAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_wan_gateway() {
    // 实现获取WAN网关的函数
    return "10.0.0.1"; // 示例返回值
}

const char *CUtils_get_core_version() {
    // 实现获取核心版本的函数
    return "core_version"; // 示例返回值
}

const char *CUtils_get_addr() {
    // 实现获取IP地址的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_hardware_release_time() {
    // 实现获取硬件发布时间的函数
    return "2023-01-01"; // 示例返回值
}

const char *CUtils_get_res_by_cmd(const char *cmd) {
    // 实现通过命令获取结果的函数
    return "result"; // 示例返回值
}

const char *CUtils_get_netmask() {
    // 实现获取子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_gateway() {
    // 实现获取网关的函数
    return "192.168.1.254"; // 示例返回值
}

unsigned long long CUtils_get_available_Disk(const char *path) {
    // 实现获取磁盘可用空间的函数
    return 10000000; // 示例返回值
}

bool CUtils_get_process_status(const char *process) {
    // 实现获取进程状态的函数
    return false; // 示例返回值
}

bool CUtils_find_dir_file_exists(const char *dir, const char *file) {
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(dir)) == NULL) {
        return false;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, file) == 0) {
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

void CUtils_cmd_system(const char *cmd) {
    system(cmd);
}

uint16_t CUtils_crc16(const uint8_t *data, int len) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void CUtils_change_password(const char *password) {
    // 实现更改密码的函数
}
void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void)) {
    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds;
    for (int i = 0; i < repeat; i++) {
        nanosleep(&req, NULL);
        callback();
    }
}

const char *CUtils_get_lan_ip() {
    // 实现获取LAN IP地址的函数
    return "192.168.1.2"; // 示例返回值
}

const char *CUtils_get_lan_netmask() {
    // 实现获取LAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_lan_gateway() {
    // 实现获取LAN网关的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_wan_ip() {
    // 实现获取WAN IP地址的函数
    return "10.0.0.2"; // 示例返回值
}

const char *CUtils_get_wan_netmask() {
    // 实现获取WAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_wan_gateway() {
    // 实现获取WAN网关的函数
    return "10.0.0.1"; // 示例返回值
}

const char *CUtils_get_core_version() {
    // 实现获取核心版本的函数
    return "core_version"; // 示例返回值
}

const char *CUtils_get_addr() {
    // 实现获取IP地址的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_hardware_release_time() {
    // 实现获取硬件发布时间的函数
    return "2023-01-01"; // 示例返回值
}

const char *CUtils_get_res_by_cmd(const char *cmd) {
    // 实现通过命令获取结果的函数
    return "result"; // 示例返回值
}

const char *CUtils_get_netmask() {
    // 实现获取子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_gateway() {
    // 实现获取网关的函数
    return "192.168.1.254"; // 示例返回值
}

unsigned long long CUtils_get_available_Disk(const char *path) {
    // 实现获取磁盘可用空间的函数
    return 10000000; // 示例返回值
}

bool CUtils_get_process_status(const char *process) {
    // 实现获取进程状态的函数
    return false; // 示例返回值
}

bool CUtils_find_dir_file_exists(const char *dir, const char *file) {
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(dir)) == NULL) {
        return false;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, file) == 0) {
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

void CUtils_cmd_system(const char *cmd) {
    system(cmd);
}

uint16_t CUtils_crc16(const uint8_t *data, int len) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void CUtils_change_password(const char *password) {
    // 实现更改密码的函数
}
int CUtils_get_file_size(const char *path) {
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        perror("open fail");
        return -1;
    }
    struct stat st;
    fstat(fd, &st);
    close(fd);
    return st.st_size;
}

void CUtils_get_dir_file_names(const char *path, char ***files, int *count) {
    DIR *pDir;
    struct dirent *ptr;
    *files = NULL;
    *count = 0;
    if (!(pDir = opendir(path))) return;
    while ((ptr = readdir(pDir)) != NULL) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
            *files = realloc(*files, (*count + 1) * sizeof(char *));
            (*files)[*count] = strdup(ptr->d_name);
            (*count)++;
        }
    }
    closedir(pDir);
}

bool CUtils_find_dir_file_exists(const char *path, const char *name) {
    char **files;
    int count;
    CUtils_get_dir_file_names(path, &files, &count);
    for (int i = 0; i < count; i++) {
        if (strcmp(name, files[i]) == 0) {
            for (int j = 0; j < count; j++) {
                free(files[j]);
            }
            free(files);
            return true;
        }
        free(files[i]);
    }
    free(files);
    return false;
}

bool CUtils_str_is_all_num(const char *str) {
    for (const char *c = str; *c; c++) {
        if (!isdigit(*c)) {
            return false;
        }
    }
    return true;
}

unsigned long long CUtils_get_available_Disk(const char *path) {
    struct statfs diskInfo;
    statfs(path, &diskInfo);
    unsigned long long block_size = diskInfo.f_bsize;
    unsigned long long available_disk = diskInfo.f_bavail * block_size;
    return (available_disk >> 10);
}

int CUtils_get_process_status(const char *cmd) {
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

int CUtils_isIpWL() {
    char lanIp[64];
    char wanIp[64];
    if (CUtils_is_ros_platform()) {
        strcpy(lanIp, CUtils_get_res_by_cmd("cm get_val VLAN1 ipaddress | tail -1"));
        strcpy(wanIp, CUtils_get_res_by_cmd("cm get_val WAN1 ipaddress | tail -1"));
    } else {
        strcpy(lanIp, CUtils_get_res_by_cmd("uci get network.lan.ipaddr"));
        strcpy(wanIp, CUtils_get_res_by_cmd("uci get network.wan.ipaddr"));
    }
    char connIp[64];
    strcpy(connIp, Singleton_getInstance()->getConnIp());
    if (strlen(connIp) == 0) {
        return 1;
    }
    char *connIpPrefix = strtok(connIp, ".");
    char *lanIpPrefix = strtok(lanIp, ".");
    char *wanIpPrefix = strtok(wanIp, ".");
    if (strcmp(connIpPrefix, lanIpPrefix) == 0) {
        return 0;
    } else if (strcmp(connIpPrefix, wanIpPrefix) == 0) {
        return 1;
    } else {
        return 1;
    }
}

int CUtils_is_ros_platform() {
    FILE *file = fopen("/bin/cm", "r");
    if (!file) {
        return 0;
    }
    fclose(file);
    return 1;
}

char *CUtils_get_ros_addr(CUtils *self) {
    char cmd[64] = {0};
    if (CUtils_isIpWL() == 0) {
        strcpy(cmd, "cm get_val VLAN1 ipaddress | tail -1");
    } else {
        strcpy(cmd, "cm get_val WAN1 ipaddress | tail -1");
    }
    return CUtils_get_addr_by_cmd(self, cmd);
}

char *CUtils_get_ros_gateway(CUtils *self) {
    char cmd[64] = {0};
    if (CUtils_isIpWL() == 0) {
        strcpy(cmd, "cm get_val VLAN1 gateway | tail -1");
    } else {
        strcpy(cmd, "cm get_val WAN1 gateway | tail -1");
    }
    return CUtils_get_addr_by_cmd(self, cmd);
}

char *CUtils_get_ros_netmask(CUtils *self) {
    char cmd[64] = {0};
    if (CUtils_isIpWL() == 0) {
        strcpy(cmd, "cm get_val VLAN1 ipmask | tail -1");
    } else {
        strcpy(cmd, "cm get_val WAN1 ipmask | tail -1");
    }
    return CUtils_get_addr_by_cmd(self, cmd);
}

char *CUtils_get_gateway(CUtils *self) {
    if (CUtils_is_ros_platform()) {
        return CUtils_get_ros_gateway(self);
    } else {
        char cmd[64] = {0};
        if (CUtils_isIpWL() == 0) {
            strcpy(cmd, "uci get network.lan.gateway");
        } else {
            strcpy(cmd, "uci get network.wan.gateway");
        }
        return CUtils_get_addr_by_cmd(self, cmd);
    }
}

char *CUtils_get_netmask(CUtils *self) {
    if (CUtils_is_ros_platform()) {
        return CUtils_get_ros_netmask(self);
    } else {
        char cmd[64] = {0};
        if (CUtils_isIpWL() == 0) {
            strcpy(cmd, "uci get network.lan.netmask");
        } else {
            strcpy(cmd, "uci get network.wan.netmask");
        }
        return CUtils_get_addr_by_cmd(self, cmd);
    }
}

char *CUtils_get_addr(CUtils *self) {
    if (CUtils_is_ros_platform()) {
        return CUtils_get_ros_addr(self);
    } else {
        char cmd[64] = {0};
        if (CUtils_isIpWL() == 0) {
            strcpy(cmd, "uci get network.lan.ipaddr");
        } else {
            strcpy(cmd, "uci get network.wan.ipaddr");
        }
        return CUtils_get_addr_by_cmd(self, cmd);
    }
}

char *CUtils_get_addr_by_cmd(CUtils *self, const char *cmd) {
    memset(self->m_lan, 0, sizeof(self->m_lan));
    FILE *fp = popen(cmd, "r");
    if (fp) {
        int nread = fread(self->m_lan, 1, sizeof(self->m_lan), fp);
        if (nread < 0) {
            perror("fread error");
            pclose(fp);
            return self->m_lan;
        }
        char *p = strchr(self->m_lan, '\n');
        if (p) {
            *p = '\0';
        }
        p = strchr(self->m_lan, '\r');
        if (p) {
            *p = '\0';
        }
        pclose(fp);
    }
    return self->m_lan;
}

char *CUtils_get_res_by_cmd(const char *cmd) {
    static char buf[1024];
    FILE *fp = popen(cmd, "r");
    if (fp) {
        int nread = fread(buf, 1, sizeof(buf), fp);
        if (nread < 0) {
            perror("fread error");
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

void CUtils_cmd_system(const char *cmd) {
    system(cmd);
}

void CUtils_change_password(const char *password) {
    if (CUtils_is_ros_platform()) {
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "webs -P %s", password);
        system(buf);
        snprintf(buf, sizeof(buf), "cm set_val sys serverpassword %s", password);
        system(buf);
    }
}

void CUtils_bit_rate_32_to_48(const char *path) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "conv.sh %s", path);
    system(cmd);
}

void CUtils_volume_gain(const char *path, const char *suffix) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "vol.sh %s %s", path, suffix);
    system(cmd);
}

void CUtils_audio_clear() {
    system("rm /tmp/output.wav");
    system("rm /tmp/output.pcm");
    system("rm /tmp/vol.wav");
    system("rm /tmp/x.pcm");
    system("rm /tmp/new_mp3");
}

void CUtils_reboot() {
    system("reboot");
}

void CUtils_record_start(bool async) {
    if (async) {
        CUtils_async_wait(1, 0, 0, [] {
            CUtils_cmd_system(asns::RECORD_CMD);
        });
    } else {
        CUtils_cmd_system(asns::RECORD_CMD);
    }
}

void CUtils_record_stop() {
    CUtils_cmd_system("killall ffmpeg");
    int time = 0;
    for (int i = 0; i < 20; i++) {
        if (CUtils_get_process_status("ffmpeg")) {
            usleep(1000 * 100);
            time++;
        } else {
            break;
        }
    }
    printf("killall ffmpeg record write to flash time: %d00 ms\n", time);
}

char *CUtils_record_upload(int time, const char *url, const char *imei) {
    static char res[1024];
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ffmpeg -t %d -y -f alsa -sample_rate 44100 -i hw:0,0 -acodec libmp3lame -ar 8k /tmp/record.mp3", time + asns::RECORD_TIME_COMPENSATE);
    CUtils_cmd_system(cmd);
    strcpy(res, CUtils_get_doupload_result(url, imei));
    CUtils_cmd_system("rm /tmp/record.mp3");
    return res;
}

void CUtils_restore(const char *path) {
    if (CUtils_is_ros_platform()) {
        CUtils_clean_audio_server_file(path);
        CUtils_cmd_system("cp /usr/lib/mp3/*.mp3 /mnt/audiodata/");
        CUtils_cmd_system("cp /usr/lib/mp3/*.json /mnt/cfg/");
        system("cm default");
        system("reboot");
    } else {
        CUtils_clean_audio_server_file(path);
        CUtils_openwrt_restore_network();
    }
}

void CUtils_network_set(const char *gateway, const char *ipAddress, const char *netMask) {
    if (CUtils_is_ros_platform()) {
        char cm[128];
        snprintf(cm, sizeof(cm), "cm set_val WAN1 gateway %s", gateway);
        system(cm);
        snprintf(cm, sizeof(cm), "cm set_val WAN1 ipaddress %s", ipAddress);
        system(cm);
        snprintf(cm, sizeof(cm), "cm set_val WAN1 ipmask %s", netMask);
        system(cm);
        system("reboot");
    } else {
        char uci[128];
        snprintf(uci, sizeof(uci), "uci set network.lan.ipaddr=%s", ipAddress);
        system(uci);
        snprintf(uci, sizeof(uci), "uci set network.lan.gateway=%s", gateway);
        system(uci);
        snprintf(uci, sizeof(uci), "uci set network.lan.netmask=%s", netMask);
        system(uci);
        system("uci commit network");
        system("/etc/init.d/network reload &");
    }
}

void CUtils_network_set_ip(const char *gateway, const char *ipAddress) {
    if (CUtils_is_ros_platform()) {
        char cm[128];
        snprintf(cm, sizeof(cm), "cm set_val WAN1 ipaddress %s", ipAddress);
        system(cm);
        snprintf(cm, sizeof(cm), "cm set_val WAN1 gateway %s", gateway);
        system(cm);
        system("reboot");
    } else {
        char uci[128];
        snprintf(uci, sizeof(uci), "uci set network.lan.ipaddr=%s", ipAddress);
        system(uci);
        snprintf(uci, sizeof(uci), "uci set network.lan.gateway=%s", gateway);
        system(uci);
        system("uci commit network");
        system("/etc/init.d/network reload");
    }
}

char *CUtils_get_core_version() {
    static char version[64];
    strcpy(version, CUtils_get_res_by_cmd("webs -V"));
    return version;
}

char *CUtils_get_hardware_release_time() {
    static char release_time[64];
    char ver[64];
    char name[64];
    strcpy(ver, CUtils_get_res_by_cmd("webs -V"));
    strcpy(name, CUtils_get_res_by_cmd("uname -a"));
    char *ver_token = strtok(ver, ":");
    char *name_token = strtok(name, " ");
    char *time_token = strtok(NULL, ":");
    snprintf(release_time, sizeof(release_time), "%s_%s_%s", ver_token, name_token, time_token);
    return release_time;
}

void CUtils_heart_beat(const char *path) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "echo $(date +\"%%s\") > %s", path);
    CUtils_cmd_system(cmd);
}

uint16_t CUtils_crc16(const uint8_t *data, uint16_t length) {
    uint16_t crc = 0xffff;
    while (length--) {
        crc ^= *data++;
        for (uint8_t i = 0; i < 8; ++i) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = (crc >> 1);
            }
        }
    }
    return crc;
}

char *CUtils_fmt_float_to_str(double val) {
    static char buf[16];
    snprintf(buf, sizeof(buf), "%.01f", val);
    return buf;
}
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

bool CUtils_is_ros_platform() {
    // 实现判断是否为ROS平台的函数
    return false; // 示例返回值
}

void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void)) {
    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds;
    for (int i = 0; i < repeat; i++) {
        nanosleep(&req, NULL);
        callback();
    }
}

const char *CUtils_get_lan_ip() {
    // 实现获取LAN IP地址的函数
    return "192.168.1.2"; // 示例返回值
}

const char *CUtils_get_lan_netmask() {
    // 实现获取LAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_lan_gateway() {
    // 实现获取LAN网关的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_wan_ip() {
    // 实现获取WAN IP地址的函数
    return "10.0.0.2"; // 示例返回值
}

const char *CUtils_get_wan_netmask() {
    // 实现获取WAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_wan_gateway() {
    // 实现获取WAN网关的函数
    return "10.0.0.1"; // 示例返回值
}

const char *CUtils_get_core_version() {
    // 实现获取核心版本的函数
    return "core_version"; // 示例返回值
}

const char *CUtils_get_addr() {
    // 实现获取IP地址的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_hardware_release_time() {
    // 实现获取硬件发布时间的函数
    return "2023-01-01"; // 示例返回值
}

const char *CUtils_get_res_by_cmd(const char *cmd) {
    // 实现通过命令获取结果的函数
    return "result"; // 示例返回值
}

const char *CUtils_get_netmask() {
    // 实现获取子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_gateway() {
    // 实现获取网关的函数
    return "192.168.1.254"; // 示例返回值
}

unsigned long long CUtils_get_available_Disk(const char *path) {
    // 实现获取磁盘可用空间的函数
    return 10000000; // 示例返回值
}

bool CUtils_get_process_status(const char *process) {
    // 实现获取进程状态的函数
    return false; // 示例返回值
}

bool CUtils_find_dir_file_exists(const char *dir, const char *file) {
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(dir)) == NULL) {
        return false;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, file) == 0) {
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

void CUtils_cmd_system(const char *cmd) {
    system(cmd);
}

uint16_t CUtils_crc16(const uint8_t *data, int len) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void CUtils_change_password(const char *password) {
    // 实现更改密码的函数
}
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

bool CUtils_is_ros_platform() {
    // 实现判断是否为ROS平台的函数
    return false; // 示例返回值
}

void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void)) {
    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds;
    for (int i = 0; i < repeat; i++) {
        nanosleep(&req, NULL);
        callback();
    }
}

const char *CUtils_get_lan_ip() {
    // 实现获取LAN IP地址的函数
    return "192.168.1.2"; // 示例返回值
}

const char *CUtils_get_lan_netmask() {
    // 实现获取LAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_lan_gateway() {
    // 实现获取LAN网关的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_wan_ip() {
    // 实现获取WAN IP地址的函数
    return "10.0.0.2"; // 示例返回值
}

const char *CUtils_get_wan_netmask() {
    // 实现获取WAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_wan_gateway() {
    // 实现获取WAN网关的函数
    return "10.0.0.1"; // 示例返回值
}

const char *CUtils_get_core_version() {
    // 实现获取核心版本的函数
    return "core_version"; // 示例返回值
}

const char *CUtils_get_addr() {
    // 实现获取IP地址的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_hardware_release_time() {
    // 实现获取硬件发布时间的函数
    return "2023-01-01"; // 示例返回值
}

const char *CUtils_get_res_by_cmd(const char *cmd) {
    // 实现通过命令获取结果的函数
    return "result"; // 示例返回值
}

const char *CUtils_get_netmask() {
    // 实现获取子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_gateway() {
    // 实现获取网关的函数
    return "192.168.1.254"; // 示例返回值
}

unsigned long long CUtils_get_available_Disk(const char *path) {
    // 实现获取磁盘可用空间的函数
    return 10000000; // 示例返回值
}

bool CUtils_get_process_status(const char *process) {
    // 实现获取进程状态的函数
    return false; // 示例返回值
}

bool CUtils_find_dir_file_exists(const char *dir, const char *file) {
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(dir)) == NULL) {
        return false;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, file) == 0) {
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

void CUtils_cmd_system(const char *cmd) {
    system(cmd);
}

uint16_t CUtils_crc16(const uint8_t *data, int len) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

int CUtils_get_file_size(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return st.st_size;
    }
    return -1;
}

void CUtils_bit_rate_32_to_48(const char *path) {
    // 实现比特率转换的函数
}

char *CUtils_get_by_cmd_res(const char *cmd) {
    FILE *fp;
    char result[1024];
    fp = popen(cmd, "r");
    if (fp == NULL) {
        return NULL;
    }
    fgets(result, sizeof(result), fp);
    pclose(fp);
    return strdup(result);
}
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

bool CUtils_is_ros_platform() {
    // 实现判断是否为ROS平台的函数
    return false; // 示例返回值
}

void CUtils_async_wait(int seconds, int nanoseconds, int repeat, void (*callback)(void)) {
    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds;
    for (int i = 0; i < repeat; i++) {
        nanosleep(&req, NULL);
        callback();
    }
}

const char *CUtils_get_lan_ip() {
    // 实现获取LAN IP地址的函数
    return "192.168.1.2"; // 示例返回值
}

const char *CUtils_get_lan_netmask() {
    // 实现获取LAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_lan_gateway() {
    // 实现获取LAN网关的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_wan_ip() {
    // 实现获取WAN IP地址的函数
    return "10.0.0.2"; // 示例返回值
}

const char *CUtils_get_wan_netmask() {
    // 实现获取WAN子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_wan_gateway() {
    // 实现获取WAN网关的函数
    return "10.0.0.1"; // 示例返回值
}

const char *CUtils_get_core_version() {
    // 实现获取核心版本的函数
    return "core_version"; // 示例返回值
}

const char *CUtils_get_addr() {
    // 实现获取IP地址的函数
    return "192.168.1.1"; // 示例返回值
}

const char *CUtils_get_hardware_release_time() {
    // 实现获取硬件发布时间的函数
    return "2023-01-01"; // 示例返回值
}

const char *CUtils_get_res_by_cmd(const char *cmd) {
    // 实现通过命令获取结果的函数
    return "result"; // 示例返回值
}

const char *CUtils_get_netmask() {
    // 实现获取子网掩码的函数
    return "255.255.255.0"; // 示例返回值
}

const char *CUtils_get_gateway() {
    // 实现获取网关的函数
    return "192.168.1.254"; // 示例返回值
}

unsigned long long CUtils_get_available_Disk(const char *path) {
    // 实现获取磁盘可用空间的函数
    return 10000000; // 示例返回值
}

bool CUtils_get_process_status(const char *process) {
    // 实现获取进程状态的函数
    return false; // 示例返回值
}

bool CUtils_find_dir_file_exists(const char *dir, const char *file) {
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(dir)) == NULL) {
        return false;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, file) == 0) {
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

void CUtils_cmd_system(const char *cmd) {
    system(cmd);
}

uint16_t CUtils_crc16(const uint8_t *data, int len) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void CUtils_reboot() {
    system("reboot");
}