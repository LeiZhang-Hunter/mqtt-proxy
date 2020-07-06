//
// Created by zhanglei on 2020/6/23.
//

#ifndef DEVICE_SERVER_MQTT_H
#define DEVICE_SERVER_MQTT_H

namespace DeviceSeverLib {
class MQTT {
public:
    MQTT() = default;

    bool parse(muduo::net::Buffer *buf, const muduo::net::TcpConnectionPtr &conn);

    uint8_t getMsgType()
    {
        return msg_type;
    }

    uint16_t getMsgId()
    {
        return message_id;
    }

    int getErrno()
    {
        return parse_mqtt_errno;
    }

    ~MQTT() = default;

private:
    //读取的字节数
    size_t read_byte = 0;
    //剩余的字节数

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
    uint8_t subscribe_qos_level = 0;
    std::string topic_name;
    std::string payload;
    //存储订阅的map
    std::map<std::string, uint8_t> subscribe_map;
    int parse_mqtt_errno = 0;

    //这里用来存储tcp的连接用来发送反馈的
    bool parseOnConnect(muduo::net::Buffer *buf);
    bool parseOnSubscribe(muduo::net::Buffer *buf);
    bool parseOnPublish(muduo::net::Buffer *buf);

};
}
#endif //DEVICE_SERVER_MQTT_H
