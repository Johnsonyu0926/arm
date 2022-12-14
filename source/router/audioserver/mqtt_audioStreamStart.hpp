#pragma once

#include "json.hpp"

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    class CAudioStreamStartResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioStreamStartResultData, null)

        template<typename Quest, typename Result>
        void do_success(const CReQuest<Quest, Result> &c) {}

    private:
        std::nullptr_t null;
    };

    class CAudioStreamStartData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioStreamStartData, streamPath, roomId, priority, volume)

        int do_req() {
            if (volume > 0 && volume <= 7) {
                CVolumeSet volumeSet;
                volumeSet.load();
                int volumeTemp = volumeSet.getVolume();
                volumeSet.setVolume(volume);
                volumeSet.addj(volume);
                volumeSet.saveToJson();

                std::string streamUrl = streamPath + roomId;
                char buf[256] = {0};
                sprintf(buf,
                        "ffmpeg -fflags nobuffer -r 30 -re -i %s -acodec mp3 -muxrate 7000000 -b:a 48k -max_delay 100 -bitrate 7000000 -fifo_size 27887 -burst_bits 1000000 -g 10 -b 7000000 -analyzeduration 10000000 -f mp3 - | madplay -",
                        streamUrl.c_str());
                std::cout << "system:" << buf << std::endl;
                system(buf);
                volumeSet.setVolume(volumeTemp);
                volumeSet.addj(volume);
                volumeSet.saveToJson();
                return 1;
            } else if (volume < 0 || volume > 7) {
            } else if (volume == 0) {
                std::string streamUrl = streamPath + roomId;
                char buf[256] = {0};
                sprintf(buf,
                        "ffmpeg -fflags nobuffer -r 30 -re -i %s -acodec mp3 -muxrate 7000000 -b:a 48k -max_delay 100 -bitrate 7000000 -fifo_size 27887 -burst_bits 1000000 -g 10 -b 7000000 -analyzeduration 10000000 -f mp3 - | madplay -",
                        streamUrl.c_str());
                std::cout << "system:" << buf << std::endl;
                system(buf);
                return 1;
            }
        }

    private:
        std::string streamPath;
        std::string roomId;
        int priority;
        int volume = 0;
    };
}