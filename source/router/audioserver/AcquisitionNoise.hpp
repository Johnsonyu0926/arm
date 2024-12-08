#pragma once

#include <deque>
#include <fstream>
#include <vector>
#include <optional>
#include <string>
#include "volume.hpp"
#include "json.hpp"

using json = nlohmann::json;

class CSchema {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CSchema, min, max, volume)

    CSchema() = default;

    CSchema(int min, int max, int vol) : min(min), max(max), volume(vol) {}

public:
    int min{0};
    int max{0};
    int volume{0};
};

class AcquisitionNoise {
public:
    AcquisitionNoise(const AcquisitionNoise &) = delete;
    AcquisitionNoise &operator=(const AcquisitionNoise &) = delete;

    static AcquisitionNoise &getInstance() {
        static AcquisitionNoise instance;
        return instance;
    }

    std::optional<int> get_noise_adaptive_volume() const {
        for (const auto &it : schema) {
            if (decibel > it.min && decibel < it.max) {
                return it.volume;
            }
        }
        return std::nullopt;
    }

    void noise_auto_set_volume() {
        auto volume_opt = get_noise_adaptive_volume();
        if (volume_opt) {
            int volume = volume_opt.value();
            asns::CVolumeSet volumeSet;
            volumeSet.load();
            if (volume != volumeSet.getVolume()) {
                volumeSet.configVolume(volume);
            }
        }
    }

    bool file_load() {
        std::ifstream i(NOISE_FILE_NAME);
        if (!i.is_open()) {
            LOG(WARNING) << NOISE_FILE_NAME << " file load error!";
            return false;
        }
        json js;
        try {
            i >> js;
            monitorStatus = js.at("monitorStatus").get<int>();
            frequency = js.at("frequency").get<int>();
            calcCycle = js.at("calcCycle").get<int>();
            schema = js.at("schema").get<std::vector<CSchema>>();
            LOG(INFO) << "schema:" << schema.size();
        } catch (json::parse_error &ex) {
            LOG(ERROR) << "parse error at byte " << ex.byte;
            i.close();
            return false;
        }
        i.close();
        return true;
    }

    void file_update() const {
        std::ofstream o(NOISE_FILE_NAME);
        if (!o.is_open()) {
            LOG(ERROR) << NOISE_FILE_NAME << " stream open fail!";
            return;
        }
        json js;
        js["monitorStatus"] = monitorStatus;
        js["frequency"] = frequency;
        js["calcCycle"] = calcCycle;
        js["schema"] = schema;
        o << js << std::endl;
        o.close();
    }

    void setDecibel(double decibel) {
        this->decibel = decibel;
    }

    double getDecibel() const {
        return decibel;
    }

    void setMonitorStatus(int monitorStatus) {
        this->monitorStatus = monitorStatus;
    }

    int getMonitorStatus() const {
        return monitorStatus;
    }

    void setFrequency(int frequency) {
        this->frequency = frequency;
    }

    int getFrequency() const {
        return frequency;
    }

    void setCalcCycle(int calcCycle) {
        this->calcCycle = calcCycle;
    }

    int getCalcCycle() const {
        return calcCycle;
    }

    void noiseDequeClear() {
        noiseDeque.clear();
        decibel = 0;
    }

    ~AcquisitionNoise() = default;

    std::deque<double> noiseDeque{};
    std::vector<CSchema> schema;

private:
    AcquisitionNoise() = default;

    static constexpr const char* NOISE_FILE_NAME = "/mnt/cfg/noise.json";

private:
    int monitorStatus{0};
    int frequency{0};
    int calcCycle{0};
    double decibel{0};
};