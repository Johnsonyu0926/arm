#pragma once

#include "volume.hpp"
#include "json.hpp"

/*
    {
        "volume":"7" // 需要设置的音量值，支持0-7档调节
    }
 */

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CVolumeSetResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CVolumeSetResultData, volume)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            volume = c.data.volume;
            int vol = std::stoi(volume);
            if (vol > 7 || vol < 0) {
                r.resultId = 2;
                r.result = "volume size error";
                return 2;
            } else {
                CVolumeSet volumeSet;
                volumeSet.setVolume(vol);
                volumeSet.addj(vol);
                volumeSet.saveToJson();
            }
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        std::string volume;
    };

    class CVolumeSetData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CVolumeSetData, volume)

        std::string volume;
    };
}