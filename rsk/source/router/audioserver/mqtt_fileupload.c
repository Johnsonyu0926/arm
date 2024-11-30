#include "mqtt_fileupload.h"

// 创建MqttFileUpload对象
MqttFileUpload* create_mqtt_file_upload(const char* clientId, const char* host, int port) {
    MqttFileUpload* mqttFileUpload = (MqttFileUpload*)malloc(sizeof(MqttFileUpload));
    if (!mqttFileUpload) return NULL;
    mqttFileUpload->clientId = strdup(clientId);
    mqttFileUpload->host = strdup(host);
    mqttFileUpload->port = port;
    mqttFileUpload->mqttClient = NULL;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    return mqttFileUpload;
}

// 销毁MqttFileUpload对象
void destroy_mqtt_file_upload(MqttFileUpload* mqttFileUpload) {
    if (mqttFileUpload) {
        free(mqttFileUpload->clientId);
        free(mqttFileUpload->host);
        free(mqttFileUpload);
        curl_global_cleanup();
    }
}

// 初始化MQTT客户端
void initialize_mqtt_client(MqttFileUpload* mqttFileUpload) {
    mqttFileUpload->mqttClient = create_mqtt_client(mqttFileUpload->clientId, mqttFileUpload->host, mqttFileUpload->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttFileUpload* mqttFileUpload, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttFileUpload->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttFileUpload* mqttFileUpload) {
    return connect_mqtt_client(mqttFileUpload->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttFileUpload* mqttFileUpload) {
    disconnect_mqtt_client(mqttFileUpload->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttFileUpload* mqttFileUpload, const char* topic) {
    return subscribe_mqtt_client(mqttFileUpload->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttFileUpload* mqttFileUpload, const char* topic, const char* message) {
    return publish_mqtt_client(mqttFileUpload->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttFileUpload* mqttFileUpload) {
    mqtt_client_loop_forever(mqttFileUpload->mqttClient);
}

// 写回调函数
static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((char*)userp)[size * nmemb] = '\0';
    strncat((char*)userp, (char*)contents, size * nmemb);
    return size * nmemb;
}

// 下载文件
bool download_file(const char* url, const char* fileName) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return false;
    }

    char readBuffer[1024 * 1024] = {0}; // 1MB buffer
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, readBuffer);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "Failed to download file: %s\n", curl_easy_strerror(res));
        return false;
    }

    FILE* outFile = fopen(fileName, "wb");
    if (!outFile) {
        fprintf(stderr, "Failed to open file for writing: %s\n", fileName);
        return false;
    }

    fwrite(readBuffer, 1, strlen(readBuffer), outFile);
    fclose(outFile);
    return true;
}

// 处理文件上传命令
json_t* handle_file_upload_command(const json_t* command) {
    const char* downloadUrl = json_string_value(json_object_get(command, "downloadUrl"));
    const char* fileName = json_string_value(json_object_get(command, "fileName"));
    int audioUploadRecordId = json_integer_value(json_object_get(command, "audioUploadRecordId"));
    int storageType = json_integer_value(json_object_get(command, "storageType"));

    bool success = download_file(downloadUrl, fileName);

    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", success ? "文件上传成功" : "文件上传失败", 
                     "resultId", success ? 1 : 2, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "fileUpload", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_pack("{s:i}", "audioUploadRecordId", audioUploadRecordId));
}