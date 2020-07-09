//
// Created by zhanglei on 2020/6/27.
//
#include "autoload.h"

bool DeviceSeverLib::MQTTResponse::sendSubscribeAck(const muduo::net::TcpConnectionPtr &conn,
        uint16_t message_id, uint8_t subscribe_qos_level)
{
    std::vector<uint8_t> buffer;
    //两个字节的固定包头 + 上一个字节的载荷长度
    uint32_t remainingLength = UINT8_LEN + UINT16_LEN;

    //拼接固定报头
    buffer.push_back(MQTT_SUBACK);
    //剩余长度计算
    std::vector<__uint8_t > encodeRemainingLength = MQTTContainer.util.encodeRemainingLength(remainingLength);
    buffer.insert(buffer.end(), encodeRemainingLength.begin(), encodeRemainingLength.end());

    //可变报头
    buffer.push_back(MSB(message_id));
    buffer.push_back(LSB(message_id));
    //载荷循环放进去
    //服务级别
    buffer.push_back(subscribe_qos_level);

    //响应
    conn->send(buffer.data(), buffer.size());
    return true;
}

//发送publish的确认消息
bool DeviceSeverLib::MQTTResponse::sendPublishAck(const muduo::net::TcpConnectionPtr &conn, uint16_t message_id)
{
    Util util;
    std::vector<uint8_t> publish_ack_buffer;
    publish_ack_buffer.push_back(MQTT_PUBACK);
    std::vector<uint8_t> remain_length = MQTTContainer.util.encodeRemainingLength(UINT16_LEN);
    publish_ack_buffer.insert(publish_ack_buffer.end(), remain_length.begin(), remain_length.end());
    publish_ack_buffer.push_back(MSB(message_id));
    publish_ack_buffer.push_back(LSB(message_id));
    conn->send(publish_ack_buffer.data(), publish_ack_buffer.size());
    return true;
}

//发送publishrec的消息
bool DeviceSeverLib::MQTTResponse::sendPublishRec(const muduo::net::TcpConnectionPtr &conn, uint16_t message_id)
{
    Util util;
    std::vector<uint8_t> publish_ack_buffer;
    publish_ack_buffer.push_back(MQTT_PUBREC);
    std::vector<uint8_t> remain_length = MQTTContainer.util.encodeRemainingLength(UINT16_LEN);
    publish_ack_buffer.insert(publish_ack_buffer.end(), remain_length.begin(), remain_length.end());
    publish_ack_buffer.push_back(MSB(message_id));
    publish_ack_buffer.push_back(LSB(message_id));
    conn->send(publish_ack_buffer.data(), publish_ack_buffer.size());
    return true;
}

//发送publishrel的消息
bool DeviceSeverLib::MQTTResponse::sendPublishRel(const muduo::net::TcpConnectionPtr &conn, uint16_t message_id)
{
    Util util;
    std::vector<uint8_t> publish_ack_buffer;
    publish_ack_buffer.push_back(MQTT_PUBREL);
    std::vector<uint8_t> remain_length = MQTTContainer.util.encodeRemainingLength(UINT16_LEN);
    publish_ack_buffer.insert(publish_ack_buffer.end(), remain_length.begin(), remain_length.end());
    publish_ack_buffer.push_back(MSB(message_id));
    publish_ack_buffer.push_back(LSB(message_id));
    conn->send(publish_ack_buffer.data(), publish_ack_buffer.size());
    return true;
}

//发送publishrel的消息
bool DeviceSeverLib::MQTTResponse::sendPublishComp(const muduo::net::TcpConnectionPtr &conn, uint16_t message_id)
{
    Util util;
    std::vector<uint8_t> publish_ack_buffer;
    publish_ack_buffer.push_back(MQTT_PUBCOMP);
    std::vector<uint8_t> remain_length = MQTTContainer.util.encodeRemainingLength(UINT16_LEN);
    publish_ack_buffer.insert(publish_ack_buffer.end(), remain_length.begin(), remain_length.end());
    publish_ack_buffer.push_back(MSB(message_id));
    publish_ack_buffer.push_back(LSB(message_id));
    conn->send(publish_ack_buffer.data(), publish_ack_buffer.size());
    return true;
}

//解析消息id
bool DeviceSeverLib::MQTTResponse::sendPingResp(const muduo::net::TcpConnectionPtr &conn)
{
    uint8_t message[2] = {MQTT_PINGRESP, 0};
    conn->send(message, 2);
    return true;
}