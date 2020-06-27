//
// Created by zhanglei on 2020/6/23.
//

#include "autoload.h"

bool DeviceSeverLib::MQTT::parse(muduo::net::Buffer *buf)
{
    size_t read_byte = buf->readableBytes();
    if(read_byte < 5)
    {
        return false;
    }

    //
    uint8_t fix_header = buf->peekInt8();

    uint8_t retain = fix_header & 0x01;

    uint8_t qos_level = fix_header & 0x06;

    uint8_t dup_flag = fix_header & 0x08;

    uint8_t msg_type = (fix_header & 0xF0) >> 4;

    //Remaining Length
    uint32_t multiplier = 1;
    uint32_t remaining_length = 0;
    uint32_t read_length = 0;

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

    //variable header/可变的报头
    uint16_t len = (buf->peekInt16());
    buf->retrieve(2);
    read_length += 2;

    if(len < 4)
        return  false;

    //protocol name
    std::string protocol_name(buf->peek(), len);
    buf->retrieve(len);
    read_length += len;

    uint8_t protocol_version = buf->peekInt8();
    buf->retrieve(1);
    read_length += 1;

    uint8_t connect_flag = buf->peekInt8();
    buf->retrieve(1);
    read_length+=1;

    //connect flag
    uint8_t will_reserved = connect_flag & 0x01;
    uint8_t will_clean_session = (connect_flag & 0x02) >> 1;
    uint8_t will_flag = connect_flag & 0x04 >> 2;
    uint8_t will_qos = static_cast<uint8_t>((connect_flag & 0x18) >> 3);
    uint8_t will_retain = ((connect_flag & 0x20) >> 5);
    uint8_t password_flag = (connect_flag & 0x40) >> 6;
    uint8_t username_flag = (connect_flag & 0x80) >> 7;

    if(will_qos > 2 || will_qos < 0)
        return  false;

    //Keep Alive timer
    uint16_t keep_live_time = buf->peekInt16();
    buf->retrieve(2);
    read_length += 2;

    uint16_t topic_name_len = buf->peekInt16();
    buf->retrieve(2);
    read_length += 2;

    std::string topic_name(buf->peek(), topic_name_len);
    read_length+=topic_name_len;

    uint32_t payload_len = remaining_length - read_length;
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