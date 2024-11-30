#include "mqtt_audiostreamstart.h"

// 创建MqttAudioStreamStart对象
MqttAudioStreamStart* create_mqtt_audio_stream_start(const char* clientId, const char* host, int port) {
    MqttAudioStreamStart* mqttAudioStreamStart = (MqttAudioStreamStart*)malloc(sizeof(MqttAudioStreamStart));
    if (!mqttAudioStreamStart) return NULL;
    mqttAudioStreamStart->clientId = strdup(clientId);
    mqttAudioStreamStart->host = strdup(host);
    mqttAudioStreamStart->port = port;
    mqttAudioStreamStart->audioConfig = NULL;
    mqttAudioStreamStart->audioConfigCount = 0;
    mqttAudioStreamStart->mqttClient = NULL;
    return mqttAudioStreamStart;
}

// 销毁MqttAudioStreamStart对象
void destroy_mqtt_audio_stream_start(MqttAudioStreamStart* mqttAudioStreamStart) {
    if (mqttAudioStreamStart) {
        free(mqttAudioStreamStart->clientId);
        free(mqttAudioStreamStart->host);
        free(mqttAudioStreamStart->audioConfig);
        free(mqttAudioStreamStart);
    }
}

// 加载音频流启动配置
bool load_audio_stream_start_config(MqttAudioStreamStart* mqttAudioStreamStart, const char* configPath) {
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

    size_t index;
    json_t* value;
    json_array_foreach(json, index, value) {
        AudioStreamStart* audioStreamStart = audio_stream_start_from_json(value);
        mqttAudioStreamStart->audioConfig = (AudioStreamStart*)realloc(mqttAudioStreamStart->audioConfig, (mqttAudioStreamStart->audioConfigCount + 1) * sizeof(AudioStreamStart));
        mqttAudioStreamStart->audioConfig[mqttAudioStreamStart->audioConfigCount++] = *audioStreamStart;
        free(audioStreamStart);
    }

    json_decref(json);
    return true;
}

// 保存音频流启动配置
bool save_audio_stream_start_config(const MqttAudioStreamStart* mqttAudioStreamStart, const char* configPath) {
    json_t* json = json_array();
    for (size_t i = 0; i < mqttAudioStreamStart->audioConfigCount; ++i) {
        json_t* audioStreamStartJson = audio_stream_start_to_json(&mqttAudioStreamStart->audioConfig[i]);
        json_array_append_new(json, audioStreamStartJson);
    }

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
void initialize_mqtt_client(MqttAudioStreamStart* mqttAudioStreamStart) {
    mqttAudioStreamStart->mqttClient = create_mqtt_client(mqttAudioStreamStart->clientId, mqttAudioStreamStart->host, mqttAudioStreamStart->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttAudioStreamStart* mqttAudioStreamStart, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttAudioStreamStart->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttAudioStreamStart* mqttAudioStreamStart) {
    return connect_mqtt_client(mqttAudioStreamStart->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttAudioStreamStart* mqttAudioStreamStart) {
    disconnect_mqtt_client(mqttAudioStreamStart->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttAudioStreamStart* mqttAudioStreamStart, const char* topic) {
    return subscribe_mqtt_client(mqttAudioStreamStart->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttAudioStreamStart* mqttAudioStreamStart, const char* topic, const char* message) {
    return publish_mqtt_client(mqttAudioStreamStart->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttAudioStreamStart* mqttAudioStreamStart) {
    mqtt_client_loop_forever(mqttAudioStreamStart->mqttClient);
}

// 处理音频流启动命令
json_t* handle_audio_stream_start_command(const MqttAudioStreamStart* mqttAudioStreamStart, const json_t* command) {
    // 这里应该实现实际的音频流启动逻辑
    // 为了示例，我们假设启动总是成功的
    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", "音频流启动成功", 
                     "resultId", 1, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "audioStreamStart", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_object());
}