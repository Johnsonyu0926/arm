#ifndef H_MQTT_KERNEL_DEF_H_
#define H_MQTT_KERNEL_DEF_H_

#include <stdio.h>

#if !defined(MAX_MESSAGE_HANDLERS)
#define MAX_MESSAGE_HANDLERS 5 /* redefinable - how many subscriptions do you want? */
#endif

typedef void *ezdev_sdk_net_work;
#define ezdev_sdk_name_len 64 ///< 设备SDK 一些命名的长度
typedef void *ezdev_sdk_time;

typedef struct
{
    int len;
    char* data;
} MQTTLenString;

typedef struct
{
    char* cstring;
    MQTTLenString lenstring;
} MQTTString;

typedef struct MQTTNetwork
{
    ezdev_sdk_net_work my_socket;
    int(*mqttread) (struct MQTTNetwork*, unsigned char*, int, int);
    int(*mqttwrite) (struct MQTTNetwork*, unsigned char*, int, int);
    char binding_nic[ezdev_sdk_name_len];
    void*client;
} Network;

enum QoS { QOS0, QOS1, QOS2 };

typedef struct MQTTMessage
{
    enum QoS qos;
    unsigned char retained;
    unsigned char dup;
    unsigned short id;
    void *payload;
    size_t payloadlen;
} MQTTMessage;

typedef struct MessageData
{
    MQTTMessage* message;
    MQTTString* topicName;
} MessageData;

typedef struct Timer
{
    ezdev_sdk_time end_time;
} Timer;

typedef void(*messageHandler)(MessageData*,void*);

typedef struct MQTTClient
{
    unsigned int next_packetid,
        command_timeout_ms;
    size_t buf_size,
        readbuf_size;
    unsigned char *buf,
        *readbuf;
    unsigned int keepAliveInterval;
    char ping_outstanding;
    int isconnected;

    struct MessageHandlers
    {
        char topicFilter[512];
        void(*fp) (MessageData*, void*);
    } messageHandlers[MAX_MESSAGE_HANDLERS];      /* Message handlers are indexed by subscription topic */

    void(*defaultMessageHandler) (MessageData*);

    Network* ipstack;
    Timer ping_timer;
    Timer connect_timer;
    void * user_data;
    int mqtt_lasterror;
#if defined(MQTT_TASK)
    Mutex mutex;
    Thread thread;
#endif 
} MQTTClient;

#endif

