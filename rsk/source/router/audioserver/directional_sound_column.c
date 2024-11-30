#include "directional_sound_column.h"

// 初始化全局变量
int g_playing_priority = 0;
VolumeSet g_volumeSet;

// 设备基本信息成功处理函数
void device_base_info_do_success(DeviceBaseInfo* info) {
    AudioCfgBusiness cfg;
    audio_cfg_business_load(&cfg);
    CUtils util;
    info->codeVersion = strdup(cfg.business[0].codeVersion);
    info->coreVersion = strdup(util_get_core_version());
    info->relayMode = relay_get_instance()->getGpioModel();
    info->ip = strdup(util_get_addr());
    info->storageType = util_is_ros_platform() ? 0 : 1;
    info->port = 34508;
    info->playStatus = play_status_get_instance()->getPlayState();
    volume_set_load(&g_volumeSet);
    info->volume = volume_set_get_volume(&g_volumeSet);
    info->relayStatus = relay_get_instance()->getGpioStatus();
    info->hardwareReleaseTime = strdup(util_get_hardware_release_time());
    info->spiFreeSpace = info->storageType ? 9752500 : 0;
    info->flashFreeSpace = util_get_available_disk("/mnt");
    info->hardwareVersion = strdup("7621");
    info->hardwareModelId = 2;
    info->password = strdup(cfg.business[0].serverPassword);
    info->temperature = 12;
    info->netmask = strdup(util_get_netmask());
    info->address = strdup("01");
    info->gateway = strdup(util_get_gateway());
    info->userName = strdup("admin");
    info->imei = strdup(cfg.business[0].deviceID);
    info->functionVersion = strdup("COMMON");
    info->deviceCode = strdup(cfg.business[0].deviceID);
    info->serverAddress = strdup(cfg.business[0].server);
    info->serverPort = strdup(cfg.business[0].port);
}

// 设备基本状态成功处理函数
void device_base_state_do_success(DeviceBaseState* state) {
    state->relayStatus = util_get_process_status("madplay") || play_status_get_instance()->getPlayState();
    volume_set_load(&g_volumeSet);
    state->volume = volume_set_get_volume(&g_volumeSet);
    state->storageType = 1;
}

// 发送成功响应
int send_true(CSocket* pClient) {
    const char* res = "01 E1";
    printf("return: %s\n", res);
    if (!pClient) {
        return rs485_uart_write(res, strlen(res));
    }
    return csocket_send(pClient, res, strlen(res));
}

// 发送错误响应
int send_fast(const char* err_code, CSocket* pClient) {
    char buf[256];
    snprintf(buf, sizeof(buf), "01 %s", err_code);
    printf("return: %s\n", buf);
    if (!pClient) {
        return rs485_uart_write(buf, strlen(buf));
    }
    return csocket_send(pClient, buf, strlen(buf));
}

// 登录处理函数
int login(const char** m_str, CSocket* pClient) {
    AudioCfgBusiness cfg;
    audio_cfg_business_load(&cfg);
    printf("%s %s %s\n", m_str[4], cfg.business[0].serverPassword, m_str[5]);
    if (strcmp(m_str[4], "admin") == 0 && strcmp(m_str[5], cfg.business[0].serverPassword) == 0) {
        printf("return login ok\n");
        return send_true(pClient);
    } else {
        printf("return login error\n");
        return send_fast("USER_PWD_ERROR", pClient);
    }
}

// 音频播放处理函数
int audio_play(const char** m_str, CSocket* pClient) {
    if (util_get_process_status("madplay")) {
        return send_fast("ALREADY_PLAYED", pClient);
    }
    const char* name = m_str[4];
    AudioCfgBusiness business;
    if (!util_str_is_all_num(name) || atoi(name) > 100 || atoi(name) < 0) {
        return send_fast("OPERATION_NEW_AUDIO_FILE_NAME_ERROR", pClient);
    } else if (!audio_cfg_business_is_name_empty(&business, m_str[4])) {
        return send_fast("AUDIO_FILE_NOT_EXITS", pClient);
    } else if (g_playing_priority != 0) {
        return send_fast("ALREADY_PLAYED", pClient);
    } else {
        if (util_get_process_status("ffplay")) {
            audio_play_util_audio_stop();
        }
        AudioCfgBusiness cfg;
        audio_cfg_business_load(&cfg);
        audio_play_util_audio_loop_play(cfg_get_audio_file_path(&cfg), m_str[4], 1);
        return send_true(pClient);
    }
}

