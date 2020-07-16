//
// Created by zhanglei on 2020/7/16.
//

#ifndef DEVICE_SERVER_MQTTSUBSCRIBE_H
#define DEVICE_SERVER_MQTTSUBSCRIBE_H

namespace DeviceServer
{
class MQTTClientSession;

class MQTTSubscribe
{

public:
    uint16_t messageId = 0;
    std::string topic;
    std::string message;
    uint8_t QosLevel;
    std::map<std::string, DeviceServer::MQTTClientSession> sessionMap;
    std::map<std::string, DeviceServer::MQTTSubscribe> sonTopic;
};
}
#endif //DEVICE_SERVER_MQTTSUBSCRIBE_H
