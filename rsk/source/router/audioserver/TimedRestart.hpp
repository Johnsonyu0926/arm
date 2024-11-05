// timerestart.hpp
#pragma once

#include <chrono>
#include <thread>
#include <functional>
#include <atomic>
#include <utility>

class TimedRestart {
public:
    TimedRestart() = default;
    ~TimedRestart() { stop(); }

    TimedRestart(const TimedRestart&) = delete;
    TimedRestart& operator=(const TimedRestart&) = delete;
    TimedRestart(TimedRestart&&) noexcept = default;
    TimedRestart& operator=(TimedRestart&&) noexcept = default;

    void start(std::chrono::seconds interval, std::function<void()> task) {
        stop();  // Ensure any existing thread is stopped
        running = true;
        worker_thread = std::thread([this, interval = std::move(interval), task = std::move(task)]() {
            while (running) {
                std::this_thread::sleep_for(interval);
                if (running) {
                    task();
                }
            }
        });
    }

    void stop() noexcept {
        running = false;
        if (worker_thread.joinable()) {
            worker_thread.join();
        }
    }

private:
    std::atomic<bool> running{false};
    std::thread worker_thread;
};

//BY GST ARMV8 GCC 13.2