#include <yangsdp/YangRtcSdp.h>

#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangSsrc.h>
#include <yangutil/sys/YangCString.h>

#include <yangssl/YangSsl.h>
#include <yangsdp/YangMediaDesc.h>
#include <yangsdp/YangMediaPayloadType.h>

#define kCRLF "\r\n"

void yang_create_rtcsdp(YangSdp* sdp) {
    if (sdp == NULL) return;
    sdp->in_media_session = yangfalse;
    yang_itoa(0, sdp->version, 10);
    sdp->start_time = 0;
    sdp->end_time = 0;
}

void yang_destroy_rtcsdp(YangSdp* sdp) {
    if (sdp == NULL) return;
    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        yang_destroy_mediadesc(&sdp->media_descs.payload[i]);
    }
    yang_destroy_YangMediaDescVector(&sdp->media_descs);
}

char* yang_rtcsdp_getnull(char* os) {
    char* p = os;
    while (*p != 0) {
        p++;
    }
    return p;
}

YangMediaDesc* yang_rtcsdp_find_media_descs(YangSdp* sdp, const char* type) {
    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        if (yang_strcmp(sdp->media_descs.payload[i].type, type) == 0) {
            return &sdp->media_descs.payload[i];
        }
    }
    return NULL;
}

void yang_rtcsdp_set_ice_ufrag(YangSdp* sdp, const char* ufrag) {
    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        yang_strncpy(sdp->media_descs.payload[i].session_info.ice_ufrag, ufrag, sizeof(sdp->media_descs.payload[i].session_info.ice_ufrag) - 1);
    }
}

void yang_rtcsdp_set_ice_pwd(YangSdp* sdp, const char* pwd) {
    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        yang_strncpy(sdp->media_descs.payload[i].session_info.ice_pwd, pwd, sizeof(sdp->media_descs.payload[i].session_info.ice_pwd) - 1);
    }
}

void yang_rtcsdp_set_dtls_role(YangSdp* sdp, const char* dtls_role) {
    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        yang_strncpy(sdp->media_descs.payload[i].session_info.setup, dtls_role, sizeof(sdp->media_descs.payload[i].session_info.setup) - 1);
    }
}

void yang_rtcsdp_set_fingerprint_algo(YangSdp* sdp, const char* algo) {
    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        yang_strncpy(sdp->media_descs.payload[i].session_info.fingerprint_algo, algo, sizeof(sdp->media_descs.payload[i].session_info.fingerprint_algo) - 1);
    }
}

void yang_rtcsdp_set_fingerprint(YangSdp* sdp, const char* fingerprint) {
    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        yang_strncpy(sdp->media_descs.payload[i].session_info.fingerprint, fingerprint, sizeof(sdp->media_descs.payload[i].session_info.fingerprint) - 1);
    }
}

void yang_rtcsdp_add_candidate(YangSdp* sdp, const char* ip, int port, const char* type) {
    // @see: https://tools.ietf.org/id/draft-ietf-mmusic-ice-sip-sdp-14.html#rfc.section.5.1
    YangCandidate candidate;

    yang_strncpy(candidate.ip, ip, sizeof(candidate.ip) - 1);
    candidate.port = port;
    yang_strncpy(candidate.type, type, sizeof(candidate.type) - 1);

    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        yang_insert_YangCandidateVector(&sdp->media_descs.payload[i].candidates, &candidate);
    }
}

const char* yang_rtcsdp_get_ice_ufrag(YangSdp* sdp) {
    // Because we use BUNDLE, so we can choose the first element.
    if (sdp->media_descs.vsize > 0) {
        return sdp->media_descs.payload[0].session_info.ice_ufrag;
    }
    return "";
}

const char* yang_rtcsdp_get_ice_pwd(YangSdp* sdp) {
    // Because we use BUNDLE, so we can choose the first element.
    if (sdp->media_descs.vsize > 0) {
        return sdp->media_descs.payload[0].session_info.ice_pwd;
    }
    return "";
}

const char* yang_rtcsdp_get_dtls_role(YangSdp* sdp) {
    // Because we use BUNDLE, so we can choose the first element.
    if (sdp->media_descs.vsize > 0) {
        return sdp->media_descs.payload[0].session_info.setup;
    }
    return "";
}

