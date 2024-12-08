#pragma once

#include <fstream>
#include "json.hpp"
#include "audiocfg.hpp"
#include "spdlog/spdlog.h"

namespace asns {

    class TestFile {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TestFile, ffmpegCmd)

        std::string ffmpegCmd = "ffplay -fflags nobuffer -nodisp -analyzeduration 1 %s &";
    };

    class TestFileBusiness {
    public:
        TestFileBusiness() {
            CAudioCfgBusiness business;
            business.load();
            filePath = business.business[0].savePrefix + TEST_FILE;
            load();
        }

        void load() {
            std::ifstream ifs(filePath);
            if (!ifs.is_open()) {
                spdlog::info("Failed to open test.json");
                return;
            }
            try {
                nlohmann::json js;
                ifs >> js;
                testFile = js.get<TestFile>();
            } catch (nlohmann::json::parse_error &ex) {
                spdlog::error("Parse error at byte {}", ex.byte);
            }
            ifs.close();
        }

        std::string getFfmpegCmd() const {
            return testFile.ffmpegCmd;
        }

    private:
        TestFile testFile;
        std::string filePath;
        static constexpr const char* TEST_FILE = "/cfg/test.json";
    };
} // namespace asns