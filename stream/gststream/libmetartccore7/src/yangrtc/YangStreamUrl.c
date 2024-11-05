#include <yangrtc/YangStreamUrl.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangCUrl.h>
#include <yangutil/sys/YangMath.h>

int32_t yang_stream_parseUrl2(char* url, YangStreamConfig* stream, YangAVInfo* avinfo, YangRtcDirection opt) {
    int32_t err = Yang_Ok;
    YangUrlData data;
    yang_memset(&data, 0, sizeof(YangUrlData));

    if ((err = yang_http_url_parse(avinfo->sys.familyType, url, &data)) != Yang_Ok) {
        return yang_error_wrap(err, "stream parse url error");
    }

    if (opt == YangSendonly) {
        yang_memset(avinfo->sys.whipUrl, 0, sizeof(avinfo->sys.whipUrl));
        yang_strncpy(avinfo->sys.whipUrl, data.stream);
    } else if (opt == YangRecvonly) {
        yang_memset(avinfo->sys.whepUrl, 0, sizeof(avinfo->sys.whepUrl));
        yang_strncpy(avinfo->sys.whepUrl, data.stream);
    }

    yang_trace("\navinfo->sys.whipUrl==%s", avinfo->sys.whipUrl);

    stream->localPort = 10000 + yang_random() % 15000;
    yang_strncpy(stream->remoteIp, data.server);
    stream->remotePort = data.port;
    stream->uid = 0;
    stream->direction = opt;

    return err;
}

int32_t yang_stream_parseUrl(char* url, YangStreamConfig* stream, YangAVInfo* avinfo, YangRtcDirection opt) {
    int32_t err = Yang_Ok;
    YangUrlData data;
    yang_memset(&data, 0, sizeof(YangUrlData));

    if ((err = yang_url_parse(avinfo->sys.familyType, url, &data)) != Yang_Ok) {
        return yang_error_wrap(err, "stream parse url error");
    }

    stream->localPort = 10000 + yang_random() % 15000;
    yang_strncpy(stream->remoteIp, data.server);
    stream->remotePort = data.port;
    yang_strncpy(stream->app, data.app);
    yang_strncpy(stream->stream, data.stream);
    stream->uid = 0;
    stream->direction = opt;

    return err;
}

// By GST @Date