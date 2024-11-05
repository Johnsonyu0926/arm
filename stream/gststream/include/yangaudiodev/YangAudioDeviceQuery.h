// Filename: YangAudioDeviceQuery.h
// Score: 95

#ifndef INCLUDE_YANGAUDIODEV_YANGAUDIODEVICEQUERY_H_
#define INCLUDE_YANGAUDIODEV_YANGAUDIODEVICEQUERY_H_

#include <vector>
#include <string>

using namespace std;

struct YangAudioDevice {
    string name;
    string deviceName;
    string subName;
    int32_t aIndex;
    int32_t aSubIndex;
    int32_t aIdx;
};

class YangAudioDeviceQuery {
public:
    YangAudioDeviceQuery();
    virtual ~YangAudioDeviceQuery();
    vector<YangAudioDevice>* getCaptureDeviceList();
    vector<YangAudioDevice>* getPlayDeviceList();

private:
    vector<YangAudioDevice> m_captureDeviceList;
    vector<YangAudioDevice> m_playDeviceList;
    void getDeviceList(int32_t stream, vector<YangAudioDevice>* plist);
};

#endif /* INCLUDE_YANGAUDIODEV_YANGAUDIODEVICEQUERY_H_ */
// By GST @Date