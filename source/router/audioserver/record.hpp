/*
#pragma once

#include "json.hpp"
#include "file_recv.hpp"
#include "utils.h"
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
            msg = "record success";
            data.length = len;
            data.port = 50004;
        }

        int do_fail(const std::string str) {
            cmd = "record";
            resultId = 2;
            msg = str,
            data.length = 0;
            data.port = 0;
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

        }

    private:
        std::string cmd;
        int seconds;
    };
}*/
