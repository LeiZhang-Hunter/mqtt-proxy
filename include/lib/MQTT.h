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
    MQTT_SUBACK = 0x90,
    MQTT_UNSUBSCRIBE = 10,
    MQTT_UNSUBACK = 11,
    MQTT_PINGREQ = 12,
    MQTT_PINGRESP = 13,
    MQTT_DISCONNECT = 14
};

namespace DeviceSeverLib {
class MQTT {
public:
    MQTT() = default;
    bool parse(muduo::net::Buffer *buf);

    //读取的字节数
    size_t read_byte = 0;
    //剩余的字节数
    uint32_t last_read_length = 0;

    //fix header
    uint8_t retain = 0;
    uint8_t dup_flag = 0;
    uint8_t msg_type= 0;
    uint8_t qos_level = 0;

    uint8_t protocol_version = 0;
    uint8_t connect_flag = 0;
    uint8_t will_reserved = 0;
    uint8_t will_clean_session = 0;
    uint8_t will_flag = 0;
    uint8_t will_qos = 0;
    uint8_t will_retain = 0;
    uint8_t password_flag = 0;
    uint8_t username_flag = 0;
    uint16_t keep_live_time = 0;
    uint16_t protocol_name_len = 0;
    uint16_t topic_name_len = 0;
    uint16_t message_id = 0;
    uint32_t payload_len = 0;
    uint32_t remaining_length = 0;


private:
    bool parseOnConnect(muduo::net::Buffer *buf);
    bool parseOnSubscribe(muduo::net::Buffer *buf);
    };
}
#endif //DEVICE_SERVER_MQTT_H
