// testfile.hpp
#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "audiocfg.hpp"
#include "log.h"  // 假设这是日志头文件

namespace asns {

class TestFile {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TestFile, ffmpegCmd)

    std::string ffmpegCmd = "ffplay -fflags nobuffer -nodisp -analyzeduration 1 %s &";
};

class TestFileBusiness {
public:
    static constexpr std::string_view TEST_FILE = "/cfg/test.json";

    TestFileBusiness() {
        CAudioCfgBusiness business;
        business.load();
        filePath = std::filesystem::path(business.business[0].savePrefix) / TEST_FILE;
        load();
    }

    void load() {
        std::ifstream ifs(filePath);
        if (!ifs.is_open()) {
            LOG(INFO) << "Failed to open test.json: " << filePath;
            return;
        }

        try {
            testFile = nlohmann::json::parse(ifs).get<TestFile>();
        } catch (const nlohmann::json::parse_error& ex) {
            LOG(ERROR) << "JSON parse error at byte " << ex.byte << ": " << ex.what();
        }
    }

    [[nodiscard]] std::string_view getFfmpegCmd() const noexcept {
        return testFile.ffmpegCmd;
    }

private:
    TestFile testFile;
    std::filesystem::path filePath;
};

} // namespace asns

//BY GST ARMV8 GCC 13.2