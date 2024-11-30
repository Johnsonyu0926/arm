#include "mqtt_filedelete.h"
#include <sys/stat.h>
#include <unistd.h>

// 创建MqttFileDelete对象
MqttFileDelete* create_mqtt_file_delete(const char* clientId, const char* host, int port) {
    MqttFileDelete* mqttFileDelete = (MqttFileDelete*)malloc(sizeof(MqttFileDelete));
    if (!mqttFileDelete) return NULL;
    mqttFileDelete->clientId = strdup(clientId);
    mqttFileDelete->host = strdup(host);
    mqttFileDelete->port = port;
    mqttFileDelete->mqttClient = NULL;
    return mqttFileDelete;
}

// 销毁MqttFileDelete对象
void destroy_mqtt_file_delete(MqttFileDelete* mqttFileDelete) {
    if (mqttFileDelete) {
        free(mqttFileDelete->clientId);
        free(mqttFileDelete->host);
        free(mqttFileDelete);
    }
}

// 初始化MQTT客户端
void initialize_mqtt_client(MqttFileDelete* mqttFileDelete) {
    mqttFileDelete->mqttClient = create_mqtt_client(mqttFileDelete->clientId, mqttFileDelete->host, mqttFileDelete->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttFileDelete* mqttFileDelete, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttFileDelete->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttFileDelete* mqttFileDelete) {
    return connect_mqtt_client(mqttFileDelete->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttFileDelete* mqttFileDelete) {
    disconnect_mqtt_client(mqttFileDelete->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttFileDelete* mqttFileDelete, const char* topic) {
    return subscribe_mqtt_client(mqttFileDelete->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttFileDelete* mqttFileDelete, const char* topic, const char* message) {
    return publish_mqtt_client(mqttFileDelete->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttFileDelete* mqttFileDelete) {
    mqtt_client_loop_forever(mqttFileDelete->mqttClient);
}

// 删除文件
bool delete_file(const char* fileName, int storageType) {
    char filePath[256];
    switch (storageType) {
        case 1:
            snprintf(filePath, sizeof(filePath), "/path/to/storage1/%s", fileName);
            break;
        case 2:
            snprintf(filePath, sizeof(filePath), "/path/to/storage2/%s", fileName);
            break;
        // Add more cases if needed
        default:
            fprintf(stderr, "Unknown storage type: %d\n", storageType);
            return false;
    }

    if (unlink(filePath) == 0) {
        printf("File deleted successfully: %s\n", filePath);
        return true;
    } else {
        perror("Error deleting file");
        return false;
    }
}

// 处理文件删除命令
json_t* handle_file_delete_command(const json_t* command) {
    const char* fileName = json_string_value(json_object_get(command, "fileName"));
    int audioUploadRecordId = json_integer_value(json_object_get(command, "audioUploadRecordId"));
    int storageType = json_integer_value(json_object_get(command, "storageType"));

    bool success = delete_file(fileName, storageType);

    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", success ? "文件删除成功" : "文件删除失败", 
                     "resultId", success ? 1 : 2, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "fileDelete", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_pack("{s:i}", "audioUploadRecordId", audioUploadRecordId));
}