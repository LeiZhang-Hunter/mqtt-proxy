//
// Created by zhanglei on 2020/7/28.
//

#ifndef DEVICE_SERVER_MQTTMESSAGE_H
#define DEVICE_SERVER_MQTTMESSAGE_H

typedef struct _MQTTMessage{

    uint8_t MQTTType = MQTT_PUBLISH_TYPE;
    uint8_t Dup = 0;
    uint8_t QosLevel = 0;
    uint8_t Retain = 1;
    uint16_t MessageId;
    std::string topic;
    std::string Payload;

}MQTTMessage;

#endif //DEVICE_SERVER_MQTTMESSAGE_H
