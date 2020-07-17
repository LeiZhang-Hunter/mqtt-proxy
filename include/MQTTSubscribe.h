//
// Created by zhanglei on 2020/7/16.
//

#ifndef DEVICE_SERVER_MQTTSUBSCRIBE_H
#define DEVICE_SERVER_MQTTSUBSCRIBE_H

#include "autoload.h"

namespace DeviceServer
{

class MQTTClientSession;

class MQTTSubscribe
{

public:
    uint16_t messageId = 0;
    std::string topic;
    uint8_t QosLevel;
    DeviceServer::Callback::MQTTClientSessionPtr sessionPtr;
};

//订阅树上的叶子节点
class MQTTSubscribeTreeNode
{

public:
    std::string topic;
    std::map<std::string, DeviceServer::MQTTClientSession> sessionMap;
    std::map<std::string, DeviceServer::MQTTSubscribe> sonTopic;
};
}
#endif //DEVICE_SERVER_MQTTSUBSCRIBE_H
