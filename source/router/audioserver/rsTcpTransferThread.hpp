#pragma once

#include "public.hpp"
#include "add_column_custom_audio_file.hpp"
#include "utils.h"
#include <fstream>

class CSThread;

class CSocket;

static int g_rs485 = -1;
static int g_rsTty = 1;
const int MAX_SEND = 12;

class RsTcpTransferThread : public CSThread {
public:
    virtual BOOL InitInstance() {
        CSocket socket;
        if (!socket.Create(TCP)) {
            return SendFast(asns::OPERATION_FAIL_ERROR);
        }
        int opt = 1;
        setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(opt));
        for (int i = m_nPort; i < asns::ENDPORT; ++i) {
            if (socket.Bind(i)) {
                m_nPort = i;
                if (std::stoi(m_vecStr[3]) != asns::RECORDEND) {
                    std::string res = "01 E1 " + m_vecStr[5] + " " + std::to_string(m_nPort);
                    _uart_work(res.c_str(), res.length());
                } else {
                    CUtils utils;
                    int file_size = utils.get_file_size(asns::RECORD_PATH);
                    std::string res = "01 E1 " + m_vecStr[4] + " " + std::to_string(file_size) + " " + std::to_string(m_nPort);
                    _uart_work(res.c_str(), res.length());
                }
                break;
            } else if (i == asns::ENDPORT) {
                DS_TRACE("fatal , bind error!\n");
                return SendFast(asns::OPERATION_FAIL_ERROR);
            }
        }
        socket.Listen();
        CSocket *pTcp = new CSocket;
        fd_set rset;
        FD_ZERO(&rset);
        FD_SET(socket.m_hSocket, &rset);
        struct timeval timeout;
        timeout.tv_sec = 30;
        timeout.tv_usec = 0;
        int n = select(socket.m_hSocket + 1, &rset, NULL, NULL, &timeout);
        if (n < 0) {
            DS_TRACE("fatal , select error!\n");
            return SendFast(asns::OPERATION_FAIL_ERROR);
        } else if (n == 0) {
            DS_TRACE("timeout!\n");
            return SendFast(asns::TCP_TIMEOUT);
        } else if (n > 0) {
            DS_TRACE("server select n = " << n);
        }
        socket.Accept(pTcp);
        DS_TRACE("Got the no." << " connection :" << pTcp->GetRemoteIp() << ":" << ntohs(pTcp->GetPeerPort()));
        do_req(pTcp);
        socket.Close();
        delete pTcp;
        std::cout << "task end" << std::endl;
    }

    virtual BOOL ExitInstance() {
        std::cout << "exit tcp transfer thread." << std::endl;
        return TRUE;
    }

    void SetPort(int nPort) { m_nPort = nPort; }

    void SetVecStr(const std::vector<std::string> &vecStr) { m_vecStr = vecStr; }

