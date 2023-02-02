#pragma once

#include "public.hpp"
#include "add_column_custom_audio_file.hpp"
#include "utils.h"
#include <fstream>

class CSThread;

class CSocket;


class TcpTransferThread : public CSThread {
public:
    virtual BOOL InitInstance() {
        CSocket socket;
        if (!socket.Create(TCP)) {
            return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
        }
        int opt = 1;
        setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(opt));
        for (int i = m_nPort; i < asns::ENDPORT; ++i) {
            if (socket.Bind(i)) {
                m_nPort = i;
                if (std::stoi(m_vecStr[3]) != asns::RECORDEND) {
                    std::string res = "01 E1 " + m_vecStr[5] + " " + std::to_string(m_nPort);
                    pClient->Send(res.c_str(), res.length());
                } else {
                    CUtils utils;
                    int file_size = utils.get_file_size(asns::RECORD_PATH);
                    std::string res = "01 E1 " + m_vecStr[4] + " " + std::to_string(file_size) + " " + std::to_string(m_nPort);
                    pClient->Send(res.c_str(), res.length());
                }
                break;
            } else if (i == asns::ENDPORT) {
                DS_TRACE("fatal , bind error!\n");
                return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
            }
        }
        socket.Listen();
        while (1) {
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
                return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
            } else if (n == 0) {
                DS_TRACE("timeout!\n");
                return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
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
    }

    virtual BOOL ExitInstance() {
        if (pClient) {
            delete pClient;
            pClient = NULL;
        }
        return TRUE;
    }

    void SetPort(int nPort) { m_nPort = nPort; }

    void SetClient(CSocket *pClient) { this->pClient = pClient; }

    void SetVecStr(const std::vector <std::string> &vecStr) { m_vecStr = vecStr; }

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
                SendFast("F4", pClient);
                break;
        }
        return 1;
    }

    int Record(CSocket *pTcp) {
        CUtils utils;
        int file_size = utils.get_file_size(asns::RECORD_PATH);
        char buf[asns::BUFSIZE] = {0};
        std::fstream fs(asns::RECORD_PATH, std::fstream::in | std::fstream::binary);
        while (!fs.eof()) {
            fs.read(buf, sizeof(buf));
            std::cout << sizeof(buf) << " ";
            pTcp->Send(buf, fs.gcount());
            file_size -= fs.gcount();
            if (file_size <= 0) {
                break;
            }
        }
        fs.close();
        system("rm /tmp/record.mp3");
        return SendTrue(pClient);
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
                return SendFast("F5", pClient);
            }
        }
        fs.close();
        std::cout << "begin json" << std::endl;
        CUtils utils;
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
        return SendTrue(pClient);
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
                return SendFast("F5", pClient);
            }
        }
        fs.close();
        std::cout << "begin up read size:" << utils.get_file_size(asns::FIRMWARE_PATH) << std::endl;
        std::string cmdRes = utils.get_by_cmd_res("webs -U /var/run/SONICCOREV100R001.bin");
        std::cout << "cmd res:" << cmdRes << std::endl;
        if (cmdRes.find("OK") != std::string::npos) {
            SendTrue(pClient);
            system("reboot");
        } else {
            system("rm /var/run/SONICCOREV100R001.bin");
            return SendFast("F5", pClient);
        }
    }

    int SendTrue(CSocket *pClient) {
        std::string res = "01 E1";
        std::cout << "return: " << res << std::endl;
        return pClient->Send(res.c_str(), res.length());
    }

    int SendFast(const std::string &err_code, CSocket *pClient) {
        std::string buf = "01 " + err_code;
        std::cout << "return: " << buf << std::endl;
        pClient->Send(buf.c_str(), buf.length());
        return 0;
    }

private:
    CSocket *pClient;
    int m_nPort;
    std::vector <std::string> m_vecStr;
};