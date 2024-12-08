#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

class MqttConfig {
public:
    std::string get_request_topic() const {
        return request_topic;
    }

    void set_request_topic(const std::string &request_topic) {
        this->request_topic = request_topic;
    }

    std::string get_publish_topic() const {
        return publish_topic;
    }

    void set_publish_topic(const std::string &publish_topic) {
        this->publish_topic = publish_topic;
    }

    int load_file() {
        std::ifstream i(MQTTCONFIG);
        if (!i.is_open()) {
            return 0;
        }
        try {
            json js;
            i >> js;
            request_topic = js.at("request_topic").get<std::string>();
            publish_topic = js.at("publish_topic").get<std::string>();
        } catch (json::parse_error &ex) {
            std::cerr << "parse error at byte " << ex.byte << std::endl;
            i.close();
            return 0;
        }
        i.close();
        return 1;
    }

    void file_update() {
        std::ofstream o(MQTTCONFIG);
        if (!o.is_open()) {
            std::cerr << MQTTCONFIG << " stream open fail!" << std::endl;
            return;
        }
        json js;
        js["request_topic"] = request_topic;
        js["publish_topic"] = publish_topic;
        o << js << std::endl;
        o.close();
    }

private:
    static constexpr const char *MQTTCONFIG = "/mnt/cfg/mqtt.json";

    std::string request_topic = "IOT/intranet/client/request/";
    std::string publish_topic = "IOT/intranet/server/report/";
};