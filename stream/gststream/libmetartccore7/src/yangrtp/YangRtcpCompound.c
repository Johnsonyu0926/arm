#include <yangrtp/YangRtcpCompound.h>

#include <yangrtp/YangRtcpSR.h>
#include <yangrtp/YangRtcpRR.h>
#include <yangrtp/YangRtcpXr.h>
#include <yangrtp/YangRtcpPli.h>
#include <yangrtp/YangRtcpSli.h>
#include <yangrtp/YangRtcpNack.h>
#include <yangrtp/YangRtcpTWCC.h>
#include <yangrtp/YangRtcpRpsi.h>

#include <yangutil/sys/YangLog.h>

yang_vector_impl(YangRtcpCommon)

void yang_create_rtcpCompound(YangRtcpCompound* rtcp) {
    if (rtcp == NULL) return;
    yang_create_YangRtcpCommonVector(&rtcp->rtcpVector);
}

void yang_destroy_rtcpCompound(YangRtcpCompound* rtcp) {
    if (rtcp == NULL) return;
    yang_rtcpCompound_clear(rtcp);
}

int32_t yang_decode_rtcpCompound(YangRtcpCompound* rtcps, YangBuffer* buffer) {
    if (rtcps == NULL || buffer == NULL) return 1;
    int32_t err = Yang_Ok;
    rtcps->data = buffer->data;
    rtcps->nb_data = buffer->size;
    YangRtcpHeader rtcpHeader = {0};
    YangRtcpHeader* header = &rtcpHeader;

    while (!yang_buffer_empty(buffer)) {
        YangRtcpCommon* rtcp = (YangRtcpCommon*) yang_calloc(1, sizeof(YangRtcpCommon));
        yang_decode_rtcpHeader(header, buffer->head);

        switch (header->type) {
            case YangRtcpType_sr:
                yang_create_rtcpSR(rtcp);
                err = yang_decode_rtcpSR(rtcp, buffer);
                break;
            case YangRtcpType_rr:
                yang_create_rtcpRR(rtcp);
                err = yang_decode_rtcpRR(rtcp, buffer);
                break;
            case YangRtcpType_rtpfb:
                if (header->rc == 1) {
                    yang_create_rtcpNack(rtcp, 0);
                    err = yang_decode_rtcpNack(rtcp, buffer);
                } else if (header->rc == 15) {
                    yang_create_rtcptwcc(rtcp);
                    err = yang_twcc_rtcp_decode(rtcp, buffer);
                }
                break;
            case YangRtcpType_psfb:
                if (header->rc == 1) {
                    yang_create_rtcpPli(rtcp);
                    err = yang_decode_rtcpPli(rtcp, buffer);
                } else if (header->rc == 2) {
                    yang_create_rtcpSli(rtcp, rtcp->ssrc);
                    err = yang_decode_rtcpSli(rtcp, buffer);
                } else if (header->rc == 3) {
                    yang_create_rtcpRpsi(rtcp);
                    err = yang_decode_rtcpRpsi(rtcp, buffer);
                } else {
                    yang_create_rtcpPsfb(rtcp);
                    err = yang_decode_rtcpPsfb(rtcp, buffer);
                }
                break;
            case YangRtcpType_xr:
                yang_create_rtcpXr(rtcp);
                err = yang_decode_rtcpXr(rtcp, buffer);
                break;
            default:
                yang_create_rtcpCommon(rtcp);
                err = yang_decode_rtcpCommon(rtcp, buffer);
                break;
        }

        if (err) {
            uint32_t header_type = rtcp->header.type;
            uint32_t header_rc = rtcp->header.rc;
            yang_free(rtcp);
            if (err == ERROR_RTC_RTCP_EMPTY_RR) {
                err = Yang_Ok;
                continue;
            }
            return yang_error_wrap(err, "decode rtcp type=%u rc=%u", header_type, header_rc);
        }

        yang_rtcpCompound_add_rtcp(rtcps, rtcp);
        yang_free(rtcp);
    }

    return err;
}

int32_t yang_encode_rtcpCompound(YangRtcpCompound* rtcps, YangBuffer* buffer) {
    if (rtcps == NULL) return 1;
    int32_t err = Yang_Ok;

    if (!yang_buffer_require(buffer, rtcps->nb_bytes)) {
        return yang_error_wrap(ERROR_RTC_RTCP, "requires %d bytes", rtcps->nb_bytes);
    }

    for (int i = 0; i < rtcps->rtcpVector.vsize; i++) {
        YangRtcpCommon* rtcp = &rtcps->rtcpVector.payload[i];

        switch (rtcp->header.type) {
            case YangRtcpType_sr:
                err = yang_encode_rtcpSR(rtcp, buffer);
                break;
            case YangRtcpType_rr:
                err = yang_encode_rtcpRR(rtcp, buffer);
                break;
            case YangRtcpType_rtpfb:
                if (rtcp->header.rc == 1) {
                    err = yang_encode_rtcpNack(rtcp, buffer);
                } else if (rtcp->header.rc == 15) {
                    // TWCC encoding logic here
                }
                break;
            case YangRtcpType_psfb:
                if (rtcp->header.rc == 1) {
                    err = yang_encode_rtcpPli(rtcp, buffer);
                } else if (rtcp->header.rc == 2) {
                    // SLI encoding logic here
                } else if (rtcp->header.rc == 3) {
                    err = yang_encode_rtcpRpsi(rtcp, buffer);
                } else {
                    err = yang_encode_rtcpPsfb(rtcp, buffer);
                }
                break;
            case YangRtcpType_xr:
                err = yang_encode_rtcpXr(rtcp, buffer);
                break;
            default:
                err = yang_encode_rtcpCommon(rtcp, buffer);
                break;
        }

        if (err) break;
    }

    yang_rtcpCompound_clear(rtcps);
    return err;
}

uint64_t yang_rtcpCompound_nb_bytes(YangRtcpCompound* rtcps) {
    return kRtcpPacketSize;
}

int32_t yang_rtcpCompound_add_rtcp(YangRtcpCompound* rtcps, YangRtcpCommon* rtcp) {
    if (rtcps == NULL || rtcp == NULL) return 1;
    yang_insert_YangRtcpCommonVector(&rtcps->rtcpVector, rtcp);
    return Yang_Ok;
}

void yang_rtcpCompound_clear(YangRtcpCompound* rtcps) {
    if (rtcps == NULL) return;
    if (rtcps->rtcpVector.vsize) {
        for (int i = 0; i < rtcps->rtcpVector.vsize; i++) {
            yang_destroy_rtcpCommon(&rtcps->rtcpVector.payload[i]);
        }
        yang_clear_YangRtcpCommonVector(&rtcps->rtcpVector);
    }
}

// By GST @Date