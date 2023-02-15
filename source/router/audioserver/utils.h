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
#include "public.hpp"
#include "audiocfg.hpp"

static int m_rs485{-1};
static int m_rsTty{1};

class Rs485 {
public:
    const int MAX_SEND = 12;

    void set_send_dir() const {
        system("echo 1 > /sys/class/gpio/gpio3/value");
        printf("set send dir! gpio is 1\n");
    }

    void set_receive_dir() const {
        system("echo 0 > /sys/class/gpio/gpio3/value");
        printf("set receive dir! gpio is 0\n");
    }

    int _uart_read(char *pcBuf, int iBufLen) {
        set_receive_dir();
        int iFd = m_rs485, iLen = 0;
        int i;

        *pcBuf = '\0';
        printf("reading... from fd:%d\n", iFd);
        iLen = read(iFd, pcBuf, iBufLen);
        if (iLen < 0) {
            close(iFd);
            m_rs485 = -1;
            printf("error read from fd %d\n", iFd);
            return iLen;
        }

        //ignore 0x0
        while (iLen == 1 && pcBuf[0] == 0x0) {
            printf("ignore the 0x0 .\n");
            iLen = read(iFd, pcBuf, iBufLen);
        }

        printf("read success: iLen= %d , hex dump:\n", iLen);
        for (i = 0; i < iLen; i++) {
            printf("%02x ", pcBuf[i]);
        }
        printf("\nhex dump end.\n");

        while (1) {
            if ((iLen > 5) &&
                ('B' == pcBuf[iLen - 5]) &&
                ('B' == pcBuf[iLen - 4]) &&
                (' ' == pcBuf[iLen - 3]) &&
                ('E' == pcBuf[iLen - 2]) &&
                ('F' == pcBuf[iLen - 1])) {
                printf("receive completed.\n");
                break;
            }

            printf("continue to read...\n");
            int next = read(iFd, pcBuf + iLen, iBufLen - iLen);
            iLen += next;
            printf("read next :%d, iLen:%d, buf:%s\n", next, iLen, pcBuf);

        }
        printf("total len = %d\n", iLen);

        for (i = 0; i < iLen; i++) {
            printf("%02x ", pcBuf[i]);
        }
        printf("\nhex dump end.\n");
        return iLen;
    }

    int _uart_open(void) {
        int iFd = -1;
        struct termios opt;
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        int iBdVal = cfg.business[0].iBdVal;

        system("echo 3 > /sys/class/gpio/export");
        system("echo out > /sys/class/gpio/gpio3/direction");

        char name[32];
        sprintf(name, "/dev/ttyS%d", m_rsTty);

        iFd = open(name, O_RDWR | O_NOCTTY);  /* 读写方式打开串口 */

        if (iFd < 0) {
            return -1;
        }

        tcgetattr(iFd, &opt);
        cfmakeraw(&opt);

        cfsetispeed(&opt, iBdVal);
        cfsetospeed(&opt, iBdVal);

        tcsetattr(iFd, TCSANOW, &opt);
        char cmd[64];
        sprintf(cmd, "stty -F /dev/ttyS%d %d", m_rsTty, iBdVal);
        system(cmd);
        m_rs485 = iFd;
        return iFd;
    }

    /* Write data to uart dev, return 0 means OK */
    int _uart_write(const char *pcData, int iLen) {
        set_send_dir();
        int iFd = m_rs485;
        int iRet = -1;
        int len = 0;
        printf("to write :%s, len:%d\n", pcData, iLen);
        int count = iLen / MAX_SEND;
        if (iLen % MAX_SEND) {
            count++;
        }
        int offset = 0;
        printf("count=%d\n", count);
        for (int i = 0; i < count; i++) {
            if ((i + 1) * MAX_SEND > iLen) {
                len = iLen - i * MAX_SEND;
            } else {
                len = MAX_SEND;
            }
            printf("no.%d : offset:%d len:%d \n", i, offset, len);
            const char *data = pcData + offset;
            for (int j = 0; j < len; j++) {
                printf("%02x ", data[j]);
            }
            iRet = write(iFd, data, len);
            if (iRet < 0) {
                close(iFd);
                m_rs485 = -1;
                printf("error write %d , len:%d\n", iFd, len);
            } else {
                printf("no.%d : write len:%d success, iRet:%d\n", i, len, iRet);
            }

            offset += MAX_SEND;

            usleep(100 * 1000);
        }
        return iRet;
    }

