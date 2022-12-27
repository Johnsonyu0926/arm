#pragma once

#include "mqtt.hpp"
#include <thread>
#include "mqttcfg.hpp"
#include "volume.hpp"

class MqttManage {
public:
    MqttManage() {}

    void start() {
        std::cout << "------------------mqtt start----------------------" << std::endl;
        asns::CMqttCfgBusiness cfg;
        cfg.load();
        name = cfg.business[0].name;
        pwd = cfg.business[0].password;
        server = cfg.business[0].server;
        port = cfg.business[0].port;
        imei = cfg.business[0].imei;
        env = cfg.business[0].env;


        std::cout << "env:" << env << " imei:" << imei << std::endl;
        CVolumeSet volumeSet;
        volumeSet.setVolume(3);
        volumeSet.addj(3);
        volumeSet.saveToJson();

        mosqpp::lib_init();
        std::cout << "mosqpp lib init. " << std::endl;

        MQTT mqtt;
        mqtt.publish_topic += env;
        mqtt.publish_topic += "/";
        mqtt.publish_topic += imei;
        mqtt.username_pw_set(name.c_str(), pwd.c_str());

        std::cout << "begin connectting mqtt server :" << server << ", port:" << port << std::endl;

        int rc = mqtt.connect(server.c_str(), port, 10);
        if (MOSQ_ERR_ERRNO == rc) {
            std::cerr << "mqtt connect error: " << mosqpp::strerror(rc) << std::endl;
        } else if (MOSQ_ERR_SUCCESS == rc) {
            std::thread([&] {
                while (true) mqtt.StartTimer(10);
            }).detach();
            std::string reStr = mqtt.m_serviceManage.boot();
            mqtt.publish(nullptr, mqtt.publish_topic.c_str(), reStr.length(), reStr.c_str());
            std::cout << "publish_topic:" << mqtt.publish_topic << std::endl;
            //订阅主题
            mqtt.request_topic += env;
            mqtt.request_topic += "/";
            mqtt.request_topic += imei;
            mqtt.subscribe(nullptr, mqtt.request_topic.c_str());
            std::cout << "Subscribe to:" << mqtt.request_topic << std::endl;
            //此函数在无限阻塞循环中为您调用 loop（）。但不能在回调中调用它。
            mqtt.loop_forever();
        }
        mosqpp::lib_cleanup();
    }

    ~MqttManage() {
        mosqpp::lib_cleanup();
    }

private:
    std::string name;
    std::string server;
    std::string pwd;
    std::string imei;
    std::string env;
    int port;
};
