#include <iostream>
#include <chrono>
#include "mosquittopp.h"
#include "mqtt_serviceManage.hpp"

class MQTT : public mosqpp::mosquittopp {
public:
    explicit MQTT(const char *id) : mosqpp::mosquittopp(id) {}

    explicit MQTT() : mosqpp::mosquittopp() {}

    //连接Mqtt服务器
    void on_connect(int rc) override {
        if (MOSQ_ERR_ERRNO == rc) {
            std::cerr << "mqtt connect err:" << mosqpp::strerror(rc) << std::endl;
            //如果由于任何原因连接失败，在本例中我们不想继续重试，所以断开连接。否则，客户端将尝试重新连接。
            this->disconnect();
        }
        std::cout << "rc:" << rc << std::endl;
    }

    void on_connect_with_flags(int rc, int flags) override {
        //std::cout << "rc:" << rc << " flags " << flags << std::endl;
    }

    //断开Mqtt连接
    void on_disconnect(int rc) override {
        //std::cout << "rc:" << rc << std::endl;
    }

    //订阅指定topic
    void on_publish(int mid) override {
        //std::cout << "mid:" << mid << std::endl;
    }

    //订阅主题接收到消息
    void on_message(const mosquitto_message *message) override {
        bool res = false;
        std::string str = static_cast<char *>(message->payload);
        json js = json::parse(str);
        std::cout << message->topic << " " << js.dump() << std::endl;
        //检查主题是否与订阅匹配。
        mosqpp::topic_matches_sub(request_topic.c_str(), message->topic, &res);
        if (res) {
            std::string reStr = m_serviceManage.m_fn[js["cmd"]](js);
            this->publish(nullptr, publish_topic.c_str(), reStr.length(), reStr.c_str());
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
        std::cout << "Subscribe mid:" << mid << " qos_count: " << qos_count << " granted_qos:" << *granted_qos
                  << std::endl;
    }

    //取消订阅
    void on_unsubscribe(int mid) override {
        //mid消息序号ID
        std::cout << "Unsubscribe:" << mid << std::endl;
    }

    void on_log(int level, const char *str) override {
        //std::cout << "level: " << level << " " << str << std::endl;
    }

    void on_error() override {
        std::cout << "error:" << std::endl;
    }

    void heartBeat() {
        std::string reStr = m_serviceManage.heartBeat();
        this->publish(nullptr, publish_topic.c_str(), reStr.length(), reStr.c_str());
    }

    void StartTimer(const int sec) {
        auto begin = std::chrono::system_clock::now();
        while (true) {
            auto cur = std::chrono::system_clock::now();
            auto end = std::chrono::duration_cast<std::chrono::seconds>(cur - begin).count();
            if (end >= sec) {
                heartBeat();
                break;
            }
        }
    }

public:
    std::string request_topic = "IOT/intranet/client/request/prod/";
    std::string publish_topic = "IOT/intranet/server/report/prod/";
public:
    ServiceManage m_serviceManage;
};