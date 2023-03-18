#include <iostream>
#include <chrono>
#include "mosquittopp.h"
#include "mqtt_serviceManage.hpp"
#include "audiocfg.hpp"
#include "public.hpp"

class MQTT : public mosqpp::mosquittopp {
public:
    explicit MQTT(const char *id) : mosqpp::mosquittopp(id) {}

    explicit MQTT() : mosqpp::mosquittopp() {}

    //连接Mqtt服务器
    void on_connect(int rc) override {
        DS_TRACE("on_connect in mqtt , rc = " << rc);
        if (MOSQ_ERR_ERRNO == rc) {
            DS_TRACE("mqtt connect err:" << mosqpp::strerror(rc));
            //如果由于任何原因连接失败，在本例中我们不想继续重试，所以断开连接。否则，客户端将尝试重新连接。
            this->disconnect();
        } else if (MOSQ_ERR_SUCCESS == rc) {
            subscribe(nullptr, request_topic.c_str());
            DS_TRACE("Subscribe to:" << request_topic.c_str());
        }
    }

    void on_connect_with_flags(int rc, int flags) override {
        DS_TRACE("on connect with flags return rc :" << rc << ", flags:" << flags);
    }

    //断开Mqtt连接
    void on_disconnect(int rc) override {
        DS_TRACE("on_disconnect rc:" << rc);
    }

    //订阅指定topic
    void on_publish(int mid) override {
        DS_TRACE("on_publish mid:" << mid);
    }

    //订阅主题接收到消息
    void on_message(const mosquitto_message *message) override {
        bool res = false;
        std::string str = static_cast<char *>(message->payload);
        json js = json::parse(str);
        DS_TRACE(message->topic << " " << js.dump().c_str());
        //检查主题是否与订阅匹配。
        mosqpp::topic_matches_sub(request_topic.c_str(), message->topic, &res);
        if (res) {
            std::string reStr = ServiceManage::instance().getHandler(js["cmd"].get<std::string>())(js);
            this->publish(nullptr, publish_topic.c_str(), reStr.length(), reStr.c_str());
        } else {
            DS_TRACE("request_topic:" << request_topic.c_str() << ", message topic:" << message->topic << " , not match.");
        }
    }

    //订阅回调函数
    void on_subscribe(int mid, int qos_count, const int *granted_qos) override {
        bool have_subscription = false;
        for (int i = 0; i < qos_count; i++) {
            printf("on_subscribe: %d:granted qos = %d\n", i, granted_qos[i]);
            if (granted_qos[i] <= 2) {
                have_subscription = true;
            }
        }
        if (!have_subscription) {
            fprintf(stderr, "Error: All subscriptions rejected.\n");
            this->disconnect();
        }
        DS_TRACE("Subscribe mid:" << mid << " qos_count: " << qos_count << " granted_qos:" << *granted_qos);
    }

    //取消订阅
    void on_unsubscribe(int mid) override {
        //mid消息序号ID
        DS_TRACE("Unsubscribe:" << mid);
    }

    void on_log(int level, const char *str) override {
        DS_TRACE("level: " << level << " " << str);
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
