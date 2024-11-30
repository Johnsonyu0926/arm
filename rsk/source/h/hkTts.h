#ifndef HK_TTS_H
#define HK_TTS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* apiKey;
    char* url;
    char* voice;
} TtsConfig;

typedef struct {
    TtsConfig config;
    char* configPath;
} TtsConfigManager;

TtsConfig* create_tts_config(const char* apiKey, const char* url, const char* voice);
void destroy_tts_config(TtsConfig* config);
const char* get_api_key(const TtsConfig* config);
void set_api_key(TtsConfig* config, const char* newApiKey);
const char* get_url(const TtsConfig* config);
void set_url(TtsConfig* config, const char* newUrl);
const char* get_voice(const TtsConfig* config);
void set_voice(TtsConfig* config, const char* newVoice);
json_t* tts_config_to_json(const TtsConfig* config);
TtsConfig* tts_config_from_json(const json_t* json);

TtsConfigManager* create_tts_config_manager(const char* configPath);
void destroy_tts_config_manager(TtsConfigManager* manager);
bool load_tts_config_manager(TtsConfigManager* manager);
bool save_tts_config_manager(const TtsConfigManager* manager);
const TtsConfig* get_tts_config(const TtsConfigManager* manager);
void set_tts_config(TtsConfigManager* manager, TtsConfig* newConfig);

#endif // HK_TTS_H