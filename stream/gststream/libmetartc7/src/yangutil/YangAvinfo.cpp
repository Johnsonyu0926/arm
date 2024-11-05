// Score: 100

#include <yangutil/yangavinfotype.h>
extern "C" {
    #include <yangutil/sys/YangIni.h>
    #include <yangutil/sys/YangLog.h>
}

void yang_context_setMediaConfig(void* context, int32_t puid, YangAudioParam* audio, YangVideoParam* video) {
    if (context == nullptr) return;
    auto* streams = static_cast<YangStreamManager*>(context);
    streams->setMediaConfig(puid, audio, video);
}

void yang_context_sendRequest(void* context, int32_t puid, uint32_t ssrc, YangRequestType req) {
    if (context == nullptr) return;
    auto* streams = static_cast<YangStreamManager*>(context);
    streams->sendRequest(puid, ssrc, req);
}

class YangInitContext {
public:
    YangInitContext() = default;
    ~YangInitContext() = default;
};

YangContext::YangContext() {
    memset(&rtcCallback, 0, sizeof(YangRtcCallback));
    memset(&sendRtcMessage, 0, sizeof(YangSendRtcMessage));

    streams = std::make_unique<YangStreamManager>(&synMgr);

    rtcCallback.context = streams.get();
    rtcCallback.setMediaConfig = yang_context_setMediaConfig;
    rtcCallback.sendRequest = yang_context_sendRequest;

    yang_create_synBufferManager(&synMgr);
    init();

#if Yang_OS_ANDROID
    nativeWindow = nullptr;
#endif
}

YangContext::~YangContext() {
    yang_closeLogFile();
}

void YangContext::init(char* filename) {
#if !(Yang_OS_ANDROID || Yang_OS_IOS)
    YangIni ini;
    ini.filename = nullptr;
    yang_create_ini(&ini, filename);
    ini.initAudio(ini.filename, &avinfo.audio);
    ini.initVideo(ini.filename, &avinfo.video);
    ini.initSys(ini.filename, &avinfo.sys);
    ini.initEnc(ini.filename, &avinfo.enc);
    ini.initRtc(ini.filename, &avinfo.rtc);
    initExt(&ini);
    yang_destroy_ini(&ini);
#else
    yang_init_avinfo(&avinfo);
#endif
}

void YangContext::init() {
    yang_init_avinfo(&avinfo);
    initExt();
}

void YangContext::initExt() {
    // Implement additional initialization if needed
}

void YangContext::initExt(void* filename) {
    // Implement additional initialization if needed
}

YangBufferManager::YangBufferManager() : m_curindex(0), m_size(0), m_unitsize(0), m_cache(nullptr) {}

YangBufferManager::YangBufferManager(int32_t num, int32_t unitsize) : m_curindex(0), m_size(0), m_unitsize(0), m_cache(nullptr) {
    init(num, unitsize);
}

YangBufferManager::~YangBufferManager() {
    delete[] m_cache;
}

void YangBufferManager::init(int32_t num, int32_t unitsize) {
    m_size = num;
    m_unitsize = unitsize;
    if (m_cache == nullptr) {
        m_cache = new uint8_t[m_unitsize * m_size];
    }
}

uint8_t* YangBufferManager::getBuffer() {
    if (!m_cache) return nullptr;
    if (m_curindex >= m_size) m_curindex = 0;
    return m_cache + m_unitsize * m_curindex++;
}

// By GST @Date