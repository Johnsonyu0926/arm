#include "Clients.h"
#include <string.h>
#include <stdio.h>

/**
 * 比较客户端ID
 * @param a 客户端结构体指针
 * @param b 客户端ID字符串
 * @return 如果相等返回1，否则返回0
 */
static inline int clientIDCompare(const void* a, const void* b)
{
    const Clients* client = (const Clients*)a;
    return strcmp(client->clientID, (const char*)b) == 0;
}

/**
 * 比较客户端Socket
 * @param a 客户端结构体指针
 * @param b Socket值指针
 * @return 如果相等返回1，否则返回0
 */
static inline int clientSocketCompare(const void* a, const void* b)
{
    const Clients* client = (const Clients*)a;
    return client->net.socket == *(const SOCKET*)b;
}
