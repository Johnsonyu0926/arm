// log_init.hpp
#pragma once

#include <string>
#include <fstream>
#include "easylogging++.h"

void initializeLogging(const std::string& configPath) {
    // 加载配置文件
    el::Configurations conf(configPath);
    
    // 设置默认配置
    el::Loggers::setDefaultConfigurations(conf, true);

    // 设置日志刷新阈值
    el::Loggers::addFlag(el::LoggingFlag::ImmediateFlush);
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);

    // 设置最大日志文件大小（10MB）
    el::Helpers::setMaxLogFileSize(10 * 1024 * 1024);

    // 初始化日志系统
    el::Loggers::reconfigureAllLoggers(conf);

    LOG(INFO) << "Logging system initialized with config from " << configPath;
}