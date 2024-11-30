#include "mqtt_audiostop.h"

// 创建MqttAudioStop对象
MqttAudioStop* create_mqtt_audio_stop(const char* clientId, const char* host, int port) {
    MqttAudioStop* mqttAudioStop = (MqttAudioStop*)malloc(sizeof(MqttAudioStop));
    if (!mqttAudioStop) return NULL;
    mqttAudioStop->clientId = strdup(clientId);
    mqttAudioStop->host = strdup(host);
    mqttAudioStop->port = port;
    mqttAudioStop->audioConfig = NULL;
    mqttAudioStop->audioConfigCount = 0;
    mqttAudioStop->mqttClient = NULL;
    return mqttAudioStop;
}

// 销毁MqttAudioStop对象
void destroy_mqtt_audio_stop(MqttAudioStop* mqttAudioStop) {
    if (mqttAudioStop) {
        free(mqttAudioStop->clientId);
        free(mqttAudioStop->host);
        free(mqttAudioStop->audioConfig);
        free(mqttAudioStop);
    }
}

// 加载配置
bool load_config(MqttAudioStop* mqttAudioStop, const char* configPath) {
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

    json_t* audioConfigArray = json_object_get(json, "audioConfig");
    size_t index;
    json_t* value;
    json_array_foreach(audioConfigArray, index, value) {
        AudioStop* audioStop = audio_stop_from_json(value);
        mqttAudioStop->audioConfig = (AudioStop*)realloc(mqttAudioStop->audioConfig, (mqttAudioStop->audioConfigCount + 1) * sizeof(AudioStop));
        mqttAudioStop->audioConfig[mqttAudioStop->audioConfigCount++] = *audioStop;
        free(audioStop);
    }

    json_decref(json);
    return true;
}

// 保存配置
bool save_config(const MqttAudioStop* mqttAudioStop, const char* configPath) {
    json_t* json = json_object();
    json_t* audioConfigArray = json_array();
    for (size_t i = 0; i < mqttAudioStop->audioConfigCount; ++i) {
        json_t* audioStopJson = audio_stop_to_json(&mqttAudioStop->audioConfig[i]);
        json_array_append_new(audioConfigArray, audioStopJson);
    }
    json_object_set_new(json, "audioConfig", audioConfigArray);

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

// 初始化客户端
void initialize_client(MqttAudioStop* mqttAudioStop) {
    mqttAudioStop->mqttClient = create_mqtt_client(mqttAudioStop->clientId, mqttAudioStop->host, mqttAudioStop->port);
}

// 设置回调函数
void set_callback(MqttAudioStop* mqttAudioStop, void (*callback)(const char* topic, const char* message)) {
    mqtt_client_set_message_callback(mqttAudioStop->mqttClient, callback);
}

// 连接MQTT
bool connect_mqtt(MqttAudioStop* mqttAudioStop) {
    return mqtt_client_connect(mqttAudioStop->mqttClient);
}

// 断开MQTT连接
void disconnect_mqtt(MqttAudioStop* mqttAudioStop) {
    mqtt_client_disconnect(mqttAudioStop->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt(MqttAudioStop* mqttAudioStop, const char* topic) {
    return mqtt_client_subscribe(mqttAudioStop->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt(MqttAudioStop* mqttAudioStop, const char* topic, const char* message) {
    return mqtt_client_publish(mqttAudioStop->mqttClient, topic, message);
}

// 永久循环
void loop_forever(MqttAudioStop* mqttAudioStop) {
    mqtt_client_loop_forever(mqttAudioStop->mqttClient);
}

// 处理音频停止命令
json_t* handle_audio_stop_command(const json_t* command) {
    // 这里应该实现实际的音频停止逻辑
    // 为了示例，我们假设停止总是成功的
    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", "音频停止成功", 
                     "resultId", 1, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "audioStop", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_object());
}