int32_t yang_rtcsdp_parse_origin(YangSdp* sdp, const char* content) {
    int32_t err = Yang_Ok;
    YangStrings str;
    yang_cstr_split(content, " ", &str);
    if (str.vsize < 5) return 1;
    // @see: https://tools.ietf.org/html/rfc4566#section-5.2
    // o=<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
    // eg. o=- 9164462281920464688 2 IN IP4 127.0.0.1

    yang_strncpy(sdp->username, str.str[0], sizeof(sdp->username) - 1);
    yang_strncpy(sdp->session_id, str.str[1], sizeof(sdp->session_id) - 1);
    yang_strncpy(sdp->session_version, str.str[2], sizeof(sdp->session_version) - 1);
    yang_strncpy(sdp->nettype, str.str[3], sizeof(sdp->nettype) - 1);
    yang_strncpy(sdp->addrtype, str.str[4], sizeof(sdp->addrtype) - 1);
    yang_strncpy(sdp->unicast_address, str.str[5], sizeof(sdp->unicast_address) - 1);

    yang_destroy_strings(&str);
    return err;
}

int32_t yang_rtcsdp_parse_version(YangSdp* sdp, const char* content) {
    int32_t err = Yang_Ok;
    // @see: https://tools.ietf.org/html/rfc4566#section-5.1
    if (yang_strlen(content)) {
        yang_strncpy(sdp->version, content, sizeof(sdp->version) - 1);
    } else {
        yang_itoa(0, sdp->version, 10);
    }
    return err;
}

int32_t yang_rtcsdp_parse_session_name(YangSdp* sdp, const char* content) {
    int32_t err = Yang_Ok;
    // @see: https://tools.ietf.org/html/rfc4566#section-5.3
    // s=<session name>
    yang_strncpy(sdp->session_name, content, sizeof(sdp->session_name) - 1);
    return err;
}

int32_t yang_rtcsdp_parse_timing(YangSdp* sdp, const char* content) {
    int32_t err = Yang_Ok;
    // @see: https://tools.ietf.org/html/rfc4566#section-5.9
    // t=<start-time> <stop-time>
    YangStrings str;
    yang_cstr_split(content, " ", &str);

    if (str.vsize < 2) return 1;

    sdp->start_time = atol(str.str[0]);
    sdp->end_time = atol(str.str[1]);

    yang_destroy_strings(&str);
    return err;
}

int32_t yang_rtcsdp_parse_attr_group(YangSdp* sdp, const char* value) {
    int32_t err = Yang_Ok;
    // @see: https://tools.ietf.org/html/rfc5888#section-5
    YangStrings str;
    yang_cstr_split(value, " ", &str);
    if (str.vsize == 0) return 1;

    yang_strncpy(sdp->group_policy, str.str[0], sizeof(sdp->group_policy) - 1);
    for (int i = 1; i < str.vsize; i++) {
        yang_insert_stringVector(&sdp->groups, str.str[i]);
    }

    yang_destroy_strings(&str);
    return err;
}

int32_t yang_rtcsdp_parse_media_description(YangSdp* sdp, const char* content) {
    int32_t err = Yang_Ok;
    // @see: https://tools.ietf.org/html/rfc4566#section-5.14
    // m=<media> <port> <proto> <fmt> ...
    // m=<media> <port>/<number of ports> <proto> <fmt> ...

    YangStrings str;
    yang_cstr_split(content, " ", &str);
    if (str.vsize < 4) return yang_error_wrap(1, "rtcsdp_parse_media_description error content=%s", content);

    YangMediaDesc md;
    yang_memset(&md, 0, sizeof(YangMediaDesc));
    yang_insert_YangMediaDescVector(&sdp->media_descs, &md);
    YangMediaDesc* desc = &sdp->media_descs.payload[sdp->media_descs.vsize - 1];
    yang_strncpy(desc->type, str.str[0], sizeof(desc->type) - 1);
    yang_strncpy(desc->protos, str.str[2], sizeof(desc->protos) - 1);
    desc->port = atoi(str.str[1]);
    for (int32_t i = 3; i < str.vsize; i++) {
        YangMediaPayloadType pt;
        yang_memset(&pt, 0, sizeof(YangMediaPayloadType));
        pt.payload_type = atoi(str.str[i]);
        yang_insert_YangMediaPayloadTypeVector(&desc->payload_types, &pt);
        yang_create_stringVector(&desc->payload_types.payload[desc->payload_types.vsize - 1].rtcp_fb);
    }

    if (!sdp->in_media_session) {
        sdp->in_media_session = yangtrue;
    }
    yang_destroy_strings(&str);
    return err;
}

