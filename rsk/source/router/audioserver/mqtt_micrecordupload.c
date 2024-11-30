#include "mqtt_micrecordupload.h"

// 创建MqttMicRecordUpload对象
MqttMicRecordUpload* create_mqtt_mic_record_upload(const char* clientId, const char* host, int port) {
    MqttMicRecordUpload* mqttMicRecordUpload = (MqttMicRecordUpload*)malloc(sizeof(MqttMicRecordUpload));
    if (!mqttMicRecordUpload) return NULL;
    mqttMicRecordUpload->clientId = strdup(clientId);
    mqttMicRecordUpload->host = strdup(host);
    mqttMicRecordUpload->port = port;
    mqttMicRecordUpload->mqttClient = NULL;
    return mqttMicRecordUpload;
}

// 销毁MqttMicRecordUpload对象
void destroy_mqtt_mic_record_upload(MqttMicRecordUpload* mqttMicRecordUpload) {
    if (mqttMicRecordUpload) {
        free(mqttMicRecordUpload->clientId);
        free(mqttMicRecordUpload->host);
        free(mqttMicRecordUpload);
    }
}

// 加载麦克风录音上传配置
bool load_mic_record_upload_config(MqttMicRecordUpload* mqttMicRecordUpload, const char* configPath) {
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

    json_t* micRecordConfigJson = json_object_get(json, "micRecordConfig");
    mqttMicRecordUpload->micRecordConfig = *mic_record_upload_from_json(micRecordConfigJson);

    json_decref(json);
    return true;
}

// 保存麦克风录音上传配置
bool save_mic_record_upload_config(const MqttMicRecordUpload* mqttMicRecordUpload, const char* configPath) {
    json_t* json = json_object();
    json_t* micRecordConfigJson = mic_record_upload_to_json(&mqttMicRecordUpload->micRecordConfig);
    json_object_set_new(json, "micRecordConfig", micRecordConfigJson);

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
void initialize_mqtt_client(MqttMicRecordUpload* mqttMicRecordUpload) {
    mqttMicRecordUpload->mqttClient = create_mqtt_client(mqttMicRecordUpload->clientId, mqttMicRecordUpload->host, mqttMicRecordUpload->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttMicRecordUpload* mqttMicRecordUpload, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttMicRecordUpload->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttMicRecordUpload* mqttMicRecordUpload) {
    return connect_mqtt_client(mqttMicRecordUpload->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttMicRecordUpload* mqttMicRecordUpload) {
    disconnect_mqtt_client(mqttMicRecordUpload->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttMicRecordUpload* mqttMicRecordUpload, const char* topic) {
    return subscribe_mqtt_client(mqttMicRecordUpload->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttMicRecordUpload* mqttMicRecordUpload, const char* topic, const char* message) {
    return publish_mqtt_client(mqttMicRecordUpload->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttMicRecordUpload* mqttMicRecordUpload) {
    mqtt_client_loop_forever(mqttMicRecordUpload->mqttClient);
}

// 处理麦克风录音上传命令
json_t* handle_mic_record_upload_command(const MqttMicRecordUpload* mqttMicRecordUpload, const json_t* command) {
    // 这里应该实现实际的麦克风录音上传逻辑
    // 为了示例，我们假设上传总是成功的
    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", "麦克风录音上传成功", 
                     "resultId", 1, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "micRecordUpload", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_object());
}