// 音频停止处理函数
int audio_stop(CSocket* pClient) {
    audio_play_util_audio_stop();
    return send_true(pClient);
}

// 音量设置处理函数
int volume_set(const char** m_str, CSocket* pClient) {
    int volume = atoi(m_str[4]);
    if (volume > 7 || volume < 0) {
        return send_fast("VOLUME_RANGE_ERROR", pClient);
    } else {
        VolumeSet volumeSet;
        volume_set_set_volume(&volumeSet, volume);
        volume_set_adjust(&volumeSet, volume);
        volume_set_save_to_json(&volumeSet);
        return send_true(pClient);
    }
}

// 重启处理函数
int reboot(CSocket* pClient) {
    send_true(pClient);
    util_reboot();
    return 1;
}

// 获取设备状态处理函数
int get_device_status(CSocket* pClient) {
    DeviceBaseState deviceState;
    device_base_state_do_success(&deviceState);
    json_t* res = json_pack("{s:i, s:i, s:i}", 
                            "relayStatus", deviceState.relayStatus, 
                            "volume", deviceState.volume, 
                            "storageType", deviceState.storageType);
    char* str = json_dumps(res, 0);
    char buf[256];
    snprintf(buf, sizeof(buf), "01 E1 %s", str);
    if (!pClient) {
        return rs485_uart_work(buf, strlen(buf));
    }
    return csocket_send(pClient, buf, strlen(buf));
}

// 恢复出厂设置处理函数
int restore(CSocket* pClient) {
    AudioCfgBusiness cfg;
    audio_cfg_business_load(&cfg);
    cfg.business[0].serverPassword = strdup("Aa123456");
    cfg.business[0].server = strdup("192.168.1.90");
    cfg.business[0].port = 7681;
    audio_cfg_business_save_to_json(&cfg);
    send_true(pClient);
    util_restore(cfg.business[0].savePrefix);
    return 1;
}

// 闪存配置处理函数
int flash_config(const char** m_str, CSocket* pClient) {
    relay_get_instance()->set_gpio_model(atoi(m_str[4]), atoi(m_str[5]));
    return send_true(pClient);
}

// TTS播放处理函数
int tts_play(const char** m_str, CSocket* pClient) {
    if (util_get_process_status("madplay") || util_get_process_status("ffplay") || play_status_get_instance()->getPlayState()) {
        return send_fast("ALREADY_PLAYED", pClient);
    }
    char* txt = util_hex_to_string(m_str[4]);
    if (util_statistical_character_count(txt) > 100) {
        return send_fast("TTS_TXT_LENGTH_ERROR", pClient);
    }
    send_true(pClient);
    audio_play_util_tts_loop_play(txt, 1);
    return 1;
}

// TTS按次数和时间播放处理函数
int tts_num_time_play(const char** m_str, CSocket* pClient) {
    if (util_get_process_status("madplay") || util_get_process_status("ffplay") ||
        util_get_process_status("tts") || play_status_get_instance()->getPlayState()) {
        return send_fast("ALREADY_PLAYED", pClient);
    }
    char* txt = util_hex_to_string(m_str[4]);
    if (util_statistical_character_count(txt) > 100) {
        return send_fast("TTS_TXT_LENGTH_ERROR", pClient);
    }
    printf("tts size: %zu txt: %s\n", strlen(txt), txt);
    int playType = atoi(m_str[5]);
    int duration = atoi(m_str[6]);
    util_async_wait(1, 0, 0, [=] {
        switch (playType) {
            case 0: {
                send_true(pClient);
                audio_play_util_tts_loop_play(txt);
                return 0;
            }
            case 1: {
                if (duration < 1) {
                    return send_fast("NONSUPPORT_ERROR", pClient);
                }
                send_true(pClient);
                audio_play_util_tts_num_play(duration, txt);
                break;
            }
            case 2: {
                if (duration < 1) {
                    return send_fast("NONSUPPORT_ERROR", pClient);
                }
                send_true(pClient);
                audio_play_util_tts_time_play(duration, txt);
                break;
            }
            default:
                return send_fast("NONSUPPORT_ERROR", pClient);
        }
        return 1;
    });
    return 1;
}