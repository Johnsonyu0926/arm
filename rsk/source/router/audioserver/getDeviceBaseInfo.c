#include "get_device_base_info.h"

// 创建DeviceBaseInfo对象
DeviceBaseInfo* create_device_base_info(const char* deviceId, const char* deviceType, const char* firmwareVersion) {
    DeviceBaseInfo* info = (DeviceBaseInfo*)malloc(sizeof(DeviceBaseInfo));
    if (!info) return NULL;
    info->deviceId = strdup(deviceId);
    info->deviceType = strdup(deviceType);
    info->firmwareVersion = strdup(firmwareVersion);
    return info;
}

// 销毁DeviceBaseInfo对象
void destroy_device_base_info(DeviceBaseInfo* info) {
    if (info) {
        free(info->deviceId);
        free(info->deviceType);
        free(info->firmwareVersion);
        free(info);
    }
}

// 获取DeviceBaseInfo的deviceId
const char* get_device_id(const DeviceBaseInfo* info) {
    return info->deviceId;
}

// 设置DeviceBaseInfo的deviceId
void set_device_id(DeviceBaseInfo* info, const char* newDeviceId) {
    free(info->deviceId);
    info->deviceId = strdup(newDeviceId);
}

// 获取DeviceBaseInfo的deviceType
const char* get_device_type(const DeviceBaseInfo* info) {
    return info->deviceType;
}

// 设置DeviceBaseInfo的deviceType
void set_device_type(DeviceBaseInfo* info, const char* newDeviceType) {
    free(info->deviceType);
    info->deviceType = strdup(newDeviceType);
}

// 获取DeviceBaseInfo的firmwareVersion
const char* get_firmware_version(const DeviceBaseInfo* info) {
    return info->firmwareVersion;
}

// 设置DeviceBaseInfo的firmwareVersion
void set_firmware_version(DeviceBaseInfo* info, const char* newFirmwareVersion) {
    free(info->firmwareVersion);
    info->firmwareVersion = strdup(newFirmwareVersion);
}

// 将DeviceBaseInfo对象转换为JSON
json_t* device_base_info_to_json(const DeviceBaseInfo* info) {
    json_t* j = json_object();
    json_object_set_new(j, "deviceId", json_string(info->deviceId));
    json_object_set_new(j, "deviceType", json_string(info->deviceType));
    json_object_set_new(j, "firmwareVersion", json_string(info->firmwareVersion));
    return j;
}

// 从JSON创建DeviceBaseInfo对象
DeviceBaseInfo* device_base_info_from_json(const json_t* j) {
    const char* deviceId = json_string_value(json_object_get(j, "deviceId"));
    const char* deviceType = json_string_value(json_object_get(j, "deviceType"));
    const char* firmwareVersion = json_string_value(json_object_get(j, "firmwareVersion"));
    return create_device_base_info(deviceId, deviceType, firmwareVersion);
}

// 创建DeviceBaseInfoManager对象
DeviceBaseInfoManager* create_device_base_info_manager(const char* configPath) {
    DeviceBaseInfoManager* manager = (DeviceBaseInfoManager*)malloc(sizeof(DeviceBaseInfoManager));
    if (!manager) return NULL;
    manager->configPath = strdup(configPath);
    manager->deviceInfo.deviceId = NULL;
    manager->deviceInfo.deviceType = NULL;
    manager->deviceInfo.firmwareVersion = NULL;
    return manager;
}

// 销毁DeviceBaseInfoManager对象
void destroy_device_base_info_manager(DeviceBaseInfoManager* manager) {
    if (manager) {
        free(manager->configPath);
        destroy_device_base_info(&manager->deviceInfo);
        free(manager);
    }
}

// 加载DeviceBaseInfoManager配置
bool load_device_base_info_manager(DeviceBaseInfoManager* manager) {
    FILE* file = fopen(manager->configPath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open config file: %s\n", manager->configPath);
        return false;
    }

    json_error_t error;
    json_t* j = json_loadf(file, 0, &error);
    fclose(file);
    if (!j) {
        fprintf(stderr, "Error loading config file: %s\n", error.text);
        return false;
    }

    DeviceBaseInfo* info = device_base_info_from_json(j);
    set_device_info(manager, info);
    json_decref(j);
    return true;
}

// 保存DeviceBaseInfoManager配置
bool save_device_base_info_manager(const DeviceBaseInfoManager* manager) {
    json_t* j = device_base_info_to_json(&manager->deviceInfo);

    FILE* file = fopen(manager->configPath, "w");
    if (!file) {
        fprintf(stderr, "Failed to open config file for writing: %s\n", manager->configPath);
        json_decref(j);
        return false;
    }

    if (json_dumpf(j, file, JSON_INDENT(4)) != 0) {
        fprintf(stderr, "Error saving config file\n");
        fclose(file);
        json_decref(j);
        return false;
    }

    fclose(file);
    json_decref(j);
    return true;
}

// 获取DeviceBaseInfoManager的DeviceBaseInfo
const DeviceBaseInfo* get_device_info(const DeviceBaseInfoManager* manager) {
    return &manager->deviceInfo;
}

// 设置DeviceBaseInfoManager的DeviceBaseInfo
void set_device_info(DeviceBaseInfoManager* manager, DeviceBaseInfo* newDeviceInfo) {
    destroy_device_base_info(&manager->deviceInfo);
    manager->deviceInfo = *newDeviceInfo;
    free(newDeviceInfo);
}