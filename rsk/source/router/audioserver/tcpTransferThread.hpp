// TcpTransferThread.hpp
#pragma once

#include "public.hpp"
#include "add_column_custom_audio_file.hpp"
#include "utils.h"
#include "Rs485.hpp"
#include "CSThread.hpp"
#include "CSocket.hpp"

#include <fstream>
#include <algorithm>
#include <string_view>

class TcpTransferThread : public CSThread {
public:
    [[nodiscard]] BOOL InitInstance() override {
        CSocket socket;
        if (!socket.Create(TCP)) {
            return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
        }
        int opt = 1;
        setsockopt(socket.m_hSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

        for (int i = m_nPort; i < asns::ENDPORT; ++i) {
            if (socket.Bind(i)) {
                m_nPort = i;
                handleResponse(socket, i);
                break;
            } else if (i == asns::ENDPORT - 1) {
                LOG(INFO) << "Fatal: bind error!";
                return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
            }
        }

        socket.Listen();
        if (!waitForConnection(socket)) {
            return SendFast(asns::OPERATION_FAIL_ERROR, pClient);
        }

        auto pTcp = std::make_unique<CSocket>();
        socket.Accept(pTcp.get());
        LOG(INFO) << "Got the connection: " << pTcp->GetRemoteIp() << ":" << ntohs(pTcp->GetPeerPort());
        do_req(pTcp.get());
        socket.Close();
        return TRUE;
    }

    [[nodiscard]] BOOL ExitInstance() override {
        LOG(INFO) << "Exit tcp transfer thread.";
        return TRUE;
    }

    void SetPort(int nPort) noexcept { m_nPort = nPort; }
    void SetClient(CSocket* pClient) noexcept { this->pClient = pClient; }
    void SetVecStr(const std::vector<std::string>& vecStr) { m_vecStr = vecStr; }

private:
    void handleResponse(const CSocket& socket, int port) {
        std::string res;
        if (std::stoi(m_vecStr[3]) != asns::RECORDEND) {
            res = "01 E1 " + m_vecStr[5] + " " + std::to_string(port);
        } else {
            CUtils utils;
            int file_size = utils.get_file_size(asns::RECORD_PATH);
            res = "01 E1 " + m_vecStr[4] + " " + std::to_string(file_size) + " " + std::to_string(port);
        }
        sendResponse(res);
    }

    [[nodiscard]] bool waitForConnection(const CSocket& socket) const {
        fd_set rset;
        FD_ZERO(&rset);
        FD_SET(socket.m_hSocket, &rset);
        struct timeval timeout = {30, 0};
        int n = select(socket.m_hSocket + 1, &rset, nullptr, nullptr, &timeout);
        if (n <= 0) {
            LOG(INFO) << (n < 0 ? "Fatal: select error!" : "Timeout!");
            return false;
        }
        LOG(INFO) << "Server select n = " << n;
        return true;
    }

    [[nodiscard]] int do_req(CSocket* pTcp) {
        int condition = std::stoi(m_vecStr[3]);
        switch (condition) {
            case asns::RECORDEND:
                return Record(pTcp);
            case asns::AUDIOFILEUPLOAD:
                return FileUpload(pTcp);
            case asns::REMOTEFILEUPGRADE:
                return FileUpgrade(pTcp);
            default:
                return SendFast(asns::NONSUPPORT_ERROR, pClient);
        }
    }

    [[nodiscard]] int Record(CSocket* pTcp) {
        CUtils utils;
        int file_size = utils.get_file_size(asns::RECORD_PATH);
        LOG(INFO) << "Record file size:" << file_size;
        std::ifstream fs(asns::RECORD_PATH, std::ios::binary);
        std::array<char, asns::BUFSIZE> buf;

        while (fs && file_size > 0) {
            fs.read(buf.data(), buf.size());
            std::streamsize count = fs.gcount();
            if (count <= 0) break;
            pTcp->Send(buf.data(), static_cast<int>(count));
            file_size -= static_cast<int>(count);
        }
        LOG(INFO) << "Send ok";
        std::filesystem::remove("/tmp/record.mp3");
        return SendTrue(pClient);
    }

    [[nodiscard]] int FileUpload(CSocket* pTcp) {
        std::string_view name_view = m_vecStr[6];
        auto dot_pos = name_view.find_last_of('.');
        std::string prefix(name_view.substr(0, dot_pos));
        std::string suffix(name_view.substr(dot_pos + 1));
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), [](unsigned char c){ return std::tolower(c); });
        std::string name = prefix + "." + suffix;

