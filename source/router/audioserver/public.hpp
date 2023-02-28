#pragma once
namespace asns {
    const int BUFSIZE = 8192;

    const int AUDIO_FILE_TYPE = 32;

    const int BEGINPORT = 34509;
    const int ENDPORT = 34608;

    const int TCPNETWORKSET      = 101;
    const int UPDATEPWD          = 102;
    const int NETWORKSET         = 104;
    const int LOGIN              = 103;
    const int SETDEVICEADDRRESS  = 0;
    const int AUDIOPLAY = 1;
    const int AUDIOSTOP = 2;
    const int VOLUMSET = 3;
    const int REBOOT = 4;
    const int GETDEVICESTATUS = 5;
    const int TTSPLAY = 6;
    const int LIGHTSWITCH = 7;
    const int FILEUPLOAD = 8;
    const int RESTORE = 9;
    const int AUDIONUMORTPLAY = 10;
    const int TTSNUMORTPLAY = 11;
    const int GETDEVICEBASEINFO = 12;
    const int RECORD = 13;
    const int REMOTEUPGRADE = 14;
    const int GETAUDIOLIST = 15;
    const int LIGHTCONFIG = 16;
    const int RECORDBEGIN = 18;
    const int RECORDEND = 19;
    const int AUDIOFILEUPLOAD = 20;
    const int REMOTEFILEUPGRADE = 21;

    const char *const BEGIN_END_CODE_ERROR  = "F1";
    const char *const LENGTH_ERROR          = "F2";
    const char *const CHECK_CODE_ERROR      = "F3";
    const char *const NONSUPPORT_ERROR      = "F4";
    const char *const OPERATION_FAIL_ERROR  = "F5";
    const char *const USER_PWD_ERROR        = "F6";
    //const char *const TCP_TIMEOUT = "F7";
    const char *const OPERATION_NEW_IP_ERROR = "F11";
    const char *const OPERATION_NEW_AUDIO_FILE_NAME_ERROR = "F21";
    const char *const ALREADY_PLAYED = "F22";
    const char *const AUDIO_FILE_NOT_EXITS = "F23";
    const char *const NETWORK_SET_ERROR = "F24";
    const char *const OPERATION_NEW_PWD_COMPLEXITY_ERROR = "F25";
    const char *const NEW_PWD_SET_ERROR = "F26";
    const char *const AUDIO_SUFFIX_ERROR = "F27";
    const char *const DOWNLOAD_FILE_ERROR = "F28";
    const char *const FLASH_LIGHT_SET_ERROR = "F30";
    const char *const VOLUME_RANGE_ERROR = "F31";
    const char *const TTS_TXT_LENGTH_ERROR = "F41";
    const char *const RECORD_TIME_ERROR = "F60";
    const char *const POST_ERROR = "F61";
    const char *const GET_ERROR = "F65";
    const char *const RECORD_SIZE_ERROR = "F62";
    const char *const TCP_TRANSFER_ERROR = "F66";
    const char* const TCP_UPGRADE_ERROR = "F70";
    const char *const TCP_TIMEOUT = "F71";

    const char *const RECORD_PATH = "/tmp/record.mp3";
    const char *const FIRMWARE_PATH = "/var/run/SONICCOREV100R001.bin";

    const std::string TTS_PATH = "/tmp/output.wav";

    const int STOP_TASK_PLAYING = -1;
    const int TIMED_TASK_PLAYING = 1;
    const int TALK_TASK_PLAYING = 2;
    const int AUDIO_TASK_PLAYING = 3;

    const bool ASYNC_START = true;

    const int GPIO_CUSTOM_MODE = 1;
    const int GPIO_PLAY_MODE = 2;

    const int GPIO_CLOSE = 0;
    const int GPIO_OPEN = 1;

    const int MQTT_HEART_BEAT_TIME = 10;

    const int FEMALE = 0;
    const int MALE = 1;
}