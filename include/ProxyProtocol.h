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
 * |          错误码
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
    uint8_t message_type;
    //消息结果 用来存储错误码
    uint8_t message_no;
    //载荷长度
    uint32_t  message_length;
    //载荷
    std::vector<char> payload;
    //CRC校验用来核对
    uint16_t message_crc;
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
}

#endif //DEVICE_SERVER_PROXYPROTOCOL_H
