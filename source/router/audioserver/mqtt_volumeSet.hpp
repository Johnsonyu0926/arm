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

    class CVolumeSetResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CVolumeSetResultData, null
        );

        template<typename Quest, typename Result>
        void do_success(const CReQuest<Quest, Result> &c) {}

    private:
        std::nullptr_t null;
    };

    class CVolumeSetData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CVolumeSetData, volume
        )

        int do_req() {
            int volume = std::stoi(this->volume);
            if (volume > 7 || volume < 0) {
                return 2;
            } else {
                CVolumeSet volumeSet;
                volumeSet.setVolume(volume);
                volumeSet.addj(volume);
                volumeSet.saveToJson();
                return 1;
            }
            return 0;
        }

    public:
        std::string volume;
    };
}