    int _uart_work(const char *buf, int len) {
        int fd = _uart_open();
        if (fd < 0) {
            printf("failed to open ttyS%d to read write.\n", m_rsTty);
            return 2;
        }
        m_rs485 = fd;
        _uart_write(buf, len);
        set_receive_dir();
        return 1;
    }


    int SendTrue() {
        std::string res = "01 E1";
        return _uart_write(res.c_str(), res.length());
    }

    int SendFast(const std::string &err_code) {
        std::string buf = "01 " + err_code;
        return _uart_write(buf.c_str(), buf.length());
    }
};


class PlayStatus {
public:
    PlayStatus(const PlayStatus &) = delete;

    PlayStatus &operator=(const PlayStatus &) = delete;

    static PlayStatus &getInstance() {
        static PlayStatus instance;
        return instance;
    }

    void init() {
        m_playId = asns::STOP_TASK_PLAYING;
        m_priority = asns::STOP_TASK_PLAYING;
        m_pId = asns::STOP_TASK_PLAYING;
    }

    int getPlayState() const {
        return m_playId == asns::STOP_TASK_PLAYING;
    }

    int getPlayId() const {
        return m_playId;
    }

    void setPlayId(const int id) {
        m_playId = id;
    }

    int getPriority() const {
        return m_priority;
    }

    void setPriority(const int id) {
        m_priority = id;
    }

    pid_t getProcessId() const {
        return m_pId;
    }

    void setProcessId(pid_t id) {
        m_pId = id;
    }

    ~PlayStatus() = default;

private:
    PlayStatus() = default;

private:
    int m_playId{-1};
    int m_priority{-1};
    pid_t m_pId{-1};
};


class CGpio {
public:
    ~CGpio() = default;

    CGpio(const CGpio &) = delete;

    CGpio &operator=(const CGpio &) = delete;

    static CGpio &getInstance() {
        static CGpio instance;
        return instance;
    }

    int getGpioStatus() const {
        return m_gpioStatus;
    }

    void set_gpio_on() {
        system("echo 1 > /sys/class/gpio/gpio16/value");
        m_gpioStatus = 1;
        printf("set send dir! gpio is 1\n");
    }

    void set_gpio_off() {
        system("echo 0 > /sys/class/gpio/gpio16/value");
        m_gpioStatus = 0;
        printf("set receive dir! gpio is 0\n");
    }

    void setGpioModel(const int model) {
        m_gpioModel = model;
    }

    int getGpioModel() const {
        return m_gpioModel;
    }

private:
    CGpio() = default;

private:
    int m_gpioStatus;
    int m_gpioModel;
};

class CUtils {
private:
    char m_lan[1024];

public:

    int uart_write(const std::string &cmd) {
        Rs485 rs;
        return rs._uart_work(cmd.c_str(), cmd.length());
    }

    int get_play_state() const {
        return PlayStatus::getInstance().getPlayState();
    }

    int get_gpio_state() const {
        return CGpio::getInstance().getGpioStatus();
    }

    int get_gpio_model() const {
        return CGpio::getInstance().getGpioModel();
    }

    void set_gpio_on() {
        CGpio::getInstance().set_gpio_on();
    }

    void set_gpio_off() {
        CGpio::getInstance().set_gpio_off();
    }

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

