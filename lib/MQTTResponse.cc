//
// Created by zhanglei on 2020/6/27.
//
#include "autoload.h"

bool DeviceSeverLib::MQTTResponse::sendSubscribeAck(const muduo::net::TcpConnectionPtr &conn,
        uint16_t message_id, const std::string& topic_name, uint8_t subscribe_qos_level)
{
    std::vector<uint8_t> buffer;
    Util util;
    uint32_t remainingLength = topic_name.length() + 2 + 2;

    //拼接固定报头
    buffer.push_back(MQTT_SUBACK);
    //剩余长度计算
    std::vector<__uint8_t > encodeRemainingLength = util.encodeRemainingLength(remainingLength);
    buffer.insert(buffer.end(), encodeRemainingLength.begin(), encodeRemainingLength.end());

    //可变报头
    buffer.push_back(MSB(message_id));
    buffer.push_back(LSB(message_id));
    //载荷循环放进去
    char* payload_str = (char*)topic_name.c_str();
    size_t payload_length = topic_name.length();
    size_t i;
    for(i = 0;i < payload_length; i++)
    {
        buffer.push_back(*payload_str);
        payload_str++;
    }
    //服务级别
    buffer.push_back(subscribe_qos_level);

    //响应
    conn->send(buffer.data(), buffer.size());
    return true;
}
