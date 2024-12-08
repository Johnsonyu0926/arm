#pragma once

#include <iostream>
#include <chrono>
#include "mosquittopp.h"
#include "mqtt_serviceManage.hpp"
#include "audiocfg.hpp"
#include "public.hpp"
#include "utils.h"
#include "json.hpp"

class MQTT : public mosqpp::mosquittopp {
public:
    explicit MQTT(const char *id) : mosqpp::mosquittopp(id) {}

    explicit MQTT() : mosqpp::mosquittopp() {}

    //连接Mqtt服务器
    void on_connect(int rc) override {
        LOG(INFO) << "on_connect in mqtt, rc = " << rc;
        if (MOSQ_ERR_ERRNO == rc) {
            LOG(INFO) << "mqtt connect err: " << mosqpp::strerror(rc);
            //如果由于任何原因连接失败，在本例中我们不想继续重试，所以断开连接。否则，客户端将尝试重新连接。
            this->disconnect();
        } else if (MOSQ_ERR_SUCCESS == rc) {
            subscribe(nullptr, request_topic.c_str());
            LOG(INFO) << "Subscribe to: " << request_topic;
        }
    }

    void on_connect_with_flags(int rc, int flags) override {
        LOG(INFO) << "on connect with flags return rc: " << rc << ", flags: " << flags;
    }

    //断开Mqtt连接
    void on_disconnect(int rc) override {
        LOG(INFO) << "on_disconnect rc: " << rc;
    }

    //订阅指定topic
    void on_publish(int mid) override {
        //LOG(INFO) << "on_publish mid: " << mid;
    }

    void micRecordUpload(const json& js) {
        CUtils utils;
        utils.async_wait(1, 0, 0, [=] {
            asns::CReQuest<asns::CMicRecordUploadData, asns::CMicRecordUploadResultData> req = js;
            std::string reStr = req.do_fail_success();
            LOG(INFO) << "publish: " << reStr;
            this->publish(nullptr, publish_topic.c_str(), reStr.length(), reStr.c_str());
        });
    }

    //订阅主题接收到消息
    void on_message(const mosquitto_message *message) override {
        bool res = false;
        std::string str = static_cast<char *>(message->payload);
        json js = json::parse(str);
        LOG(INFO) << message->topic << " " << js.dump();
        //检查主题是否与订阅匹配。
        mosqpp::topic_matches_sub(request_topic.c_str(), message->topic, &res);
        if (res) {
            if (js["cmd"].get<std::string>() == "micRecordUpload") {
                micRecordUpload(js);
            } else {
                std::string reStr = ServiceManage::instance().getHandler(js["cmd"].get<std::string>())(js);
                LOG(INFO) << "publish: " << reStr;
                this->publish(nullptr, publish_topic.c_str(), reStr.length(), reStr.c_str());
            }
        } else {
            LOG(INFO) << "request_topic: " << request_topic << ", message topic: " << message->topic << " , not match.";
        }
    }

    //订阅回调函数
    void on_subscribe(int mid, int qos_count, const int *granted_qos) override {
        bool have_subscription = false;
        for (int i = 0; i < qos_count; i++) {
            LOG(INFO) << "on_subscribe: " << i << " granted qos = " << granted_qos[i];
            if (granted_qos[i] <= 2) {
                have_subscription = true;
            }
        }
        if (!have_subscription) {
            LOG(WARNING) << "Error: All subscriptions rejected.";
            this->disconnect();
        }
        LOG(INFO) << "Subscribe mid: " << mid << " qos_count: " << qos_count << " granted_qos: " << *granted_qos;
    }

    //取消订阅
    void on_unsubscribe(int mid) override {
        //mid消息序号ID
        LOG(INFO) << "Unsubscribe: " << mid;
    }

    void on_log(int level, const char *str) override {
        //LOG(INFO) << "level: " << level << " " << str;
    }

    void on_error() override {}

    void heartBeat() {
        CUtils utils;
        utils.async_wait(0, asns::MQTT_HEART_BEAT_TIME, asns::MQTT_HEART_BEAT_TIME, [&] {
            std::string reStr = ServiceManage::instance().heartBeat();
            this->publish(nullptr, publish_topic.c_str(), reStr.length(), reStr.c_str());
            utils.heart_beat("/tmp/audio_server_mqtt_heartbeat.txt");
        });
    }

public:
    std::string request_topic = "IOT/intranet/client/request/";
    std::string publish_topic = "IOT/intranet/server/report/";
};