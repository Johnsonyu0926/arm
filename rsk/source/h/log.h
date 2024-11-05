// log.h
#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>
#include <memory>
#include <iostream>
#include <mutex>
#include <chrono>
#include <format>
#include <source_location>

class CLogger {
public:
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static CLogger& getInstance() {
        static CLogger instance;
        return instance;
    }

    void setLogFile(const std::string& filename) {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_logFile = std::make_unique<std::ofstream>(filename, std::ios::app);
    }

    template<typename... Args>
    void log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
        log(level, std::format(fmt, std::forward<Args>(args)...), std::source_location::current());
    }

    template<typename... Args>
    void debug(std::format_string<Args...> fmt, Args&&... args) {
        log(LogLevel::DEBUG, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args) {
        log(LogLevel::INFO, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning(std::format_string<Args...> fmt, Args&&... args) {
        log(LogLevel::WARNING, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args) {
        log(LogLevel::ERROR, fmt, std::forward<Args>(args)...);
    }

private:
    CLogger() = default;
    ~CLogger() = default;
    CLogger(const CLogger&) = delete;
    CLogger& operator=(const CLogger&) = delete;
    CLogger(CLogger&&) = delete;
    CLogger& operator=(CLogger&&) = delete;

    void log(LogLevel level, const std::string& message, const std::source_location& location) {
        std::lock_guard<std::mutex> guard(m_mutex);
        const auto& output = m_logFile && m_logFile->is_open() ? *m_logFile : std::cerr;
        output << std::format("{} [{}] {} ({}:{})\n", 
                              getTimestamp(), logLevelToString(level), message, 
                              location.file_name(), location.line());
    }

    static std::string getTimestamp() {
        const auto now = std::chrono::system_clock::now();
        return std::format("{:%Y-%m-%d %H:%M:%S}", now);
    }

    static constexpr std::string_view logLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    std::unique_ptr<std::ofstream> m_logFile;
    std::mutex m_mutex;
};

#endif // LOG_H

//BY GST ARMV8 GCC 13.2

/*这些更改使日志类更加现代化、类型安全和高效，同时保持了原有的功能和接口。使用这个版本的日志类，你可以这样记录日志：
CLogger::getInstance().info("Starting application version {}", APP_VERSION);
CLogger::getInstance().error("Failed to open file: {}", filename);

这将自动包含时间戳、日志级别、消息内容、源文件名和行号。