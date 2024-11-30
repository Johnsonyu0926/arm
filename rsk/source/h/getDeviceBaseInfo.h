#ifndef GET_DEVICE_BASE_INFO_H
#define GET_DEVICE_BASE_INFO_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* deviceId;
    char* deviceType;
    char* firmwareVersion;
} DeviceBaseInfo;

typedef struct {
    char* configPath;
    DeviceBaseInfo deviceInfo;
} DeviceBaseInfoManager;

DeviceBaseInfo* create_device_base_info(const char* deviceId, const char* deviceType, const char* firmwareVersion);
void destroy_device_base_info(DeviceBaseInfo* info);
const char* get_device_id(const DeviceBaseInfo* info);
void set_device_id(DeviceBaseInfo* info, const char* newDeviceId);
const char* get_device_type(const DeviceBaseInfo* info);
void set_device_type(DeviceBaseInfo* info, const char* newDeviceType);
const char* get_firmware_version(const DeviceBaseInfo* info);
void set_firmware_version(DeviceBaseInfo* info, const char* newFirmwareVersion);
json_t* device_base_info_to_json(const DeviceBaseInfo* info);
DeviceBaseInfo* device_base_info_from_json(const json_t* j);

DeviceBaseInfoManager* create_device_base_info_manager(const char* configPath);
void destroy_device_base_info_manager(DeviceBaseInfoManager* manager);
bool load_device_base_info_manager(DeviceBaseInfoManager* manager);
bool save_device_base_info_manager(const DeviceBaseInfoManager* manager);
const DeviceBaseInfo* get_device_info(const DeviceBaseInfoManager* manager);
void set_device_info(DeviceBaseInfoManager* manager, DeviceBaseInfo* newDeviceInfo);

#endif // GET_DEVICE_BASE_INFO_H