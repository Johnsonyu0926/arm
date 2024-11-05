#include <yangrtc/YangBandwidth.h>
#include <yangrtc/YangRtcStats.h>

void yang_bandwidth_updateEstimate(int64_t now_ms) {
    // Function implementation can be added here if needed
}

// By lost RTP packet rate
int32_t yang_bandwidth_check(YangBandwidthSession* bw, YangRtcStats* stats, YangStreamConfig* streamconfig, uint32_t ssrc) {
    int32_t err = Yang_Ok;
    bw->lostRate = 0;

    if (stats->recvStats.video.fractionLost < kDefaultHighLossThreshold && bw->preLostRateState == YangLowLostRate) {
        return Yang_Ok;
    }

    if (stats->recvStats.video.fractionLost > kDefaultHighLossThreshold) {
        bw->hightLostCount++;
        bw->lowLostCount = 0;
        if (bw->hightLostCount >= Yang_LostRate_defaultCount) {
            bw->lostRate = stats->recvStats.video.fractionLost;
            bw->lostRateState = YangHighLostRate;
        }
    } else {
        bw->lowLostCount++;
        bw->hightLostCount = 0;
        if (bw->lowLostCount >= Yang_LostRate_defaultCount) {
            bw->lostRate = stats->recvStats.video.fractionLost;
            bw->lostRateState = YangLowLostRate;
        }
    }

    if (bw->preLostRateState == bw->lostRateState) {
        return Yang_Ok;
    }

    // High lost rate
    if (bw->lostRateState == YangHighLostRate) {
        if (streamconfig->rtcCallback.sendRequest) {
            streamconfig->rtcCallback.sendRequest(streamconfig->rtcCallback.context, streamconfig->uid, ssrc, Yang_Req_HighLostPacketRate);
        }
    } else if (bw->lostRateState == YangLowLostRate) { // Low lost rate
        if (streamconfig->rtcCallback.sendRequest) {
            streamconfig->rtcCallback.sendRequest(streamconfig->rtcCallback.context, streamconfig->uid, ssrc, Yang_Req_LowLostPacketRate);
        }
    }

    bw->preLostRateState = bw->lostRateState;

    return err;
}

int32_t yang_bandwidth_checkByTwcc(YangBandwidthSession* bw, YangTwccSession* twcc) {
    // Function implementation can be added here if needed
    return Yang_Ok;
}

int32_t yang_bandwidth_estimate(YangBandwidthSession* bw, YangTwccSession* twcc) {
    // Function implementation can be added here if needed
    return Yang_Ok;
}

void yang_create_bandwidth(YangBandwidth* bw) {
    if (bw == NULL) return;
    bw->estimate = yang_bandwidth_estimate;
    bw->checkByTwcc = yang_bandwidth_checkByTwcc;
    bw->checkBandWidth = yang_bandwidth_check;
}

void yang_destroy_bandwidth(YangBandwidth* bw) {
    // Function implementation can be added here if needed
}

#endif /* SRC_YANGRTC_YANGBANDWIDTH_H_ */

// By GST @Date