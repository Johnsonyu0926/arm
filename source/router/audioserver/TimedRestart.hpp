#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <algorithm>

#include "json.hpp"
#include "utils.h"
#include "spdlog/spdlog.h"

using json = nlohmann::json;

class TimedRestart {
public:
    TimedRestart() {
        file_load();
    }

    int file_load() {
        std::ifstream i(TIMED_RESTART_FILE_NAME);
        if (!i.is_open()) {
            spdlog::error("{} file load error!", TIMED_RESTART_FILE_NAME);
            return 0;
        }

        try {
            json js;
            i >> js;
            spdlog::info("Loaded JSON: {}", js.dump());
            timeList_ = js.at("rebootTask").get<std::vector<std::string>>();
        } catch (json::parse_error &ex) {
            spdlog::error("Parse error at byte {}", ex.byte);
            i.close();
            return 0;
        }
        i.close();
        return 1;
    }

    void testJson() {
        json js;
        timeList_.emplace_back("01:00");
        js["rebootTask"] = timeList_;
        std::ofstream o(TIMED_RESTART_FILE_NAME);
        if (!o.is_open()) {
            spdlog::error("Failed to open file for writing: {}", TIMED_RESTART_FILE_NAME);
            return;
        }
        o << std::setw(4) << js << std::endl;
        o.close();
    }

    std::string getCurrTime() const {
        const std::time_t t = std::time(nullptr);
        const std::tm *lt = std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(lt, "%H:%M");
        return ss.str();
    }

    void loop() {
        CUtils::async_wait(1, 0, 0, [&] {
            while (true) {
                std::for_each(timeList_.begin(), timeList_.end(), [&](const std::string &time) {
                    spdlog::info("Checking time: {} Current time: {}", time, getCurrTime());
                    if (getCurrTime() == time) {
                        spdlog::info("Time matched: {}. Rebooting...", time);
                        CUtils::reboot();
                    }
                });
                std::this_thread::sleep_for(std::chrono::seconds(30));
            }
        });
    }

private:
    std::vector<std::string> timeList_;
    static constexpr const char* TIMED_RESTART_FILE_NAME = "/mnt/cfg/timedRestart.json";
};