//
// Created by zhanglei on 2020/6/23.
//

#ifndef DEVICE_SERVER_MQTT_H
#define DEVICE_SERVER_MQTT_H

enum{
    MQTT_CONNECT = 1,
    MQTT_CONNECTACK = 0x20,
    MQTT_PUBLISH = 3,
    MQTT_PUBACK = 4,
    MQTT_PUBREC = 5,
    MQTT_PUBREL = 6,
    MQTT_PUBCOMP = 7,
    MQTT_SUBSCRIBE = 8,
    MQTT_SUBACK = 9,
    MQTT_UNSUBSCRIBE = 10,
    MQTT_UNSUBACK = 11,
    MQTT_PINGREQ = 12,
    MQTT_PINGRESP = 13,
    MQTT_DISCONNECT = 14
};

namespace DeviceSeverLib {
class MQTT :public muduo::noncopyable{
    public:
        MQTT() = default;
        bool parse(muduo::net::Buffer *buf);
    private:
        std::string buffer;
    };
}
#endif //DEVICE_SERVER_MQTT_H
