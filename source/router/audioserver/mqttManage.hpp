#pragma once

#include "mqtt.hpp"
#include <thread>
#include "audiocfg.hpp"

class MqttManage {
public:
    MqttManage() {}

    void start() {
        std::cout << "------------------mqtt start----------------------" << std::endl;
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        name = cfg.business[0].devName;
        pwd = cfg.business[0].password;
        server = cfg.business[0].server;
        port = cfg.business[0].port;
        imei = cfg.business[0].serial;

        mosqpp::lib_init();
        MQTT mqtt(name.c_str());
        mqtt.publish_topic += imei;
        mqtt.username_pw_set(name.c_str(), pwd.c_str());
        int rc = mqtt.connect(server.c_str(), port, 10);
        //int rc = mqtt.connect("192.168.10.2", 1883, 10);
        if (MOSQ_ERR_ERRNO == rc) {
            std::cerr << "mqtt connect error: " << mosqpp::strerror(rc) << std::endl;
        } else if (MOSQ_ERR_SUCCESS == rc) {
            std::thread([&] {
                while (true) {
                    mqtt.StartTimer(10);
                }
            }).detach();
            std::string reStr = mqtt.m_serviceManage.boot();
            mqtt.publish(nullptr, mqtt.publish_topic.c_str(), reStr.length(), reStr.c_str());
            //订阅主题
            mqtt.request_topic += imei;
            mqtt.subscribe(nullptr, mqtt.request_topic.c_str());
            //此函数在无限阻塞循环中为您调用 loop（）。但不能在回调中调用它。
            std::cout << "Subscribe to:" << mqtt.request_topic << std::endl;
            mqtt.loop_forever();
        }
        mosqpp::lib_cleanup();
    }

    ~MqttManage() {}

private:
    std::string name;
    std::string server;
    std::string pwd;
    std::string imei;
    int port;
};