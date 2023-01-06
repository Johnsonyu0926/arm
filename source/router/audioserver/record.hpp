#pragma once

#include "json.hpp"
#include "file_recv.hpp"
#include <thread>

using json = nlohmann::json;

namespace asns {

    class CRecordData {
    public:
        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CRecordData, length, port)

    public:
        size_t length;
        size_t port;
    };

    class CRecordResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRecordResult, cmd, resultId, msg)

        int do_success(size_t len) {
            cmd = "record";
            resultId = 1;
            msg = "record success",
            data.length = len;
            data.port = 50004;
        }

    private:
        std::string cmd;
        int resultId;
        std::string msg;
        CRecordData data;
    };

    class CRecord {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRecord, cmd, seconds)

        int do_req(CSocket *pClient) {
            system("arecord -f cd /tmp/record.mp3");
            std::ifstream fs("/tmp/record.mp3", std::fstream::in | std::fstream::binary);
            std::thread([&] {
                std::this_thread::sleep_for(std::chrono::seconds(seconds));
                system("killall -9 arecord");
                std::streampos begin, end;
                begin = fs.tellg();
                fs.seekg(0, std::ios::end);
                end = fs.tellg();
                CRecordResult result;
                result.do_success(end - begin);
                json js = result;
                std::string res = js.dump();
                pClient->Send(res.c_str(), res.length());
            }).detach();
            Server ser(50004);
            ser.bind();
            ser.listen();
            int connfd = ser.accept();
            char buf[1024] = {0};
            while (!fs.eof()) {
                fs.read(buf, sizeof(buf));
                int len = fs.gcount();
                ser.write(connfd, buf, len);
            }
            fs.close();
        }

    private:
        std::string cmd;
        int seconds;
    };
}