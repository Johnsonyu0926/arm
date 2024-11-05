#ifndef DEV_COM_ERROR_H
#define DEV_COM_ERROR_H

#include <cstdint>

namespace DevComError {

constexpr int32_t NONE = 0;  // 没有错误
constexpr int32_t NOINIT = 3;  // 没有初始化
constexpr int32_t CONNECT = 7;  // 连接服务器失败
constexpr int32_t SEND = 8;  // 向服务器发送失败
constexpr int32_t RECV = 9;  // 从服务器接收数据失败
constexpr int32_t RECV_TIMEOUT = 10;  // 从服务器接收数据超时
constexpr int32_t DATA = 11;  // 网络上接收到的数据异常
constexpr int32_t ORDER = 12;  // 调用次序错误
constexpr int32_t PARAMETER = 17;  // 参数错误
constexpr int32_t SUPPORT = 23;  // 不支持该功能
constexpr int32_t ALLOC_RESOURCE = 41;  // 资源分配错误
constexpr int32_t NOENOUGH_BUF = 43;  // 缓冲区太小
constexpr int32_t CREATESOCKET = 44;  // 创建SOCKET出错
constexpr int32_t SETSOCKET = 45;  // 设置SOCKET出错
constexpr int32_t MAXNUM = 46;  // 达到最大数量
constexpr int32_t BINDSOCKET = 72;  // 绑定套接字失败
constexpr int32_t SOCKETCLOSE = 73;  // socket连接中断，此错误通常是由于连接中断或目的地不可达

namespace Authentication {
    constexpr int32_t SUCCESS = 500;  // 认证成功
    constexpr int32_t FAIL = 501;  // 认证失败
    constexpr int32_t CMD_ERROR = 502;  // 非法命令
    constexpr int32_t DATA_INVALID = 503;  // 协议解析失败
    constexpr int32_t DEV_ID_INVALID = 504;  // DevID非法
    constexpr int32_t QUERY_AUTHCODE_FAIL = 505;  // 平台查询验证码失败
    constexpr int32_t QUERY_REDIS_FAIL = 506;  // 平台查询redis失败
    constexpr int32_t SESSION_KEY_INVALID = 507;  // SessionKey无效
    constexpr int32_t ENCRYPT_FAIL = 508;  // 协议加密失败
    constexpr int32_t QUERY_DATA_ERROR = 509;  // 查询Das或Stun信息失败
    constexpr int32_t MASTERKEY_INVALID = 510;  // masterkey非法
    constexpr int32_t SESSION_KEY_INVALID_NOT = 511;  // 暂不用
    constexpr int32_t STUN_INVALID_AUTHENTICATION = 512;  // 查询stun认证失败
    constexpr int32_t DAS_INVALID_AUTHENTICATION = 513;  // 查询das认证失败
    constexpr int32_t CODE_ERROR = 514;  // 交互流程错误
    constexpr int32_t LBS_UPDATE_REDIS_FAIL = 515;  // 平台更新设备信息失败
}

namespace Talk {
    constexpr int32_t PARSING_FAIL = 600;  // 解析消息失败
    constexpr int32_t CHECK_TOKEN = 601;  // 验证token失败
}

namespace EzDevSDK {
    constexpr int32_t INDEX = 100000;
    constexpr int32_t INTERNAL = INDEX + 1;  // 内部错误
    constexpr int32_t INVALID_CALL = INDEX + 2;  // 调用流程有误
    constexpr int32_t PARAMS_INVALID = INDEX + 3;  // 参数非法
    constexpr int32_t BUFFER_TOO_SMALL = INDEX + 4;  // 缓冲区大小不足
    constexpr int32_t DATA_LEN_RANGE = INDEX + 5;  // 数据大小超出范围
    constexpr int32_t MEMORY = INDEX + 6;  // 内存异常
    constexpr int32_t JSON_INVALID = INDEX + 7;  // 非法的json数据
    constexpr int32_t JSON_FORMAT = INDEX + 8;  // json数据有误
    constexpr int32_t EXTEND_NO_FIND = INDEX + 9;  // 信令路由找不到对应的注册领域
    constexpr int32_t EXTEND_FULL = INDEX + 10; // 扩展注册已满
    constexpr int32_t EXTEND_EXISTED = INDEX + 11; // 扩展已经注册
    constexpr int32_t QUEUE_FULL = INDEX + 12; // 消息队列已满
    constexpr int32_t VALUE_LOAD = INDEX + 13; // 获取设备数据失败
    constexpr int32_t VALUE_SAVE = INDEX + 14; // 保存数据至设备失败
    constexpr int32_t MSG_STOP_DISTRIBUTE = INDEX + 15; // 设备正在停止,上层消息停止下发
    constexpr int32_t MSG_DEFLATE_FAIL = INDEX + 16; // 报文解压失败
    constexpr int32_t MSG_INFLATE_FAIL = INDEX + 17; // 报文压缩失败
    constexpr int32_t INS_FULL = INDEX + 18; // 客户端数量达到上限
}

// 其他错误码定义...

} // namespace DevComError

#endif // DEV_COM_ERROR_H
