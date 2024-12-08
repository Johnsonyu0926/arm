//mqttmanage.hpp
#pragma once

#include "mqtt.hpp"
#include <thread>
#include "audiocfg.hpp"
#include "MqttConfig.hpp"

class MqttManage {
public:
    MqttManage() = default;

    void start() {
        LOG(INFO) << "------------------mqtt start----------------------";
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        name = cfg.business[0].devName;
        pwd = cfg.business[0].password;
        server = cfg.business[0].server;
        port = cfg.business[0].port;
        imei = cfg.business[0].serial;
        env = cfg.business[0].env;

        LOG(INFO) << "env:" << env.c_str() << " imei:" << imei;

        mosqpp::lib_init();

        MQTT mqtt;
        MqttConfig mqtt_config;
        if (mqtt_config.load_file()) {
            mqtt.publish_topic = mqtt_config.get_publish_topic();
            mqtt.request_topic = mqtt_config.get_request_topic();
        }
        mqtt.publish_topic += env;
        mqtt.publish_topic += "/";
        mqtt.publish_topic += imei;
        mqtt.username_pw_set(name.c_str(), pwd.c_str());

        LOG(INFO) << "begin connectting mqtt server :" << server << ", port:" << port;
        int rc;
        while (true) {
            rc = mqtt.connect(server.c_str(), port);
            if (MOSQ_ERR_ERRNO == rc) {
                LOG(INFO) << "mqtt connect error: " << mosqpp::strerror(rc);
                std::this_thread::sleep_for(std::chrono::seconds(10));
            } else if (MOSQ_ERR_SUCCESS == rc) {
                break;
            }
        }
        if (MOSQ_ERR_SUCCESS == rc) {
            mqtt.request_topic += env;
            mqtt.request_topic += "/";
            asns::REQUEST_TOPIC = mqtt.request_topic;
            mqtt.request_topic += imei;
            std::string reStr = ServiceManage::instance().boot();
            mqtt.publish(nullptr, mqtt.publish_topic.c_str(), reStr.length(), reStr.c_str());
            LOG(INFO) << "publish_topic:" << mqtt.publish_topic;
            mqtt.heartBeat();
            //订阅主题
            mqtt.subscribe(nullptr, mqtt.request_topic.c_str());
            LOG(INFO) << "Subscribe to:" << mqtt.request_topic;
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
