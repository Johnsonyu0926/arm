#include "mqtt_boot.h"

// 创建MqttBoot对象
MqttBoot* create_mqtt_boot(const char* clientId, const char* host, int port) {
    MqttBoot* mqttBoot = (MqttBoot*)malloc(sizeof(MqttBoot));
    if (!mqttBoot) return NULL;
    mqttBoot->clientId = strdup(clientId);
    mqttBoot->host = strdup(host);
    mqttBoot->port = port;
    mqttBoot->bootConfig = NULL;
    mqttBoot->bootConfigSize = 0;
    mqttBoot->mqttClient = NULL;
    return mqttBoot;
}

// 销毁MqttBoot对象
void destroy_mqtt_boot(MqttBoot* mqttBoot) {
    if (mqttBoot) {
        free(mqttBoot->clientId);
        free(mqttBoot->host);
        free(mqttBoot->bootConfig);
        free(mqttBoot);
    }
}

// 加载MqttBoot配置
bool load_mqtt_boot_config(MqttBoot* mqttBoot, const char* configPath) {
    FILE* file = fopen(configPath, "r");
    if (!file) {
        fprintf(stderr, "Config file does not exist: %s\n", configPath);
        return false;
    }

    json_error_t error;
    json_t* json = json_loadf(file, 0, &error);
    fclose(file);
    if (!json) {
        fprintf(stderr, "Error loading config file: %s\n", error.text);
        return false;
    }

    json_t* bootConfigJson = json_object_get(json, "bootConfig");
    size_t index;
    json_t* value;
    mqttBoot->bootConfigSize = json_array_size(bootConfigJson);
    mqttBoot->bootConfig = (Boot*)malloc(mqttBoot->bootConfigSize * sizeof(Boot));
    json_array_foreach(bootConfigJson, index, value) {
        mqttBoot->bootConfig[index] = boot_from_json(value);
    }

    json_decref(json);
    return true;
}

// 保存MqttBoot配置
bool save_mqtt_boot_config(const MqttBoot* mqttBoot, const char* configPath) {
    json_t* json = json_object();
    json_t* bootConfigJson = json_array();
    for (size_t i = 0; i < mqttBoot->bootConfigSize; ++i) {
        json_array_append_new(bootConfigJson, boot_to_json(&mqttBoot->bootConfig[i]));
    }
    json_object_set_new(json, "bootConfig", bootConfigJson);

    FILE* file = fopen(configPath, "w");
    if (!file) {
        fprintf(stderr, "Failed to open config file for writing: %s\n", configPath);
        json_decref(json);
        return false;
    }

    if (json_dumpf(json, file, JSON_INDENT(4)) != 0) {
        fprintf(stderr, "Error saving config file\n");
        fclose(file);
        json_decref(json);
        return false;
    }

    fclose(file);
    json_decref(json);
    return true;
}

// 初始化MQTT客户端
void initialize_mqtt_client(MqttBoot* mqttBoot) {
    mqttBoot->mqttClient = create_mqtt_client(mqttBoot->clientId, mqttBoot->host, mqttBoot->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttBoot* mqttBoot, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttBoot->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttBoot* mqttBoot) {
    return connect_mqtt_client(mqttBoot->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttBoot* mqttBoot) {
    disconnect_mqtt_client(mqttBoot->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttBoot* mqttBoot, const char* topic) {
    return subscribe_mqtt_client(mqttBoot->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttBoot* mqttBoot, const char* topic, const char* message) {
    return publish_mqtt_client(mqttBoot->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttBoot* mqttBoot) {
    mqtt_client_loop_forever(mqttBoot->mqttClient);
}

// 生成启动报告
json_t* generate_start_report(void) {
    json_t* report = json_pack("{s:i, s:s, s:i, s:s, s:s, s:i, s:s, s:i, s:i, s:i, s:i, s:i, s:s, s:s, s:s, s:s, s:i}",
                               "sdcardSpace", 16225000,
                               "lng", "120.2168731",
                               "flashSpace", 16225000,
                               "project", "0Tr1N2kbP5sKUDglAIOUxHdwp4VkLucz",
                               "iotVersion", "COMMON",
                               "version", "2.0.30",
                               "volume", 4,
                               "iccId", "898604B41022C0341497",
                               "v12", 0,
                               "volt", 971,
                               "v24", 0,
                               "storageType", 1,
                               "imei", "869298057401606",
                               "topic", "TaDiao/device/report/test/869298057401606",
                               "cmd", "start",
                               "lat", "030.2099819",
                               "v5", 0);
    return report;
}

// 发送启动报告
void send_start_report(MqttBoot* mqttBoot) {
    json_t* report = generate_start_report();
    char* message = json_dumps(report, 0);
    const char* topic = json_string_value(json_object_get(report, "topic"));
    publish_mqtt_message(mqttBoot, topic, message);
    free(message);
    json_decref(report);
}