#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef __CUTILS_H__
#define __CUTILS_H__

class CUtils {
private:
    char m_lan[1024];

public:
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
        strcpy(cmd, "cm get_val VLAN1 ipaddress|tail -1");
        get_addr_by_cmd(cmd);
    }

    char *get_ros_gateway() {
        char cmd[64] = {0};
        strcpy(cmd, "cm get_val VLAN1 gateway|tail -1");
        get_addr_by_cmd(cmd);
        return m_lan;
    }

    char *get_ros_netmask() {
        char cmd[64] = {0};
        strcpy(cmd, "cm get_val VLAN1 ipmask|tail -1");
        get_addr_by_cmd(cmd);
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
        get_addr_by_cmd(cmd);
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

    char *get_addr_by_cmd(char *cmd) {
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
};

#endif
