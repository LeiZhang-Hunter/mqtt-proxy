//
// Created by zhanglei on 2020/6/23.
//

#include "autoload.h"

bool DeviceSeverLib::MQTT::parse(muduo::net::Buffer *buf)
{
    read_byte = buf->readableBytes();
    if(read_byte < 5)
    {
        return false;
    }

    //
    uint8_t fix_header = buf->peekInt8();

    retain = fix_header & 0x01;

    qos_level = fix_header & 0x06;

    dup_flag = fix_header & 0x08;

    msg_type = (fix_header & 0xF0) >> 4;

    //Remaining Length
    uint32_t multiplier = 1;
    remaining_length = 0;
    read_length = 0;

    char i=0;
    const char* byte = buf->peek();
    do {

        byte++;
        i++;

        if(i > 4)
        {
            return false;
        }

        remaining_length += (*byte & 127) * multiplier;
        multiplier *= 128;
    }while((*byte & 128) != 0);

    if(remaining_length <= 0)
    {
        return  false;
    }

    //count readable bytes
    if(read_byte < remaining_length +i +1)
    {
        return false;
    }

    buf->retrieve(i+1);

    switch (msg_type) {
        case MQTT_CONNECT:
            return parseOnConnect(buf);

        case MQTT_SUBSCRIBE:
            return parseOnSubscribe(buf);

        default:
            return false;
    }
}

bool DeviceSeverLib::MQTT::parseOnConnect(muduo::net::Buffer *buf)
{
//variable header/可变的报头
    protocol_name_len = (buf->peekInt16());
    buf->retrieve(2);
    read_length += 2;

    if(protocol_name_len < 4)
        return  false;

    //protocol name
    std::string protocol_name(buf->peek(), protocol_name_len);
    buf->retrieve(protocol_name_len);
    read_length += protocol_name_len;

    protocol_version = buf->peekInt8();
    buf->retrieve(1);
    read_length += 1;

    connect_flag = buf->peekInt8();
    buf->retrieve(1);
    read_length+=1;

    //connect flag
    will_reserved = connect_flag & 0x01;
    will_clean_session = (connect_flag & 0x02) >> 1;
    will_flag = connect_flag & 0x04 >> 2;
    will_qos = static_cast<uint8_t>((connect_flag & 0x18) >> 3);
    will_retain = ((connect_flag & 0x20) >> 5);
    password_flag = (connect_flag & 0x40) >> 6;
    username_flag = (connect_flag & 0x80) >> 7;

    if(will_qos > 2 || will_qos < 0)
        return  false;

    //Keep Alive timer
    keep_live_time = buf->peekInt16();
    buf->retrieve(2);
    read_length += 2;

    topic_name_len = buf->peekInt16();
    buf->retrieve(2);
    read_length += 2;

    std::string topic_name(buf->peek(), topic_name_len);
    buf->retrieve(topic_name_len);
    read_length+=topic_name_len;

    payload_len = remaining_length - read_length;
    if(payload_len < 0)
    {
        return  false;
    }

    if(payload_len == 0)
    {
        return true;
    }
    return true;
}

bool DeviceSeverLib::MQTT::parseOnSubscribe(muduo::net::Buffer *buf)
{
    message_id = buf->peekInt16();
    buf->retrieve(2);

    payload_len = buf->peekInt16();
    if(payload_len > remaining_length - 2)
    {
        return false;
    }
    buf->retrieve(2);

    std::string payload(buf->peek(), payload_len);
    buf->retrieve(payload_len);
    std::cout<<payload<<std::endl;
    uint8_t temp_qos_level = buf->peekInt8();
    return true;
}