
#include <yangavutil/video/YangCMeta.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangEndian.h>
#include <yangutil/yangavtype.h>

void yang_find_start_code(YangVideoCodec pve, uint8_t *buf, int32_t bufLen, int32_t *vpsPos, int32_t *vpsLen, int32_t *spsPos, int32_t *spsLen, int32_t *ppsPos, int32_t *ppsLen) {
    int32_t i = 0;
    *spsPos = 0;
    *ppsPos = 0;
    if (pve == Yang_VED_H265) {
        *vpsPos = 0;
        while (i < bufLen - 3) {
            if (buf[i] == 0 && buf[i + 1] == 0 && buf[i + 2] == 0 && buf[i + 3] == 1) {
                *vpsPos = i + 4;
                i += 4;
                break;
            }
            i++;
        }
    }
    while (i < bufLen - 3) {
        if (buf[i] == 0 && buf[i + 1] == 0 && buf[i + 2] == 0 && buf[i + 3] == 1) {
            if (pve == Yang_VED_H265) *vpsLen = i - 4;
            *spsPos = i + 4;
            i += 4;
            break;
        }
        i++;
    }
    while (i < bufLen - 3) {
        if (buf[i] == 0 && buf[i + 1] == 0 && buf[i + 2] == 0 && buf[i + 3] == 1) {
            *spsLen = i - *spsPos;
            *ppsPos = i + 4;
            *ppsLen = bufLen - *ppsPos;
            break;
                  }
        i++;
    }
}

int32_t yang_find_pre_start_code(uint8_t *buf, int32_t bufLen) {
    for (int32_t i = 0; i < bufLen - 3; i++) {
        if (buf[i] == 0 && buf[i + 1] == 0 && buf[i + 2] == 0 && buf[i + 3] == 1) {
            return i;
        }
    }
    return -1;
}

void yang_createH264Meta(YangVideoMeta* pvmd, YangFrame *videoFrame) {
    yang_memset(pvmd, 0, sizeof(YangVideoMeta));
    uint32_t spslen = yang_get_be32(videoFrame->payload);
    uint32_t ppslen = yang_get_be32(videoFrame->payload + spslen + 4);
    pvmd->mp4Meta.spsLen = spslen;
    pvmd->mp4Meta.ppsLen = ppslen;
    yang_memcpy(pvmd->mp4Meta.sps, videoFrame->payload + 4, spslen);
    yang_memcpy(pvmd->mp4Meta.pps, videoFrame->payload + 4 + spslen + 4, ppslen);
}

