//
// Created by zhanglei on 2020/7/16.
//

#ifndef DEVICE_SERVER_PROXYPROTOCOL_H
#define DEVICE_SERVER_PROXYPROTOCOL_H

/**
 * 这个文件是做MQTT代理协议的 一般用在内网传输，不推荐用加密的方式，当然载荷部分可以加密的
 *
 * 具体结构:
 *
 * -------------------------------
 * |
 * |          类型
 * |------------------------------
 * |
 * |          设备id
 * |-------------------------------
 * |
 * |          错误码
 * |
 * |-------------------------------
 * |
 * |          剩余长度
 * |-------------------------------
 * |
 * |          载荷(使用json)
 * |-------------------------------
 * |
 * |          CRC校验
 * |-------------------------------
 */
namespace MQTTProxy
{

//MQTT代理的协议
typedef struct _MQTTProxyProtocol
{
    //消息类型
    uint8_t MessageType;
    //消息结果 用来存储错误码
    uint8_t MessageNo;
    //客户端id的长度
    uint32_t ClientIdLength;
    //设备id
    std::string ClientId;
    //消息长度
    uint32_t  MessageLength;
    //载荷
    std::vector<uint8_t> Payload;
    //CRC校验用来核对
    uint16_t MessageCrc;
}MQTTProxyProtocol;

//协议的类型
enum{
    CONNECT,
    CONNECT_MESSAGE,
    SUBSCRIBE,
    SUBSCRIBE_MESSAGE,
    UNSUBSCRIBE,
    UNSUBSCRIBE_MESSAGE,
    PUBLISH,
    PUBLISH_MESSAGE,
    DISCONNECT,
    DISCONNECT_MESSAGE
};

//协议的类型
enum{
    RETURN_OK = 0,
    RETURN_FAILED = 1
};
}

#endif //DEVICE_SERVER_PROXYPROTOCOL_H
