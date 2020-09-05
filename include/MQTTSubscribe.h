//
// Created by zhanglei on 2020/7/16.
//

#ifndef DEVICE_SERVER_MQTTSUBSCRIBE_H
#define DEVICE_SERVER_MQTTSUBSCRIBE_H

#include "autoload.h"
#include "MQTTSessionCallback.h"

namespace MQTTProxy {

    class MQTTClientSession;

    class MQTTSubscribe {

    public:
        uint16_t messageId = 0;
        std::string topic;
        uint8_t QosLevel;
        MQTTProxy::Callback::MQTTClientSessionPtr sessionPtr;
    };
}
#endif //DEVICE_SERVER_MQTTSUBSCRIBE_H
