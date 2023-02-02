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

    const char *const RECORD_PATH = "/tmp/record.mp3";
    const char *const FIRMWARE_PATH = "/var/run/SONICCOREV100R001.bin";
}