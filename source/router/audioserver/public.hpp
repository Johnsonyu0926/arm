#pragma once

#include <string>

namespace asns {
    constexpr int BUFSIZE = 8192;
    constexpr int AUDIO_FILE_TYPE = 32;
    constexpr int BEGINPORT = 34509;
    constexpr int ENDPORT = 34608;

    constexpr int TCPNETWORKSET = 101;
    constexpr int UPDATEPWD = 102;
    constexpr int NETWORKSET = 104;
    constexpr int LOGIN = 103;
    constexpr int SETDEVICEADDRRESS = 0;
    constexpr int AUDIOPLAY = 1;
    constexpr int AUDIOSTOP = 2;
    constexpr int VOLUMSET = 3;
    constexpr int REBOOT = 4;
    constexpr int GETDEVICESTATUS = 5;
    constexpr int TTSPLAY = 6;
    constexpr int LIGHTSWITCH = 7;
    constexpr int FILEUPLOAD = 8;
    constexpr int RESTORE = 9;
    constexpr int AUDIONUMORTPLAY = 10;
    constexpr int TTSNUMORTPLAY = 11;
    constexpr int GETDEVICEBASEINFO = 12;
    constexpr int RECORD = 13;
    constexpr int REMOTEUPGRADE = 14;
    constexpr int GETAUDIOLIST = 15;
    constexpr int LIGHTCONFIG = 16;
    constexpr int RECORDBEGIN = 18;
    constexpr int RECORDEND = 19;
    constexpr int AUDIOFILEUPLOAD = 20;
    constexpr int REMOTEFILEUPGRADE = 21;
    constexpr int DSPMANAGE = 22;

    constexpr const char *BEGIN_END_CODE_ERROR = "F1";
    constexpr const char *LENGTH_ERROR = "F2";
    constexpr const char *CHECK_CODE_ERROR = "F3";
    constexpr const char *NONSUPPORT_ERROR = "F4";
    constexpr const char *OPERATION_FAIL_ERROR = "F5";
    constexpr const char *USER_PWD_ERROR = "F6";
    constexpr const char *OPERATION_NEW_IP_ERROR = "F11";
    constexpr const char *OPERATION_NEW_AUDIO_FILE_NAME_ERROR = "F21";
    constexpr const char *ALREADY_PLAYED = "F22";
    constexpr const char *AUDIO_FILE_NOT_EXITS = "F23";
    constexpr const char *NETWORK_SET_ERROR = "F24";
    constexpr const char *OPERATION_NEW_PWD_COMPLEXITY_ERROR = "F25";
    constexpr const char *NEW_PWD_SET_ERROR = "F26";
    constexpr const char *AUDIO_SUFFIX_ERROR = "F27";
    constexpr const char *DOWNLOAD_FILE_ERROR = "F28";
    constexpr const char *FLASH_LIGHT_SET_ERROR = "F30";
    constexpr const char *VOLUME_RANGE_ERROR = "F31";
    constexpr const char *TTS_TXT_LENGTH_ERROR = "F41";
    constexpr const char *RECORD_TIME_ERROR = "F60";
    constexpr const char *POST_ERROR = "F61";
    constexpr const char *GET_ERROR = "F65";
    constexpr const char *RECORD_SIZE_ERROR = "F62";
    constexpr const char *TCP_TRANSFER_ERROR = "F66";
    constexpr const char *TCP_UPGRADE_ERROR = "F70";
    constexpr const char *TCP_TIMEOUT = "F71";
    constexpr const char *LACK_SPACE = "F72";
    constexpr const char *AUDIO_SIZE_LARGE = "F73";

    constexpr const char *RECORD_PATH = "/tmp/record.mp3";
    constexpr const char *FIRMWARE_PATH = "/var/run/SONICCOREV100R001.bin";

    const std::string TTS_PATH = "/tmp/output.wav";

    constexpr int STOP_TASK_PLAYING = -1;
    constexpr int TIMED_TASK_PLAYING = 1;
    constexpr int TALK_TASK_PLAYING = 2;
    constexpr int AUDIO_TASK_PLAYING = 3;
    constexpr int TTS_TASK_PLAYING = 4;

    constexpr bool ASYNC_START = true;

    constexpr int SINGLE_PLAY = 1;

    constexpr int GPIO_CUSTOM_MODE = 1;
    constexpr int GPIO_PLAY_MODE = 2;

    constexpr int GPIO_CLOSE = 0;
    constexpr int GPIO_OPEN = 1;

    constexpr int MQTT_HEART_BEAT_TIME = 10;

    constexpr int FEMALE = 0;
    constexpr int MALE = 1;

    constexpr int RECORD_TIME_COMPENSATE = 2;
    const std::string RECORD_CMD = "ffmpeg -t 200 -y -f alsa -sample_rate 44100 -i hw:0,0 -acodec libmp3lame -ar 8k /tmp/record.mp3";

    inline std::string REQUEST_TOPIC;
    constexpr int DISK_SIZE = 5120;

    const std::string led_check = "55AA0000";
} // namespace asns