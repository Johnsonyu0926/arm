#ifndef __START_VOICE_TALK_HPP__
#define __START_VOICE_TALK_HPP__

#include <iostream>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;


/*
{
    "status": 200,
    "code": "0x00000000",
    "errorMsg": "success!",
    "data": {
        "AudioCodeInfo": {
            "type": 25
        },
        "sessionID": "3001"
    }
}
*/
namespace asns{
		
class CAudioCodeInfo{
    private:
            int type;
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioCodeInfo, type)
};
class CStartVoiceTalkData{
	private:
        CAudioCodeInfo AudioCodeInfo;
        string sessionID;
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CStartVoiceTalkData, AudioCodeInfo, sessionID)
};

class CStartVoiceTalkBusiness
{
    private:
        CStartVoiceTalkData startVoiceTalk;
    public:
        int parse(string req) {
        }
};
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"


char *create_start_audio_play_reply(void)
{
    int audio_code_info_type = 25;
    char sessionID[]="3001";
    int status = 200;
    char code[] = "0x00000000";
    char errorMsg[] = "success!";

    char *string = NULL;

    cJSON *monitor = cJSON_CreateObject();

    printf("success %d\n",__LINE__);
    if (cJSON_AddNumberToObject(monitor, "status", status) == NULL) {
        printf("error %d\n",__LINE__);
        goto end;
    }

    printf("success %d\n",__LINE__);
    if (cJSON_AddStringToObject(monitor, "code", code) == NULL)
    {
        printf("error %d\n",__LINE__);
        goto end;
    }

    printf("success %d\n",__LINE__);
    if (cJSON_AddStringToObject(monitor, "errorMsg", errorMsg) == NULL)
    {
        printf("error %d\n",__LINE__);
        goto end;
    }

    printf("success %d\n",__LINE__);
    cJSON* data = cJSON_CreateObject();
    printf("success %d\n",__LINE__);
    cJSON_AddItemToObject(monitor, "data", data);

    printf("success %d\n",__LINE__);

    cJSON* audio_code_info = cJSON_CreateObject();
    cJSON_AddItemToObject(data, "AudioCodeInfo", audio_code_info);
    if (cJSON_AddNumberToObject(audio_code_info, "type", audio_code_info_type) == NULL) {
        goto end;
    }

    printf("success %d\n",__LINE__);
    if (cJSON_AddStringToObject(data, "sessionID", sessionID) == NULL)
    {
        printf("error %d\n",__LINE__);
        goto end;
    }

/*
    resolutions = cJSON_AddArrayToObject(monitor, "resolutions");
    if (resolutions == NULL)
    {
        goto end;
    }

    for (index = 0; index < (sizeof(resolution_numbers) / (2 * sizeof(int))); ++index)
    {
        cJSON *resolution = cJSON_CreateObject();

        if (cJSON_AddNumberToObject(resolution, "width", resolution_numbers[index][0]) == NULL)
        {
            goto end;
        }

        if (cJSON_AddNumberToObject(resolution, "height", resolution_numbers[index][1]) == NULL)
        {
            goto end;
        }

        cJSON_AddItemToArray(resolutions, resolution);
    }
*/

    string = cJSON_Print(monitor);
    if (string == NULL)
    {
        fprintf(stderr, "Failed to print monitor.\n");
    }

end:
    cJSON_Delete(monitor);
    printf("create success!\n");
    return string;
}
int main()
{
    printf("begin create reply...\n");
    char* string = create_start_audio_play_reply();
    printf("reply:%s, free it!\n", string);
    free(string);

}

#endif
