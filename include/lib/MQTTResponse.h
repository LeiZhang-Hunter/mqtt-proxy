//
// Created by zhanglei on 2020/6/27.
//

#ifndef DEVICE_SERVER_MQTTRESPONSE_H
#define DEVICE_SERVER_MQTTRESPONSE_H

namespace DeviceServerLib {
class MQTTResponse : public muduo::noncopyable
{
public:
    MQTTResponse() = default;
    bool sendConnectAck(const muduo::net::TcpConnectionPtr& conn, uint8_t ack, uint8_t result)
    {
        uint8_t message[4] = {MQTT_CONNECTACK, 2, ack, result};
        if(!conn)
            return false;
        conn->send(message, sizeof(message));
        return true;
    }
    bool sendSubscribeAck(const muduo::net::TcpConnectionPtr &conn, uint16_t message_id, uint8_t subscribe_qos_level);
    bool sendPublishAck(const muduo::net::TcpConnectionPtr &conn, uint16_t message_id);
    bool sendPublishRec(const muduo::net::TcpConnectionPtr &conn, uint16_t message_id);
    bool sendPublishRel(const muduo::net::TcpConnectionPtr &conn, uint16_t message_id);
    bool sendPublishComp(const muduo::net::TcpConnectionPtr &conn, uint16_t message_id);
    bool sendPingResp(const muduo::net::TcpConnectionPtr &conn);
    ~MQTTResponse() = default;
};
}

#endif //DEVICE_SERVER_MQTTRESPONSE_H
