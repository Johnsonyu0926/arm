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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <chrono>
#include <thread>

#ifndef __CUTILS_H__
#define __CUTILS_H__

class CUtils {
private:
    char m_lan[1024];

public:

    size_t get_file_size(const std::string &path) {
        int fd = open(path.c_str(), O_RDWR);
        if (fd < 0) {
            printf("open fail %s!\n", path.c_str());
            return -1;
        }
        struct stat st;
        fstat(fd, &st);
        close(fd);
        return st.st_size;
    }

    void get_dir_file_names(const std::string &path, std::vector <std::string> &files) {
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

    bool find_dir_file_exists(const std::string &path, const std::string &name) {
        std::vector <std::string> files_name;
        get_dir_file_names(path, files_name);
        for (auto iter = files_name.cbegin(); iter != files_name.cend(); ++iter) {
            std::cout << *iter << " ";
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
        std::ifstream i("/mnt/cfg/startup");
        if (!i.is_open()) {
            return 0;
        }
        return 1;
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
        if (is_ros_platform()) {
            get_ros_gateway();
        } else {
            char cmd[64] = {0};
            strcpy(cmd, "uci get network.lan.gateway");
            get_addr_by_cmd(cmd);
        }
        printf("gateway = %s\n", m_lan);
        return m_lan;
    }

    char *get_lan_netmask() {
        if (is_ros_platform()) {
            get_ros_netmask();
        } else {
            char cmd[64] = {};
            strcpy(cmd, "uci get network.lan.netmask");
            get_addr_by_cmd(cmd);
        }
        printf("netmask = %s\n", m_lan);
        return m_lan;
    }

    char *get_lan_addr() {
        if (is_ros_platform()) {
            get_ros_addr();
        } else {
            char cmd[64] = {0};
            strcpy(cmd, "uci get network.lan.ipaddr");
            get_addr_by_cmd(cmd);
        }
        printf("address = %s\n", m_lan);
        return m_lan;
    }

    std::string get_doupload_result(const std::string url, const std::string &imei) {
        char cmd[1024] = {0};
        sprintf(cmd,
                "curl --location --request POST '%s' \\\n"
                "--form 'FormDataUploadFile=@\"/tmp/record.mp3\"' \\\n"
                "--form 'imei=\"%s\"'", url.c_str(), imei.c_str());
        std::cout << "cmd:" << cmd << std::endl;
        return get_by_cmd_res(cmd);
    }

    pid_t get_process_id(const std::string &name) {
        char cmd[128] = {0};
        sprintf(cmd, "ps -ef | grep %s | grep -v grep | awk '{print $2}'", name.c_str());
        return std::atoi(get_by_cmd_res(cmd).c_str());
    }

    std::string get_by_cmd_res(char *cmd) {
        FILE *fp = nullptr;
        char buf[1024];
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
        std::cout << cmd << std::endl;
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
        sprintf(full, "%s%s", prefix, filename);

        printf("full name:%s\n", full);
        int fd = open(full, O_RDWR);
        if (fd < 0) {
            printf("open fail %s!\n", full);
            return -1;
        }
        struct stat st;
        fstat(fd, &st);
        close(fd);
        return st.st_size;
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

    std::string hex_to_string(const std::string &str) {
        std::string result;
        //十六进制两个字符为原始字符一个字符
        for (size_t i = 0; i < str.length(); i += 2) {
            //每次切两个字符
            std::string byte = str.substr(i, 2);
            //将十六进制的string转成long再强转成int再转成char
            result.push_back(
                    static_cast<char>(static_cast<int>(std::strtol(byte.c_str(), nullptr, 16))));//将处理完的字符压入result中
        }
        return result;
    }

    void udp_multicast_send(const std::string &ip, uint16_t port, const std::string &msg) {
        // 1. 创建通信的套接字
        int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (fd == -1) {
            perror("socket");
        }
        // 2. 设置组播属性
        struct in_addr opt;
        // 将组播地址初始化到这个结构体成员中即可
        inet_pton(AF_INET, ip.c_str(), &opt.s_addr);
        setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &opt, sizeof(opt));

        struct sockaddr_in cliaddr;
        cliaddr.sin_family = AF_INET;
        cliaddr.sin_port = htons(port);
        // 发送组播消息, 需要使用组播地址, 和设置组播属性使用的组播地址一致就可以
        inet_pton(AF_INET, ip.c_str(), &cliaddr.sin_addr.s_addr);
        sendto(fd, msg.c_str(), msg.length(), 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
        printf("%s %d udp_multicast_send: %s\n", ip.c_str(), port, msg.c_str());
        close(fd);
    }

    /**
     * 异步定时任务
     * @tparam callable
     * @tparam arguments
     * @param count 执行次数, 0 循环执行
     * @param after 定时执行, 单位秒，过after秒后执行 0 立即执行
     * @param interval 间隔时间, 单位秒, 每次执行间隔多长  0 不间隔
     * @param f
     * @param args
     */
    template<typename callable, class... arguments>
    void async_wait(const size_t count, const size_t after, const size_t interval, callable &&f, arguments &&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task
                (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        std::thread([after, task, count, interval]() {
            auto begin = std::chrono::high_resolution_clock::now();
            if (count == 0) {
                while (true) {
                    auto diff = std::chrono::duration_cast<std::chrono::seconds>
                            (std::chrono::high_resolution_clock::now() - begin).count();
                    if (diff >= after) {
                        task();
                        auto beg = std::chrono::system_clock::now();
                        while (true) {
                            auto end = std::chrono::duration_cast<std::chrono::seconds>
                                    (std::chrono::system_clock::now() - beg).count();
                            if (end >= interval) {
                                break;
                            }
                        }
                    }
                }
            } else {
                while (true) {
                    auto diff = std::chrono::duration_cast<std::chrono::seconds>
                            (std::chrono::high_resolution_clock::now() - begin).count();
                    if (diff >= after) {
                        for (size_t i = 0; i < count; ++i) {
                            task();
                            auto beg = std::chrono::system_clock::now();
                            while (true) {
                                auto end = std::chrono::duration_cast<std::chrono::seconds>
                                        (std::chrono::system_clock::now() - beg).count();
                                if (end >= interval) {
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }).detach();
    }

    /**
    * 字符串切片
    * @param str_v
    * @param del_ims
    * @return
    */
    std::vector <std::string> string_split(std::string str_v, std::string del_ims = " ") {
        std::vector <std::string> output;
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

	int change_password(const char *password) {
		if (is_ros_platform()) {
			char buf[64] = {0};
			snprintf(buf, sizeof(buf), "webs -P %s", password);
			system(buf);
			snprintf(buf, sizeof(buf), "cm set_val sys serverpassword %s", password);
			system(buf);
		}
	}
};

#endif