    void get_dir_file_names(const std::string &path, std::vector<std::string> &files) {
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
        std::vector<std::string> files_name;
        get_dir_file_names(path, files_name);
        for (auto iter = files_name.cbegin(); iter != files_name.cend(); ++iter) {
            std::cout << *iter << " ";
            if (name == *iter) {
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

    std::string get_doupload_result(const std::string &url, const std::string &imei) {
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
    std::vector<std::string> string_split(std::string str_v, std::string del_ims = " ") {
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
        //通过字节码进行判断
        return c >= 0x80;
    }

    int statistical_character_count(const std::string &str) {
        std::cout << str.length() << std::endl;
        std::cout << str.size() << std::endl;
        int LowerCase, UpperCase; //大写，小写
        int space = 0;
        int digit, character; //数字，字符
        int chinese = 0; //中文
        digit = character = LowerCase = UpperCase = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] >= 'a' && str[i] <= 'z') {
                std::cout << str[i] << " ";
                LowerCase++;
            } else if (str[i] >= 'A' && str[i] <= 'Z') {
                std::cout << str[i] << " ";
                UpperCase++;
            } else if (str[i] >= '0' && str[i] <= '9') {
                std::cout << str[i] << " ";
                digit++;
            } else if (check(str[i])) {
                std::cout << "chinese" << str[i] << " ";
                chinese++;
            } else if (str[i] == ' ') {
                std::cout << str[i] << " ";
                space++;
            } else {
                std::cout << str[i] << " ";
                character++;
            }
        }
        printf("UpperCase%d，LowerCase%d，digit%d，character%d，chinese%d，space%d\n", UpperCase, LowerCase, digit,
               character,
               chinese / 3, space);
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
    }

    void bit_rate_32_to_48(const std::string &path) {
        std::string cmd = "conv.sh " + path;
        std::cout << "cmd : " << cmd << std::endl;
        system(cmd.c_str());
    }

    void volume_gain(const std::string &path, const std::string &suffix) {
        std::string cmd = "vol.sh " + path + " " + suffix;
        std::cout << "cmd :" << cmd << std::endl;
        system(cmd.c_str());
    }

    void txt_to_audio(const std::string &txt, const int speed = 50) {
        std::string cmd = "tts -t " + txt + " -p " + std::to_string(speed) + " -f /tmp/output.pcm";
        system(cmd.c_str());
        system("ffmpeg -y -f s16le -ar 16000 -ac 1 -i /tmp/output.pcm /tmp/output.wav");
        volume_gain(asns::TTS_PATH, "wav");
    }

    void audio_clear() {
        system("rm /tmp/output.wav");
        system("rm /tmp/output.pcm");
        system("rm /tmp/vol.wav");
    }

    void audio_stop() {
        PlayStatus::getInstance().init();
        system("killall -9 ffplay");
        system("killall -9 ffmpeg");
        system("killall -9 madplay");
        system("killall -9 aplay");
    }

    void tts_loop_play(const bool async = false) {
        if (async) {
            async_wait(1, 0, 0, [&] {
                PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
                while (PlayStatus::getInstance().getPlayId() == asns::AUDIO_TASK_PLAYING) {
                    system("aplay /tmp/output.wav");
                }
            });
        } else {
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            while (PlayStatus::getInstance().getPlayId() == asns::AUDIO_TASK_PLAYING) {
                system("aplay /tmp/output.wav");
            }
        }
    }

    void tts_num_play(const int num, const bool async = false) {
        if (async) {
            async_wait(1, 0, 0, [=] {
                PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
                for (int i = 0; i < num; ++i) {
                    system("aplay /tmp/output.wav");
                }
                PlayStatus::getInstance().init();
            });
        } else {
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            for (int i = 0; i < num; ++i) {
                system("/tmp/output.wav");
            }
            PlayStatus::getInstance().init();
        }
    }

    void tts_time_play(const int time, const bool async = false) {
        async_wait(1, time, 0, [&] {
            system("killall -9 aplay");
            PlayStatus::getInstance().init();
        });
        if (async) {
            async_wait(1, 0, 0, [&] {
                PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
                while (PlayStatus::getInstance().getPlayId() == asns::AUDIO_TASK_PLAYING) {
                    system("aplay /tmp/output.wav");
                }
            });
        } else {
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            while (PlayStatus::getInstance().getPlayId() == asns::AUDIO_TASK_PLAYING) {
                system("aplay /tmp/output.wav");
            }
        }
    }

    void audio_loop_play(const std::string &path, const bool async = false) {
        if (async) {
            async_wait(1, 0, 0, [=] {
                std::string cmd = "madplay " + path + " -r";
                std::cout << cmd << std::endl;
                PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
                system(cmd.c_str());
                PlayStatus::getInstance().init();
            });
        } else {
            std::string cmd = "madplay " + path + " -r";
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            system(cmd.c_str());
            PlayStatus::getInstance().init();
        }
    }

    void audio_num_play(const int num, const std::string &path, const bool async = false) {
        if (async) {
            async_wait(1, 0, 0, [=] {
                std::string cmd = "madplay ";
                for (int i = 0; i < num; ++i) {
                    cmd += path + ' ';
                }
                PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
                std::cout << cmd << std::endl;
                system(cmd.c_str());
                PlayStatus::getInstance().init();
            });
        } else {
            std::string cmd = "madplay ";
            for (int i = 0; i < num; ++i) {
                cmd += path + ' ';
            }
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            std::cout << cmd << std::endl;
            system(cmd.c_str());
            PlayStatus::getInstance().init();
        }

    }

    void audio_time_play(const int time, const std::string &path, const bool async = false) {
        int d = time / (3600 * 24);
        int t = time % (3600 * 24) / 3600;
        int m = time % (3600 * 24) % 3600 / 60;
        int s = time % (3600 * 24) % 3600 % 60;
        char buf[64] = {0};
        char cmd[128] = {0};
        sprintf(buf, "%d:%d:%d:%d", d, t, m, s);
        sprintf(cmd, "madplay %s -r -t %s", path.c_str(), buf);
        if (async) {
            async_wait(1, 0, 0, [=] {
                PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
                std::cout << cmd << std::endl;
                system(cmd);
                PlayStatus::getInstance().init();
            });
        } else {
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            std::cout << cmd << std::endl;
            system(cmd);
            PlayStatus::getInstance().init();
        }
    }

    void reboot() {
        system("reboot");
    }

    void record_start(const bool async = false) {
        if (async) {
            async_wait(1, 0, 0, [&] {
                system("arecord -f cd /tmp/record.mp3");
            });
        } else {
            system("arecord -f cd /tmp/record.mp3");
        }
    }

    void record_stop() {
        system("killall -9 arecord");
    }

    std::string record_upload(const int time, const std::string &url, const std::string &imei) {
        std::future<std::string> res = std::async(std::launch::async, [=] {
            std::this_thread::sleep_for(std::chrono::seconds(time));
            record_stop();
            volume_gain(asns::RECORD_PATH, "mp3");
            std::string res = get_doupload_result(url, imei);
            system("rm /tmp/record.mp3");
            system("rm /tmp/vol.mp3");
            return res;
        });
        async_wait(1, 0, 0, [&] {
            record_start();
        });
        return res.get();
    }

    void gpio_set(const int model, const int status) {
        CGpio::getInstance().setGpioModel(model);
        switch (model) {
            case asns::GPIO_CUSTOM_MODE:
                if (status == 0) {
                    CGpio::getInstance().set_gpio_off();
                } else if (status == 1) {
                    CGpio::getInstance().set_gpio_on();
                }
                break;
            case asns::GPIO_PLAY_MODE:
                async_wait(1, 0, 0, [&] {
                    while (CGpio::getInstance().getGpioModel() == 2) {
                        if (get_process_status("madplay") || get_process_status("aplay")) {
                            CGpio::getInstance().set_gpio_on();
                        } else {
                            CGpio::getInstance().set_gpio_off();
                        }
                    }
                });
                break;
            default:
                break;
        }
    }

    void restore(const std::string &path) {
        if (is_ros_platform()) {
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
            sprintf(cm, "cm set_val WAN1 gateway %s", gateway.c_str());
            system(cm);
            sprintf(cm, "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
            system(cm);
            sprintf(cm, "cm set_val WAN1 ipmask %s", netMask.c_str());
            system(cm);
            system("reboot");
        } else {
            char uci[128] = {0};
            sprintf(uci, "uci set network.lan.ipaddr=%s", ipAddress.c_str());
            system(uci);
            sprintf(uci, "uci set network.lan.gateway=%s", gateway.c_str());
            system(uci);
            sprintf(uci, "uci set network.lan.netmask=%s", netMask.c_str());
            system(uci);
            sprintf(uci, "uci commit network");
            system(uci);
            sprintf(uci, "/etc/init.d/network reload &");
            system(uci);
        }
    }

    void network_set(const std::string &gateway, const std::string &ipAddress) {
        if (is_ros_platform()) {
            char cm[128] = {0};
            sprintf(cm, "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
            system(cm);
            sprintf(cm, "cm set_val WAN1 gateway %s", gateway.c_str());
            system(cm);
            system("reboot");
        } else {
            char uci[128] = {0};
            sprintf(uci, "uci set network.lan.ipaddr=%s", ipAddress.c_str());
            system(uci);
            sprintf(uci, "uci set network.lan.gateway=%s", gateway.c_str());
            system(uci);
            sprintf(uci, "uci commit network");
            system(uci);
            sprintf(uci, "/etc/init.d/network reload");
            system(uci);
        }
    }
};