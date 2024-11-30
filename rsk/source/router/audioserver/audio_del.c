#include "audio_del.h"

// CDeleteAudioResult functions
void init_delete_audio_result(CDeleteAudioResult* result) {
    result->cmd = NULL;
    result->resultId = 0;
    result->msg = NULL;
    result->data = NULL;
    result->data_count = 0;
}

void free_delete_audio_result(CDeleteAudioResult* result) {
    free(result->cmd);
    free(result->msg);
    for (size_t i = 0; i < result->data_count; ++i) {
        free(result->data[i].fileName);
        free(result->data[i].audioId);
    }
    free(result->data);
}

void do_success(CDeleteAudioResult* result) {
    result->cmd = strdup("AudioDelete");
    result->resultId = 1;
    result->msg = strdup("AudioDelete handle success");

    CAddCustomAudioFileBusiness audios;
    load_add_custom_audio_file_business(&audios);

    CAudioCfgBusiness cfg;
    load_audio_cfg_business(&cfg);

    CUtils utils;
    init_utils(&utils);

    result->data = (CGetAudioData*)malloc(audios.business_count * sizeof(CGetAudioData));
    result->data_count = audios.business_count;

    for (size_t i = 0; i < audios.business_count; ++i) {
        CGetAudioData* v = &result->data[i];
        v->storageType = 0;
        v->type = 32;
        v->fileName = strdup(audios.business[i].customAudioName);
        v->size = get_size(&utils, get_audio_file_path(&cfg), audios.business[i].filename);
        v->audioId = strdup(audios.business[i].customAudioID);
    }

    free_add_custom_audio_file_business(&audios);
    free_audio_cfg_business(&cfg);
    free_utils(&utils);
}

// CDeleteAudio functions
void init_delete_audio(CDeleteAudio* del) {
    del->cmd = NULL;
    del->deleteName = NULL;
    del->storageType = 0;
}

void free_delete_audio(CDeleteAudio* del) {
    free(del->cmd);
    free(del->deleteName);
}

int do_del(const char* name, int type) {
    CAddCustomAudioFileBusiness audios;
    init_add_custom_audio_file_business(&audios);
    delete_audio(&audios, name);
    free_add_custom_audio_file_business(&audios);

    CAddColumnCustomAudioFileBusiness business;
    init_add_column_custom_audio_file_business(&business);
    delete_audio_column(&business, name);
    free_add_column_custom_audio_file_business(&business);

    return 0;
}

int do_req(CDeleteAudio* del, CSocket* pClient) {
    do_del(del->deleteName, del->storageType);

    CDeleteAudioResult res;
    init_delete_audio_result(&res);
    do_success(&res);

    json_t* json = json_object();
    json_object_set_new(json, "cmd", json_string(res.cmd));
    json_object_set_new(json, "resultId", json_integer(res.resultId));
    json_object_set_new(json, "msg", json_string(res.msg));

    json_t* data_array = json_array();
    for (size_t i = 0; i < res.data_count; ++i) {
        json_t* data_obj = json_object();
        json_object_set_new(data_obj, "storageType", json_integer(res.data[i].storageType));
        json_object_set_new(data_obj, "type", json_integer(res.data[i].type));
        json_object_set_new(data_obj, "fileName", json_string(res.data[i].fileName));
        json_object_set_new(data_obj, "size", json_integer(res.data[i].size));
        json_object_set_new(data_obj, "audioId", json_string(res.data[i].audioId));
        json_array_append_new(data_array, data_obj);
    }
    json_object_set_new(json, "data", data_array);

    char* json_str = json_dumps(json, JSON_INDENT(4));
    int result = send_socket(pClient, json_str, strlen(json_str));

    free(json_str);
    json_decref(json);
    free_delete_audio_result(&res);

    return result;
}