private:
    int do_req(CSocket *pTcp) {
        int condition = std::stoi(m_vecStr[3]);
        switch (condition) {
            case asns::RECORDEND:
                std::cout << "RecordEnd" << std::endl;
                Record(pTcp);
                break;
            case asns::AUDIOFILEUPLOAD:
                std::cout << "AudioFileUpload" << std::endl;
                FileUpload(pTcp);
                break;
            case asns::REMOTEFILEUPGRADE:
                std::cout << "RemoteFileUpgrade" << std::endl;
                FileUpgrade(pTcp);
                break;
            default:
                std::cout << "switch F4" << std::endl;
                SendFast(asns::NONSUPPORT_ERROR);
                break;
        }
        return 1;
    }

    int Record(CSocket *pTcp) {
        CUtils utils;
        int file_size = utils.get_file_size(asns::RECORD_PATH);
        std::cout << "record file size:" << file_size << std::endl;
        char buf[asns::BUFSIZE] = {0};
        std::fstream fs(asns::RECORD_PATH, std::fstream::in | std::fstream::binary);
        while (!fs.eof()) {
            fs.read(buf, sizeof(buf));
            std::cout << fs.gcount() << " ";
            if (fs.gcount() <= 0) {
                std::cout << "read count < 0" << std::endl;
                break;
            }
            pTcp->Send(buf, fs.gcount());
            file_size -= fs.gcount();
            if (file_size <= 0) {
                std::cout << "file size < 0" << std::endl;
                break;
            }
        }
        fs.close();
        std::cout << "Send ok" << std::endl;
        system("rm /tmp/record.mp3");
        return SendTrue();
    }

    int FileUpload(CSocket *pTcp) {
        std::string prefix = m_vecStr[6].substr(0, m_vecStr[6].find_first_of('.'));
        std::string suffix = m_vecStr[6].substr(m_vecStr[6].find_first_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
        std::string name = prefix + "." + suffix;
        asns::CAudioCfgBusiness cfg;
        std::string path = cfg.getAudioFilePath() + name;
        std::fstream fs(path, std::fstream::out | std::fstream::binary | std::fstream::trunc);

        int file_size = std::atoi(m_vecStr[4].c_str());
        char buf[asns::BUFSIZE] = {0};

        while (true) {
            int len = pTcp->Recv(buf, sizeof(buf));
            std::cout << "recv len :" << len << std::endl;
            if (len > 0) {
                fs.write(buf, len);
                file_size -= len;
                if (file_size <= 0) {
                    std::cout << "read end" << std::endl;
                    break;
                }
            } else if (len == 0) {
                std::cout << "read end" << std::endl;
                break;
            } else if (len < 0) {
                fs.close();
                std::string cmd = "rm " + path;
                system(cmd.c_str());
                return SendFast(asns::RECORD_SIZE_ERROR);
            }
        }
        fs.close();
        std::cout << "begin json" << std::endl;
        CUtils utils;
        utils.bit_rate_32_to_48(path);
        asns::CAddColumnCustomAudioFileBusiness business;
        if (!business.exist(name)) {
            asns::CAddColumnCustomAudioFileData node;
            node.type = asns::AUDIO_FILE_TYPE;
            node.setName(name);
            node.size = utils.get_file_size(path);
            business.business.push_back(node);
            business.saveJson();
        } else {
            business.updateSize(name, std::atoi(m_vecStr[4].c_str()));
        }
        return SendTrue();
    }

    int FileUpgrade(CSocket *pTcp) {
        CUtils utils;
        std::fstream fs(asns::FIRMWARE_PATH, std::fstream::out | std::fstream::binary);
        int file_size = std::atoi(m_vecStr[4].c_str());
        char buf[asns::BUFSIZE] = {0};
        while (true) {
            int len = pTcp->Recv(buf, sizeof(buf));
            if (len > 0) {
                fs.write(buf, len);
                std::cout << len << " ";
                file_size -= len;
                if (file_size <= 0)break;
            } else if (len == 0) {
                break;
            } else if (len < 0) {
                fs.close();
                return SendFast(asns::TCP_TRANSFER_ERROR);
            }
        }
        fs.close();
        std::cout << "begin up read size:" << utils.get_file_size(asns::FIRMWARE_PATH) << std::endl;
        std::string cmdRes = utils.get_by_cmd_res("webs -U /var/run/SONICCOREV100R001.bin");
        std::cout << "cmd res:" << cmdRes << std::endl;
        if (cmdRes.find("OK") != std::string::npos) {
            SendTrue();
            system("reboot");
        } else {
            system("rm /var/run/SONICCOREV100R001.bin");
            return SendFast(asns::TCP_UPGRADE_ERROR);
        }
    }

    static int _uart_write(const char *pcData, int iLen) {
        int iFd = g_rs485;
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
                g_rs485 = -1;
                printf("error write %d , len:%d\n", iFd, len);
            } else {
                printf("no.%d : write len:%d success, iRet:%d\n", i, len, iRet);
            }

            offset += MAX_SEND;

            usleep(100 * 1000);
        }
        return iRet;
    }
    static int _uart_open(void) {
        int iFd = -1;
        struct termios opt;
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        int iBdVal = cfg.business[0].iBdVal;

        system("echo 3 > /sys/class/gpio/export");
        system("echo out > /sys/class/gpio/gpio3/direction");

        char name[32];
        sprintf(name, "/dev/ttyS%d", g_rsTty);

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
        sprintf(cmd, "stty -F /dev/ttyS%d %d", g_rsTty, iBdVal);
        system(cmd);

        return iFd;
    }
    void set_send_dir() {
        system("echo 1 > /sys/class/gpio/gpio3/value");
        printf("set send dir! gpio is 1\n");
    }

    void set_receive_dir() {
        system("echo 0 > /sys/class/gpio/gpio3/value");
        printf("set receive dir! gpio is 0\n");
    }
    int _uart_work(const char *buf, int len) {
        int fd = _uart_open();
        if (fd < 0) {
            printf("failed to open ttyS%d to read write.\n", g_rsTty);
            return 2;
        }
        g_rs485 = fd;
        set_send_dir();
        _uart_write(buf, len);
        set_receive_dir();
        return 1;
    }

    int SendTrue() {
        std::string res = "01 E1";
        return _uart_work(res.c_str(), res.length());
    }

    int SendFast(const char *err_code) {
        std::string code = err_code;
        std::string buf = "01 " + code;
        _uart_work(buf.c_str(), buf.length());
        return 0;
    }

private:
    int m_nPort;
    std::vector<std::string> m_vecStr;
};