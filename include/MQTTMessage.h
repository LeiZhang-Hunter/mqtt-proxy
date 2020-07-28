//
// Created by zhanglei on 2020/7/28.
//

#ifndef DEVICE_SERVER_MQTTMESSAGE_H
#define DEVICE_SERVER_MQTTMESSAGE_H

struct _MQTTMessage{

    uint8_t MQTTType = MQTT_PUBLISH_TYPE;
    uint8_t Dup = 0;
    uint8_t QosLevel = 0;
    uint8_t Retain = 1;
    std::string Payload;

}MQTTMessage;

#endif //DEVICE_SERVER_MQTTMESSAGE_H