int32_t yang_rtcsdp_parse_attribute(YangSdp* sdp, const char* content) {
    int32_t err = Yang_Ok;
    // @see: https://tools.ietf.org/html/rfc4566#section-5.13
    // a=<attribute>
    // a=<attribute>:<value>
    const char* p = yang_strstr(content, ":");

    char attribute[256];
    char value[256];
    yang_memset(attribute, 0, sizeof(attribute));
    yang_memset(value, 0, sizeof(value));
    if (p) {
        yang_memcpy(attribute, content, p - content);
        yang_strncpy(value, p + 1, sizeof(value) - 1);
    }
    if (yang_strcmp(attribute, "group") == 0) {
        return yang_rtcsdp_parse_attr_group(sdp, value);
    } else if (yang_strcmp(attribute, "msid-semantic") == 0) {
        YangStrings str;
        yang_cstr_split(value, " ", &str);
        yang_strncpy(sdp->msid_semantic, str.str[0], sizeof(sdp->msid_semantic) - 1);
        for (int j = 1; j < str.vsize; j++) {
            yang_insert_stringVector(&sdp->msids, str.str[j]);
        }
        yang_destroy_strings(&str);
    } else {
        return yang_sessioninfo_parse_attribute(&sdp->session_info, attribute, value);
    }

    return err;
}

int32_t yang_rtcsdp_is_unified(YangSdp* sdp) {
    return sdp->media_descs.vsize > 2 ? 1 : 0;
}

int32_t yang_rtcsdp_update_msid(YangSdp* sdp, const char* id) {
    int32_t err = Yang_Ok;
    yang_clear_stringVector(&sdp->msids);
    yang_insert_stringVector(&sdp->msids, id);

    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        YangMediaDesc* desc = &sdp->media_descs.payload[i];
        if ((err = yang_mediadesc_update_msid(desc, id)) != Yang_Ok) {
            yang_trace("\ndesc %s update msid %s", desc->mid, id);
            return err;
        }
    }
    return err;
}

int32_t yang_rtcsdp_parse_line(YangSdp* sdp, const char* line) {
    int32_t err = Yang_Ok;
    const char* content = line + 2;

    switch (line[0]) {
        case 'o': {
            return yang_rtcsdp_parse_origin(sdp, content);
        }
        case 'v': {
            return yang_rtcsdp_parse_version(sdp, content);
        }
        case 's': {
            return yang_rtcsdp_parse_session_name(sdp, content);
        }
        case 't': {
            return yang_rtcsdp_parse_timing(sdp, content);
        }
        case 'a': {
            if (sdp->in_media_session) {
                return yang_mediadesc_parse_line(&sdp->media_descs.payload[sdp->media_descs.vsize - 1], line);
            }
            return yang_rtcsdp_parse_attribute(sdp, content);
        }
        case 'm': {
            return yang_rtcsdp_parse_media_description(sdp, content);
        }
        case 'c': {
            break;
        }
        default: {
            yang_trace("ignore sdp line=%s", line);
            break;
        }
    }
    return err;
}

