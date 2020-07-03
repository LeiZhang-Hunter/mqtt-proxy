//
// Created by zhanglei on 2020/6/29.
//

#ifndef DEVICE_SERVER_MQTTTYPE_H
#define DEVICE_SERVER_MQTTTYPE_H
enum{
    MQTT_CONNECT = 1,
    MQTT_CONNECTACK = 0x20,
    MQTT_PUBLISH = 3,
    MQTT_PUBACK = 4,
    MQTT_PUBREC = 5,
    MQTT_PUBREL = 6,
    MQTT_PUBCOMP = 7,
    MQTT_SUBSCRIBE = 8,
    MQTT_SUBACK = 0x90,
    MQTT_UNSUBSCRIBE = 10,
    MQTT_UNSUBACK = 11,
    MQTT_PINGREQ = 12,
    MQTT_PINGRESP = 13,
    MQTT_DISCONNECT = 14
};

#define SUBSCRIBE_LEVEL_ZERO 0
#define SUBSCRIBE_LEVEL_ONE 1
#define SUBSCRIBE_LEVEL_TWO 2
#endif //DEVICE_SERVER_MQTTTYPE_H

//获取字节高位
#define MSB(message_id) static_cast<uint8_t>((message_id & 0xFF00) >> 8)
#define LSB(message_id) static_cast<uint8_t>(message_id & 0x00FF)
