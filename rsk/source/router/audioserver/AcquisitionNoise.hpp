// AcquisitionNoise.hpp

#pragma once

#include <deque>
#include <numeric>
#include <cmath>
#include <fstream>
#include <chrono>
#include <thread>
#include <shared_mutex>
#include <atomic>
#include <filesystem>
#include <stdexcept>
#include <nlohmann/json.hpp>

class AcquisitionNoise {
public:
    static AcquisitionNoise& getInstance() {
        static AcquisitionNoise instance;
        return instance;
    }

    AcquisitionNoise(const AcquisitionNoise&) = delete;
    AcquisitionNoise& operator=(const AcquisitionNoise&) = delete;
    AcquisitionNoise(AcquisitionNoise&&) = delete;
    AcquisitionNoise& operator=(AcquisitionNoise&&) = delete;

    void updateNoise(const std::deque<double>& noiseDeque) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        this->noiseDeque = noiseDeque;
    }

    [[nodiscard]] std::deque<double> getNoiseDeque() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return noiseDeque;
    }

    void setMonitorStatus(bool status) { monitorStatus.store(status, std::memory_order_release); }
    [[nodiscard]] bool getMonitorStatus() const { return monitorStatus.load(std::memory_order_acquire); }

    void setFrequency(int freq) { frequency.store(freq, std::memory_order_release); }
    [[nodiscard]] int getFrequency() const { return frequency.load(std::memory_order_acquire); }

    void setCalcCycle(int cycle) { calcCycle.store(cycle, std::memory_order_release); }
    [[nodiscard]] int getCalcCycle() const { return calcCycle.load(std::memory_order_acquire); }

    void setDecibel(double dec) { decibel.store(dec, std::memory_order_release); }
    [[nodiscard]] double getDecibel() const { return decibel.load(std::memory_order_acquire); }

    void file_update() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        try {
            std::filesystem::path filePath = "/mnt/cfg/noise.json";
            std::ofstream ofs(filePath);
            if (!ofs) {
                throw std::runtime_error("Unable to open file for writing: " + filePath.string());
            }
            nlohmann::json j = {
                {"noiseDeque", noiseDeque},
                {"monitorStatus", monitorStatus.load()},
                {"frequency", frequency.load()},
                {"calcCycle", calcCycle.load()},
                {"decibel", decibel.load()}
            };
            ofs << j.dump(4);
            if (ofs.fail()) {
                throw std::runtime_error("Failed to write to file: " + filePath.string());
            }
        } catch (const std::exception& e) {
            // 在实际应用中，您可能想要使用更复杂的日志系统
            std::cerr << "Error in file_update: " << e.what() << std::endl;
        }
    }

private:
    AcquisitionNoise() = default;

    mutable std::shared_mutex mutex_;
    std::deque<double> noiseDeque;
    std::atomic<bool> monitorStatus{false};
    std::atomic<int> frequency{0};
    std::atomic<int> calcCycle{0};
    std::atomic<double> decibel{0.0};
};

//By GST ARMV8 GCC13.2 AcquisitionNoise.hpp