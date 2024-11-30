#include "log_init.h"
#include <zephyr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LOG_MODULE_REGISTER(log_init);

void initialize_logging(const char* configPath) {
    // 读取配置文件
    FILE* file = fopen(configPath, "r");
    if (!file) {
        LOG_ERR("Failed to open config file: %s", configPath);
        return;
    }

    // 读取配置文件内容
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* configContent = (char*)malloc(fileSize + 1);
    if (!configContent) {
        LOG_ERR("Failed to allocate memory for config content");
        fclose(file);
        return;
    }

    fread(configContent, 1, fileSize, file);
    configContent[fileSize] = '\0';
    fclose(file);

    // 解析配置文件内容并设置日志配置
    // 这里假设配置文件内容是简单的键值对格式
    char* line = strtok(configContent, "\n");
    while (line) {
        if (strstr(line, "log_level") != NULL) {
            if (strstr(line, "INFO") != NULL) {
                log_set_level(LOG_LEVEL_INF);
            } else if (strstr(line, "DEBUG") != NULL) {
                log_set_level(LOG_LEVEL_DBG);
            } else if (strstr(line, "ERROR") != NULL) {
                log_set_level(LOG_LEVEL_ERR);
            }
        }
        line = strtok(NULL, "\n");
    }

    free(configContent);

    LOG_INF("Logging system initialized with config from %s", configPath);
}