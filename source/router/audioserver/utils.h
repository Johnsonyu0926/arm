#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/statfs.h>
#include <string>
#include <stdio.h>
#include <dirent.h>
#include <cstring>

#ifndef __CUTILS_H__
#define __CUTILS_H__

class CUtils {
private:
    char m_lan[1024];

public:
    void get_dir_file_names(std::string &path, std::vector <std::string> &files) {
        DIR *pDir;
        dirent *ptr;
        if (!(pDir = opendir(path.c_str()))) return;
        while ((ptr = readdir(pDir)) != 0) {
            if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
                files.push_back(ptr->d_name);
            }
        }
        closedir(pDir);
    }

    bool find_dir_file_exists(std::string path, std::string &name) {
        std::vector <std::string> files_name;
        get_dir_file_names(path, files_name);
        for (auto iter = files_name.cbegin(); iter != files_name.cend(); ++iter) {
            std::cout << "fileList: " << *iter << " ";
            if (name == *iter) {
                return true;
            }
        }
        return false;
    }

    unsigned long long get_available_Disk(const std::string &path) {
        struct statfs diskInfo;
        statfs(path.c_str(), &diskInfo);

        unsigned long long block_size = diskInfo.f_bsize;                       //每个block里包含的字节数
        unsigned long long total_size = block_size * diskInfo.f_blocks;         //总的字节数，f_blocks为block的数目
        unsigned long long free_disk = diskInfo.f_bfree * block_size;           //剩余空间的大小
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
    int get_process_status(char *cmd) {
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
            fclose(fp);
        }
        return exist;
    }

    int is_ros_platform() {
        get_ros_addr();
        if (strlen(m_lan) > 0) {
            return 1;
        }
        return 0;
    }

    char *get_ros_addr() {
        char cmd[64] = {0};
        strcpy(cmd, "cm get_val WAN1 ipaddress|tail -1");
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_ros_gateway() {
        char cmd[64] = {0};
        strcpy(cmd, "cm get_val WAN1 gateway|tail -1");
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_ros_netmask() {
        char cmd[64] = {0};
        strcpy(cmd, "cm get_val WAN1 ipmask|tail -1");
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_lan_gateway() {
        char cmd[64] = {0};
        strcpy(cmd, "uci get network.lan.gateway");
        get_addr_by_cmd(cmd);
        if (strlen(m_lan) == 0) {
            get_ros_gateway();
        }
        printf("gateway = %s\n", m_lan);
        return m_lan;
    }

    char *get_lan_netmask() {
        char cmd[64] = {};
        strcpy(cmd, "uci get network.lan.netmask");
        get_addr_by_cmd(cmd);
        if (strlen(m_lan) == 0) {
            get_ros_netmask();
        }
        printf("netmask = %s\n", m_lan);
        return m_lan;
    }

    char *get_lan_addr() {
        char cmd[64] = {0};
        strcpy(cmd, "uci get network.lan.ipaddr");
        get_addr_by_cmd(cmd);
        if (strlen(m_lan) == 0) {
            get_ros_addr();
        }
        printf("address = %s\n", m_lan);
        return m_lan;
    }

    char *get_doupload_result(const std::string url, const std::string &imei) {
        char cmd[1024] = {0};
        sprintf(cmd,
                "curl --location --request POST '%s' \\\n"
                "--form 'FormDataUploadFile=@\"/tmp/record.mp3\"' \\\n"
                "--form 'imei=\"%s\"'", url.c_str(), imei.c_str());
        std::cout << "cmd:" << cmd << std::endl;
        get_doupload_by_cmd(cmd);
        return m_lan;
    }

    char *get_doupload_by_cmd(char *cmd) {
        FILE *fp;
        fp = popen(cmd, "r");
        if (fp) {
            int nread = fread(m_lan, 1, sizeof(m_lan), fp);
            if (nread < 0) {
                printf("error\n");
                fclose(fp);
                return m_lan;
            }
        }
        return m_lan;
    }

    //curl是将消息写入到stderr,所以popen获取不到，需要把stderr重定向stdout 2>&1
    char *get_upload_result(const std::string &url, const std::string &path, const std::string &name) {
        char cmd[1024] = {0};
        sprintf(cmd, "curl --location --request GET %s -f --output %s%s 2>&1", url.c_str(), path.c_str(), name.c_str());
        get_doupload_by_cmd(cmd);
        return m_lan;
    }

    char *get_addr_by_cmd(char *cmd) {
        memset(m_lan, 0, sizeof m_lan);
        FILE *fp;
        fp = popen(cmd, "r");
        if (fp) {
            int nread = fread(m_lan, 1, sizeof(m_lan), fp);
            if (nread < 0) {
                printf("error\n");
                fclose(fp);
                return m_lan;
            }

            if (strchr(m_lan, '.')) {
                printf("ok address is: %s\n", m_lan);
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
            fclose(fp);
        }
        return m_lan;
    }

    float get_size(const char *prefix, const char *filename) {

        char full[256];
        sprintf(full, "%s/%s", prefix, filename);

        printf("full name:%s\n", full);
        int fd = open(full, O_RDWR);
        if (fd < 0) {
            printf("open fail %s!\n", full);
            return -1;
        }

        struct stat st;
        fstat(fd, &st);
        close(fd);
        return st.st_size / (1024 * 1024); // MB
    }


    int clean_audio_server_file(const char *prefix) {
        char cmd[128];
        sprintf(cmd, "rm %s/audiodata/*", prefix);
        system(cmd);
        sprintf(cmd, "rm %s/cfg/*.json", prefix);
        system(cmd);
    }

    int openwrt_restore_network() {
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

    int ros_restore_allcfg() {
        system("cm default");
    }
};

#endif
