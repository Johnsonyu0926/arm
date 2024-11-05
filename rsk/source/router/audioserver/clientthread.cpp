// clientthread.cpp
#include "clientthread.hpp"
#include <nlohmann/json.hpp>
#include "utils.h"
#include "audiocfg.hpp"
#include "audioPlay.hpp"
#include "audioStop.hpp"
#include "audio_del.hpp"
#include "broadcast_plan.hpp"
#include "MqttConfig.hpp"
#include "Relay.hpp"
#include "Rs485.hpp"
#include "Rs485NoiseMange.hpp"
#include <iostream>
#include <string_view>

bool CClientThread::InitInstance() {
    // Initialization code.
    return true;
}

bool CClientThread::ExitInstance() {
    // Cleanup code.
    return true;
}

bool CClientThread::Check(const unsigned char* szBuf) const {
    // Check the buffer.
    return true;
}

bool CClientThread::Gen(unsigned char* szBuf) const {
    // Generate data for the buffer.
    return true;
}

int CClientThread::do_req(std::string_view buf, std::shared_ptr<CSocket> pClient) {
    try {
        nlohmann::json j = nlohmann::json::parse(buf);
        std::string_view cmd = j.at("cmd").get<std::string_view>();

        if (cmd == "AudioPlay") {
            return j.get<asns::CAudioPlay>().do_req(pClient);
        } else if (cmd == "AudioStop") {
            return j.get<asns::CAudioStop>().do_req(pClient);
        } else if (cmd == "AudioDelete") {
            return j.get<asns::CDeleteAudio>().do_req(pClient);
        } else if (cmd == "BroadcastPlan") {
            return j.get<asns::BroadcastPlan>().do_req(pClient);
        }
        // Add more commands as needed.
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return -1;
    }
    return -1;
}

int CClientThread::do_verify(std::string_view buf) const {
    // Verification code.
    return 0;
}

int CClientThread::do_str_req(std::shared_ptr<CSocket> pClient) {
    // String request handling code.
    return 0;
}

int CClientThread::do_str_verify(std::string_view buf, std::shared_ptr<CSocket> pClient) const {
    // String verification code.
    return 0;
}

//By GST ARMV8 GCC13.2 clientthread.cpp