// mqtt.hpp
#pragma once

#include <iostream>
#include <chrono>
#include <string_view>
#include <functional>
#include "mosquittopp.h"
#include "mqtt_serviceManage.hpp"
#include "audiocfg.hpp"
#include "public.hpp"
#include "utils.h"

class MQTT : public mosqpp::mosquittopp {
public:
    explicit MQTT(const char* id) : mosqpp::mosquittopp(id) {}
    MQTT() : mosqpp::mosquittopp() {}

    void on_connect(int rc) override {
        LOG(INFO) << "on_connect in mqtt, rc = " << rc;
        if (rc == MOSQ_ERR_ERRNO) {
            LOG(INFO) << "mqtt connect err:" << mosqpp::strerror(rc);
            this->disconnect();
        } else if (rc == MOSQ_ERR_SUCCESS) {
            subscribe(nullptr, request_topic.c_str());
            LOG(INFO) << "Subscribe to:" << request_topic;
        }
    }

    void on_connect_with_flags(int rc, int flags) override {
        LOG(INFO) << "on_connect with flags return rc: " << rc << ", flags: " << flags;
    }

    void on_disconnect(int rc) override {
        LOG(INFO) << "on_disconnect rc:" << rc;
    }

    void on_publish(int mid) override {}

    void micRecordUpload(const nlohmann::json& js) {
        CUtils utils;
        utils.async_wait(1, 0, 0, [this, js] {
            asns::CReQuest<asns::CMicRecordUploadData, asns::CMicRecordUploadResultData> req = js;
            std::string reStr = req.do_fail_success();
            LOG(INFO) << "publish: " << reStr;
            publish(nullptr, publish_topic.c_str(), static_cast<int>(reStr.length()), reStr.c_str());
        });
    }

    void on_message(const mosquitto_message* message) override {
        std::string_view payload(static_cast<const char*>(message->payload), message->payloadlen);
        nlohmann::json js = nlohmann::json::parse(payload);
        LOG(INFO) << message->topic << " " << js.dump();

        bool res = false;
        mosqpp::topic_matches_sub(request_topic.c_str(), message->topic, &res);
        if (res) {
            std::string_view cmd = js["cmd"].get<std::string_view>();
            if (cmd == "micRecordUpload") {
                micRecordUpload(js);
            } else {
                std::string reStr = ServiceManage::instance().getHandler(std::string(cmd))(js);
                LOG(INFO) << "publish: " << reStr;
                publish(nullptr, publish_topic.c_str(), static_cast<int>(reStr.length()), reStr.c_str());
            }
        } else {
            LOG(INFO) << "request_topic:" << request_topic << ", message topic:" << message->topic << " , not match.";
        }
    }

    void on_subscribe(int mid, int qos_count, const int* granted_qos) override {
        bool have_subscription = false;
        for (int i = 0; i < qos_count; ++i) {
            LOG(INFO) << "on_subscribe: " << i << " granted qos = " << granted_qos[i];
            if (granted_qos[i] <= 2) {
                have_subscription = true;
            }
        }
        if (!have_subscription) {
            LOG(WARNING) << "Error: All subscriptions rejected.";
            this->disconnect();
        }
        LOG(INFO) << "Subscribe mid:" << mid << " qos_count: " << qos_count << " granted_qos:" << *granted_qos;
    }

    void on_unsubscribe(int mid) override {
        LOG(INFO) << "Unsubscribe:" << mid;
    }

    void on_log(int level, const char* str) override {}

    void on_error() override {}

    void heartBeat() {
        CUtils utils;
        utils.async_wait(0, asns::MQTT_HEART_BEAT_TIME, asns::MQTT_HEART_BEAT_TIME, [this, &utils] {
            std::string reStr = ServiceManage::instance().heartBeat();
            publish(nullptr, publish_topic.c_str(), static_cast<int>(reStr.length()), reStr.c_str());
            utils.heart_beat("/tmp/audio_server_mqtt_heartbeat.txt");
        });
    }

public:
    std::string request_topic = "IOT/intranet/client/request/";
    std::string publish_topic = "IOT/intranet/server/report/";
};

//BY GST ARMV8 GCC 13.2