        asns::CAudioCfgBusiness cfg;
        std::string path = cfg.getAudioFilePath() + name;
        std::ofstream fs(path, std::ios::binary | std::ios::trunc);

        int file_size = std::stoi(m_vecStr[4]);
        std::array<char, asns::BUFSIZE> buf;

        while (file_size > 0) {
            int len = pTcp->Recv(buf.data(), buf.size());
            if (len > 0) {
                fs.write(buf.data(), len);
                file_size -= len;
            } else {
                fs.close();
                std::filesystem::remove(path);
                return SendFast(asns::RECORD_SIZE_ERROR, pClient);
            }
        }
        fs.close();

        LOG(INFO) << "Begin json";
        CUtils utils;
        utils.bit_rate_32_to_48(path);
        asns::CAddColumnCustomAudioFileBusiness business;
        if (!business.exist(name)) {
            asns::CAddColumnCustomAudioFileData node = {asns::AUDIO_FILE_TYPE, name, utils.get_file_size(path)};
            business.business.push_back(node);
            business.saveJson();
        } else {
            business.updateSize(name, std::stoi(m_vecStr[4]));
        }
        return SendTrue(pClient);
    }

    [[nodiscard]] int FileUpgrade(CSocket* pTcp) {
        CUtils utils;
        std::ofstream fs(asns::FIRMWARE_PATH, std::ios::binary);
        int file_size = std::stoi(m_vecStr[4]);
        std::array<char, asns::BUFSIZE> buf;

        while (file_size > 0) {
            int len = pTcp->Recv(buf.data(), buf.size());
            if (len > 0) {
                fs.write(buf.data(), len);
                file_size -= len;
            } else {
                fs.close();
                return SendFast(asns::TCP_TRANSFER_ERROR, pClient);
            }
        }
        fs.close();
        SendTrue(pClient);
        LOG(INFO) << "Begin up read size:" << utils.get_file_size(asns::FIRMWARE_PATH);
        std::string cmdRes = utils.get_by_cmd_res("webs -U /var/run/SONICCOREV100R001.bin");
        LOG(INFO) << "Cmd res:" << cmdRes;
        if (cmdRes.find("OK") != std::string::npos) {
            utils.reboot();
        } else {
            std::filesystem::remove("/var/run/SONICCOREV100R001.bin");
        }
        return 1;
    }

    void sendResponse(std::string_view res) const {
        if (pClient == nullptr) {
            Rs485::_uart_work(res.data(), res.length());
        } else {
            pClient->Send(res.data(), static_cast<int>(res.length()));
        }
    }

    [[nodiscard]] int SendTrue(CSocket* pClient) const {
        std::string_view res = "01 E1";
        LOG(INFO) << "Return: " << res;
        if (pClient == nullptr) {
            return Rs485::_uart_work(res.data(), res.length());
        }
        return pClient->Send(res.data(), static_cast<int>(res.length()));
    }

    [[nodiscard]] int SendFast(std::string_view err_code, CSocket* pClient) const {
        std::string buf = "01 " + std::string(err_code);
        LOG(INFO) << "Return: " << buf;
        if (pClient == nullptr) {
            return Rs485::_uart_work(buf.c_str(), buf.length());
        }
        return pClient->Send(buf.c_str(), static_cast<int>(buf.length()));
    }

private:
    CSocket* pClient{nullptr};
    int m_nPort{0};
    std::vector<std::string> m_vecStr;
};

//BY GST ARMV8 GCC 13.2