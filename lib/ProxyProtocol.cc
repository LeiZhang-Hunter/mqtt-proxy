//
// Created by zhanglei on 2020/7/16.
//

#include "autoload.h"

bool MQTTProxy::ProxyProtocolHandle::parse(muduo::net::Buffer* buffer)
{
    size_t read_bytes = buffer->readableBytes();

    if(read_bytes < 4)
    {
        return false;
    }

    //解析出协议的类型，这里协议的类型必须
    int8_t protocol_type = buffer->peekInt8();
    if(protocol_type != MQTT_PROXY)
        return false;
    buffer->retrieve(UINT8_LEN);

    //mqtt的消息类型
    uint8_t mqtt_type = buffer->peekInt8();
    buffer->retrieve(UINT8_LEN);

    uint8_t message_no = buffer->peekInt8();
    buffer->retrieve(UINT8_LEN);

    //解码出长度
    const char* client_id_len_byte = buffer->peek();
    uint32_t client_id_len = MQTTContainer.Util.decodeRemainingLength(client_id_len_byte);
    buffer->retrieve(UINT8_LEN);

    //解析出client_id
    std::string client_id;
    if(client_id_len)
    {
        client_id.assign(buffer->peek(), client_id_len);
        buffer->retrieve(client_id_len);
    }

    //尝试解析载荷长度
    const char* payload_len_byte = buffer->peek();
    uint32_t payload_len = MQTTContainer.Util.decodeRemainingLength(payload_len_byte);
    std::string payload;
    payload.assign(buffer->peek(), payload_len);
    buffer->retrieve(UINT8_LEN);
    return true;
}