int32_t yang_rtcsdp_encode(YangSdp* sdp, YangBuffer* os) {
    int32_t err = Yang_Ok;
    char tmp[2048];
    yang_memset(tmp, 0, sizeof(tmp));
    yang_sprintf(tmp, "v=%s%s"
                      "o=%s %s %s %s %s %s%s"
                      "s=%s%s"
                      "t=%" PRId64 " %" PRId64 "%s"
                      "a=ice-lite%s",
                 sdp->version, kCRLF, sdp->username, sdp->session_id,
                 sdp->session_version, sdp->nettype, sdp->addrtype,
                 sdp->unicast_address, kCRLF, sdp->session_name, kCRLF,
                 sdp->start_time, sdp->end_time, kCRLF,
                 kCRLF);

    if (sdp->groups.vsize > 0) {
        char* p = yang_rtcsdp_getnull(tmp);
        yang_sprintf(p, "a=group:%s", sdp->group_policy);
        for (int i = 0; i < sdp->groups.vsize; i++) {
            p = yang_rtcsdp_getnull(tmp);
            yang_sprintf(p, " %s", sdp->groups.payload[i]);
        }
        p = yang_rtcsdp_getnull(tmp);
        yang_sprintf(p, "%s", kCRLF);
    }

    char* p = yang_rtcsdp_getnull(tmp);
    yang_sprintf(p, "a=msid-semantic: %s", sdp->msid_semantic);
    for (int i = 0; i < sdp->msids.vsize; i++) {
        p = yang_rtcsdp_getnull(tmp);
        yang_sprintf(p, " %s", sdp->msids.payload[i]);
    }
    p = yang_rtcsdp_getnull(tmp);
    yang_sprintf(p, "%s", kCRLF);

    yang_write_cstring(os, tmp);
    if ((err = yang_encode_sessionInfo(&sdp->session_info, os)) != Yang_Ok) {
        return printf("encode session info failed");
    }

    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        if ((err = yang_encode_mediadesc(&sdp->media_descs.payload[i], os)) != Yang_Ok) {
            return printf("encode media description failed");
        }
    }
    return err;
}

int32_t yang_rtcsdp_parse(YangSdp* sdp, const char* sdp_str) {
    int32_t err = Yang_Ok;
    YangStrings strs;

    yang_cstr_split(sdp_str, "\n", &strs);
    // printf("\nsdp===%s\n",sdp_str.c_str());
    // All webrtc SrsSdp annotated example
    // @see: https://tools.ietf.org/html/draft-ietf-rtcweb-SrsSdp-11
    // Sdp example
    // session info
    // v=
    // o=
    // s=
    // t=
    // media description
    // m=
    // a=
    // ...
    // media description
    // m=
    // a=
    // ...
    // std::istringstream is(sdp_str);
    // std::string line;
    for (int i = 0; i < strs.vsize; i++) {
        const char* line = strs.str[i];
        // yang_trace("%s", line.c_str());
        if (yang_strlen(line) < 2 || line[1] != '=') {
            return yang_error_wrap(ERROR_RTC_SDP_DECODE, "invalid sdp line=%s", line);
        }
         if ((err = yang_rtcsdp_parse_line(sdp, line)) != Yang_Ok) {
            return yang_error_wrap(1, "parse sdp line failed:%s", line);
        }
    }

    // The msid/tracker/mslabel is optional for SSRC, so we copy it when it's empty.
    for (int i = 0; i < sdp->media_descs.vsize; i++) {
        YangMediaDesc* media_desc = &sdp->media_descs.payload[i];
        for (size_t j = 0; j < media_desc->ssrc_infos.vsize; ++j) {
            YangSSRCInfo* ssrc_info = &media_desc->ssrc_infos.payload[j];
            if (yang_strlen(ssrc_info->msid) == 0) {
                yang_strncpy(ssrc_info->msid, media_desc->msid, sizeof(ssrc_info->msid) - 1);
            }
            if (yang_strlen(ssrc_info->msid_tracker) == 0) {
                yang_strncpy(ssrc_info->msid_tracker, media_desc->msid_tracker, sizeof(ssrc_info->msid_tracker) - 1);
            }
            if (yang_strlen(ssrc_info->mslabel) == 0) {
                yang_strncpy(ssrc_info->mslabel, media_desc->msid, sizeof(ssrc_info->mslabel) - 1);
            }
            if (yang_strlen(ssrc_info->label) == 0) {
                yang_strncpy(ssrc_info->label, media_desc->msid_tracker, sizeof(ssrc_info->label) - 1);
            }
        }
    }
    yang_destroy_strings(&strs);
    return err;
}

// By GST @Date