void yang_createH265Meta(YangVideoMeta* pvmd, YangFrame *videoFrame) {
    yang_memset(pvmd, 0, sizeof(YangVideoMeta));
    uint32_t vpslen = yang_get_be32(videoFrame->payload);
    uint32_t spslen = yang_get_be32(videoFrame->payload + vpslen + 4);
    uint32_t ppslen = yang_get_be32(videoFrame->payload + vpslen + 4 + spslen + 4);
    pvmd->mp4Meta.vpsLen = vpslen;
    pvmd->mp4Meta.spsLen = spslen;
    pvmd->mp4Meta.ppsLen = ppslen;
    yang_memcpy(pvmd->mp4Meta.vps, videoFrame->payload + 4, v
请从33行续写
回复 yuJohnson: 
请从33行续写
好的，以下是从第33行续写的优化后的代码：
    yang_memcpy(pvmd->mp4Meta.vps, videoFrame->payload + 4, vpslen);
    yang_memcpy(pvmd->mp4Meta.sps, videoFrame->payload + 4 + vpslen + 4, spslen);
    yang_memcpy(pvmd->mp4Meta.pps, videoFrame->payload + 4 + vpslen + 4 + spslen + 4, ppslen);
}

void yang_getConfig_Flv_H264(YangH2645Conf *p_264, uint8_t *configBuf, int32_t *p_configLen) {
    configBuf[0] = 0x17;
    configBuf[1] = 0x00;
    configBuf[2] = 0x00;
    configBuf[3] = 0x00;
    configBuf[4] = 0x00;
    configBuf[5] = 0x01;
    configBuf[6] = p_264->sps[1];
    configBuf[7] = p_264->sps[2];
    configBuf[8] = p_264->sps[3];
    configBuf[9] = 0xff;
    configBuf[10] = 0xe1;
    uint8_t *szTmp = configBuf + 11;

    *szTmp++ = 0x00;
    *szTmp++ = p_264->spsLen;
    yang_memcpy(szTmp, p_264->sps, p_264->spsLen);
    szTmp += p_264->spsLen;
    *szTmp++ = 0x01;
    *szTmp++ = 0x00;
    *szTmp++ = p_264->ppsLen;
    yang_memcpy(szTmp, p_264->pps, p_264->ppsLen);
    szTmp += p_264->ppsLen;
    *p_configLen = szTmp - configBuf;
}

void yang_getConfig_Meta_H264(YangSample* psps, YangSample* ppps, uint8_t *configBuf, int32_t *p_configLen) {
    int32_t spsLen = psps->nb;
    int32_t ppsLen = ppps->nb;
    uint8_t* sps = (uint8_t*)psps->bytes;
    uint8_t* pps = (uint8_t*)ppps->bytes;

    configBuf[0] = 0x17;
    configBuf[1] = 0x00;
    configBuf[2] = 0x00;
    configBuf[3] = 0x00;
    configBuf[4] = 0x00;
    configBuf[5] = 0x01;
    configBuf[6] = sps[1];
    configBuf[7] = sps[2];
    configBuf[8] = sps[3];
    configBuf[9] = 0xff;
    configBuf[10] = 0xe1;
    uint8_t *szTmp = configBuf + 11;

    yang_put_be16((char*)szTmp, (uint16_t)spsLen);
    szTmp += 2;
    yang_memcpy(szTmp, sps, spsLen);
    szTmp += spsLen;
    *szTmp++ = 0x01;
    yang_put_be16((char*)szTmp, (uint16_t)ppsLen);
    szTmp += 2;
    yang_memcpy(szTmp, pps, ppsLen);
    szTmp += ppsLen;
    *p_configLen = szTmp - configBuf;
}

void yang_getConfig_Flv_H265(YangH2645Conf *p_264, uint8_t *configBuf, int32_t *p_configLen) {
    int32_t i = 0;
    configBuf[i++] = 0x1C;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = p_264->sps[1];
    configBuf[i++] = p_264->sps[2];
    configBuf[i++] = p_264->sps[3];
    configBuf[i++] = p_264->sps[4];
    configBuf[i++] = p_264->sps[5];
    configBuf[i++] = p_264->sps[6];
    configBuf[i++] = p_264->sps[7];
    configBuf[i++] = p_264->sps[8];
    configBuf[i++] = p_264->sps[9];
    configBuf[i++] = p_264->sps[10];
    configBuf[i++] = p_264->sps[11];
    configBuf[i++] = p_264->sps[12];
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x03;

    configBuf[i++] = 0x20;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x01;
    configBuf[i++] = (p_264->vpsLen >> 8) & 0xff;
    configBuf[i++] = p_264->vpsLen & 0xff;
    yang_memcpy(&configBuf[i], p_264->vps, p_264->vpsLen);
    i += p_264->vpsLen;

    configBuf[i++] = 0x21;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x01;
        configBuf[i++] = (p_264->spsLen >> 8) & 0xff;
    configBuf[i++] = p_264->spsLen & 0xff;
    yang_memcpy(&configBuf[i], p_264->sps, p_264->spsLen);
    i += p_264->spsLen;

    configBuf[i++] = 0x22;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x01;
    configBuf[i++] = (p_264->ppsLen >> 8) & 0xff;
    configBuf[i++] = p_264->ppsLen & 0xff;
    yang_memcpy(&configBuf[i], p_264->pps, p_264->ppsLen);
    i += p_264->ppsLen;

    *p_configLen = i;
}

void yang_getConfig_Meta_H265(YangSample* pvps, YangSample* psps, YangSample* ppps, uint8_t *configBuf, int32_t *p_configLen) {
    int32_t vpsLen = pvps->nb;
    int32_t spsLen = psps->nb;
    int32_t ppsLen = ppps->nb;
    uint8_t* vps = (uint8_t*)pvps->bytes;
    uint8_t* sps = (uint8_t*)psps->bytes;
    uint8_t* pps = (uint8_t*)ppps->bytes;

    int32_t i = 0;
    configBuf[i++] = 0x1C;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = sps[1];
    configBuf[i++] = sps[2];
    configBuf[i++] = sps[3];
    configBuf[i++] = sps[4];
    configBuf[i++] = sps[5];
    configBuf[i++] = sps[6];
    configBuf[i++] = sps[7];
    configBuf[i++] = sps[8];
    configBuf[i++] = sps[9];
    configBuf[i++] = sps[10];
    configBuf[i++] = sps[11];
    configBuf[i++] = sps[12];
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x03;

    configBuf[i++] = 0x20;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x01;
    configBuf[i++] = (vpsLen >> 8) & 0xff;
    configBuf[i++] = vpsLen & 0xff;
    yang_memcpy(&configBuf[i], vps, vpsLen);
    i += vpsLen;

    configBuf[i++] = 0x21;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x01;
    configBuf[i++] = (spsLen >> 8) & 0xff;
    configBuf[i++] = spsLen & 0xff;
    yang_memcpy(&configBuf[i], sps, spsLen);
    i += spsLen;

    configBuf[i++] = 0x22;
    configBuf[i++] = 0x00;
    configBuf[i++] = 0x01;
    configBuf[i++] = (ppsLen >> 8) & 0xff;
    configBuf[i++] = ppsLen & 0xff;
    yang_memcpy(&configBuf[i], pps, ppsLen);
    i += ppsLen;

    *p_configLen = i;
}
void yang_parseH264Header(uint8_t *buf, YangH2645Conf *p_264) {
    uint8_t *p = buf;
    p_264->spsLen = (p[6] << 8) | p[7];
    p_264->sps = (uint8_t *)malloc(p_264->spsLen);
    yang_memcpy(p_264->sps, p + 8, p_264->spsLen);

    p += 8 + p_264->spsLen;
    p_264->ppsLen = (p[0] << 8) | p[1];
    p_264->pps = (uint8_t *)malloc(p_264->ppsLen);
    yang_memcpy(p_264->pps, p + 2, p_264->ppsLen);
}

void yang_parseH265Header(uint8_t *buf, YangH2645Conf *p_265) {
    uint8_t *p = buf;
    p_265->vpsLen = (p[27] << 8) | p[28];
    p_265->vps = (uint8_t *)malloc(p_265->vpsLen);
    yang_memcpy(p_265->vps, p + 29, p_265->vpsLen);

    p += 29 + p_265->vpsLen;
    p_265->spsLen = (p[0] << 8) | p[1];
    p_265->sps = (uint8_t *)malloc(p_265->spsLen);
    yang_memcpy(p_265->sps, p + 2, p_265->spsLen);

    p += 2 + p_265->spsLen;
    p_265->ppsLen = (p[0] << 8) | p[1];
    p_265->pps = (uint8_t *)malloc(p_265->ppsLen);
    yang_memcpy(p_265->pps, p + 2, p_265->ppsLen);
}

void yang_freeH2645Conf(YangH2645Conf *p_2645) {
    if (p_2645->sps) {
        free(p_2645->sps);
        p_2645->sps = NULL;
    }
    if (p_2645->pps) {
        free(p_2645->pps);
        p_2645->pps = NULL;
    }
    if (p_2645->vps) {
        free(p_2645->vps);
        p_2645->vps = NULL;
    }
}
