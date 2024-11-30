#include "utils.h"
#include <stdio.h>
#include "timerestart.h"
int main() {
    CUtils utils;
    TimedRestart timedRestart;
    if (TimedRestart_file_load(&timedRestart)) {
        printf("File loaded successfully\n");
    } else {
        printf("Failed to load file\n");
    }

    TimedRestart_testJson(&timedRestart);
    printf("Current time: %s\n", TimedRestart_getCurrTime());

    TimedRestart_loop(&timedRestart);

    return 0;
}
    printf("File size: %d\n", CUtils_get_file_size("/path/to/file"));
    printf("Available disk space: %llu KB\n", CUtils_get_available_Disk("/path/to/disk"));
    printf("Process status: %d\n", CUtils_get_process_status("process_name"));
    printf("Core version: %s\n", CUtils_get_core_version());
    printf("Hardware release time: %s\n", CUtils_get_hardware_release_time());
    printf("CRC16: %04x\n", CUtils_crc16((const uint8_t *)"data", 4));
    printf("Formatted float: %s\n", CUtils_fmt_float_to_str(123.456));

    return 
    // 获取文件大小
    int file_size = CUtils_get_file_size("/path/to/file");
    if (file_size != -1) {
        printf("File size: %d bytes\n", file_size);
    } else {
        printf("Failed to get file size\n");
    }

    // 获取可用磁盘空间
    unsigned long long available_disk = CUtils_get_available_Disk("/path/to/disk");
    printf("Available disk space: %llu KB\n", available_disk);

    // 获取进程状态
    int process_status = CUtils_get_process_status("process_name");
    printf("Process status: %d\n", process_status);

    // 获取核心版本
    char *core_version = CUtils_get_core_version();
    printf("Core version: %s\n", core_version);

    // 获取硬件发布时间
    char *hardware_release_time = CUtils_get_hardware_release_time();
    printf("Hardware release time: %s\n", hardware_release_time);

    // 计算CRC16
    uint16_t crc = CUtils_crc16((const uint8_t *)"data", 4);
    printf("CRC16: %04x\n", crc);

    // 格式化浮点数
    char *formatted_float = CUtils_fmt_float_to_str(123.456);
    printf("Formatted float: %s\n", formatted_float);

    // 获取LAN IP地址
    char *lan_ip = CUtils_get_addr(&utils);
    printf("LAN IP address: %s\n", lan_ip);

    // 获取LAN子网掩码
    char *lan_netmask = CUtils_get_netmask(&utils);
    printf("LAN netmask: %s\n", lan_netmask);

    // 获取LAN网关
    char *lan_gateway = CUtils_get_gateway(&utils);
    printf("LAN gateway: %s\n", lan_gateway);

    // 获取WAN IP地址
    char *wan_ip = CUtils_get_addr(&utils);
    printf("WAN IP address: %s\n", wan_ip);

    // 获取WAN子网掩码
    char *wan_netmask = CUtils_get_netmask(&utils);
    printf("WAN netmask: %s\n", wan_netmask);

    // 获取WAN网关
    char *wan_gateway = CUtils_get_gateway(&utils);
    printf("WAN gateway: %s\n", wan_gateway);

    // 更改密码
    CUtils_change_password("new_password");

    // 录音开始
    CUtils_record_start(false);

    // 录音停止
    CUtils_record_stop();

    // 上传录音
    char *upload_result = CUtils_record_upload(10, "http://example.com/upload", "1234567890");
    printf("Upload result: %s\n", upload_result);

    // 恢复出厂设置
    CUtils_restore("/mnt");

    // 设置网络
    CUtils_network_set("192.168.1.1", "192.168.1.100", "255.255.255.0");

    // 设置网络（仅IP和网关）
    CUtils_network_set_ip("192.168.1.1", "192.168.1.100");

    // 心跳
    CUtils_heart_beat("/path/to/heartbeat");

    return 0;
}