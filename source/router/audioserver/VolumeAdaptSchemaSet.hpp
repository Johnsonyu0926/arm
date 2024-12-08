#pragma once

#include "json.hpp"
#include "AcquisitionNoise.hpp"
#include "spdlog/spdlog.h"

namespace asns {

    class CVolumeAdaptSchemaData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CVolumeAdaptSchemaData, monitorStatus, frequency, calcCycle, schema)

        void set_data() const {
            auto& acquisitionNoise = AcquisitionNoise::getInstance();
            acquisitionNoise.setMonitorStatus(monitorStatus);
            acquisitionNoise.setFrequency(frequency);
            acquisitionNoise.setCalcCycle(calcCycle);
            if (calcCycle < static_cast<int>(acquisitionNoise.noiseDeque.size())) {
                const size_t n = acquisitionNoise.noiseDeque.size() - calcCycle;
                for (size_t i = 0; i < n; ++i) {
                    acquisitionNoise.noiseDeque.pop_back();
                }
            }
            acquisitionNoise.schema.assign(schema.cbegin(), schema.cend());
            acquisitionNoise.file_update();
            spdlog::info("Set data: monitorStatus={}, frequency={}, calcCycle={}, schema size={}",
                         monitorStatus, frequency, calcCycle, schema.size());
        }

        void do_data() {
            auto& acquisitionNoise = AcquisitionNoise::getInstance();
            monitorStatus = acquisitionNoise.getMonitorStatus();
            frequency = acquisitionNoise.getFrequency();
            calcCycle = acquisitionNoise.getCalcCycle();
            schema.assign(acquisitionNoise.schema.cbegin(), acquisitionNoise.schema.cend());
            spdlog::info("Get data: monitorStatus={}, frequency={}, calcCycle={}, schema size={}",
                         monitorStatus, frequency, calcCycle, schema.size());
        }

    public:
        int monitorStatus{0};
        int frequency{0};
        int calcCycle{0};
        std::vector<CSchema> schema;
    };
} // namespace asns