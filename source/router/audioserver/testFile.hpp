#pragma once

#include "json.hpp"
#include "audiocfg.hpp"

namespace asns {
    class TestFile {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TestFile, ffmpegCmd)

    public:
        std::string ffmpegCmd;
    };

    class TestFileBusiness {
    public:
        const std::string TEST_FILE = "/cfg/test.json";

        TestFileBusiness() {
            CAudioCfgBusiness business;
            business.load();
            filePath = business.business[0].savePrefix + TEST_FILE;
            std::ifstream ifs(filePath);
            if (!ifs.is_open()) {
                DS_TRACE("open test.json fail");
            }
            json js;
            ifs >> js;
            testFile = js;
        }

        void load() {
            std::ifstream ifs(filePath);
            if (!ifs.is_open()) {
                DS_TRACE("open test.json fail");
                return;
            }
            json js;
            ifs >> js;
            testFile = js;
        }

        std::string getFfmpegCmd() const {
            return testFile.ffmpegCmd;
        }

    private:
        TestFile testFile;
        std::string filePath;
    };
}