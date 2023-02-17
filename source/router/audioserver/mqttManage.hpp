#pragma once

#include "mqtt.hpp"
#include <thread>
#include "audiocfg.hpp"
#include "volume.hpp"

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
        env = cfg.business[0].env;

        DS_TRACE("env:" << env.c_str() << " imei:" << imei.c_str());
        CVolumeSet volumeSet;
        volumeSet.setVolume(3);
        volumeSet.addj(3);
        volumeSet.saveToJson();

        mosqpp::lib_init();

        MQTT mqtt;
        mqtt.publish_topic += env;
        mqtt.publish_topic += "/";
        mqtt.publish_topic += imei;
        mqtt.username_pw_set(name.c_str(), pwd.c_str());

        DS_TRACE("begin connectting mqtt server :" << server.c_str() << ", port:" << port);
        int rc;
        while (true) {
            rc = mqtt.connect(server.c_str(), port);
            if (MOSQ_ERR_ERRNO == rc) {
                DS_TRACE("mqtt connect error: " << mosqpp::strerror(rc));
                std::this_thread::sleep_for(std::chrono::seconds(10));
            } else if (MOSQ_ERR_SUCCESS == rc) {
                break;
            }
        }
        if (MOSQ_ERR_SUCCESS == rc) {
            std::string reStr = ServiceManage::instance().boot();
            mqtt.publish(nullptr, mqtt.publish_topic.c_str(), reStr.length(), reStr.c_str());
            DS_TRACE("publish_topic:" << mqtt.publish_topic.c_str());
            mqtt.heartBeat();
            //订阅主题
            mqtt.request_topic += env;
            mqtt.request_topic += "/";
            mqtt.request_topic += imei;
            mqtt.subscribe(nullptr, mqtt.request_topic.c_str());
            DS_TRACE("Subscribe to:" << mqtt.request_topic.c_str());
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
