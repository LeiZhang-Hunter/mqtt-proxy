//
// Created by zhanglei on 2020/6/23.
//

#include "autoload.h"

//循环解析mqtt报文
bool DeviceSeverLib::MQTT::parse(muduo::net::Buffer *buf, const muduo::net::TcpConnectionPtr &conn)
{
    //这个包可能并不完整我门首先要从不完整这个角度来分析，如果并不完整那么我们选择不解析
    read_byte = buf->readableBytes();

    //可读的字节数
    while(read_byte > 0){
        if(read_byte < UINT16_LEN)
        {
            return false;
        }

        //解析固定的报头
        uint8_t fix_header = buf->peekInt8();
        retain = fix_header & 0x01;
        qos_level = (fix_header & 0x06) >> 1;
        dup_flag = fix_header & 0x08;
        msg_type = (fix_header & 0xF0) >> 4;
        //解析剩余的长度
        uint32_t multiplier = UINT8_LEN;
        remaining_length = 0;
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

        //说明这是一个不完整的包,不要继续读了
        if(read_byte < remaining_length +i + UINT8_LEN)
        {
            return false;
        }

        //检查剩余的可读的长度是否满足当前mqtt的剩余包长，如果满足则继续读出来如果不满足则等待下一个包
        buf->retrieve(i + UINT8_LEN);
        read_byte -= i + UINT8_LEN;

        bool res;

        MQTTResponse response;

        switch (msg_type) {
            //如果说消息类型是连接消息，那么不需要考虑粘包问题因为下一步我们是需要发送ack的
            case MQTT_CONNECT:
                res = parseOnConnect(buf);
                if(res)
                {
                    response.sendConnectAck(conn, 0, 0);
                }
                break;

            case MQTT_SUBSCRIBE:
                if(parseOnSubscribe(buf))
                {
                    response.sendSubscribeAck(conn, message_id, subscribe_qos_level);
                }
                break;

            case MQTT_PUBLISH:
                parseOnPublish(buf);
                break;

            default:
                return false;
        }
    }

    return true;
}

//解析mqtt连接协议
bool DeviceSeverLib::MQTT::parseOnConnect(muduo::net::Buffer *buf)
{
    //variable header/可变的报头
    protocol_name_len = (buf->peekInt16());
    buf->retrieve(UINT16_LEN);
    read_byte -= UINT16_LEN;

    if(protocol_name_len < 4)
        return  false;

    if(protocol_name_len > read_byte)
    {
        return false;
    }

    //protocol name
    std::string protocol_name(buf->peek(), protocol_name_len);
    buf->retrieve(protocol_name_len);
    read_byte -= protocol_name_len;
    if(read_byte < 0)
    {
        return false;
    }

    protocol_version = buf->peekInt8();
    buf->retrieve(UINT8_LEN);
    read_byte -= UINT8_LEN;

    connect_flag = buf->peekInt8();
    buf->retrieve(UINT8_LEN);
    read_byte -= UINT8_LEN;

    //connect flag
    will_reserved = connect_flag & 0x01;
    will_clean_session = (connect_flag & 0x02) >> 1;
    will_flag = connect_flag & 0x04 >> 2;
    will_qos = static_cast<uint8_t>((connect_flag & 0x18) >> 3);
    will_retain = ((connect_flag & 0x20) >> 5);
    password_flag = (connect_flag & 0x40) >> 6;
    username_flag = (connect_flag & 0x80) >> 7;

    if(will_qos > UINT16_LEN || will_qos < 0)
        return  false;

    //Keep Alive timer
    keep_live_time = buf->peekInt16();
    buf->retrieve(UINT16_LEN);
    read_byte -= UINT16_LEN;

    topic_name_len = buf->peekInt16();
    if(topic_name_len > read_byte)
    {
        return false;
    }
    buf->retrieve(UINT16_LEN);
    read_byte -= UINT16_LEN;

    std::string topic_name(buf->peek(), topic_name_len);
    buf->retrieve(topic_name_len);
    read_byte-=topic_name_len;

    //这里要检查一下payload的长度是否合法
    return true;
}

//解析订阅协议
bool DeviceSeverLib::MQTT::parseOnSubscribe(muduo::net::Buffer *buf)
{
    message_id = buf->peekInt16();
    buf->retrieve(UINT16_LEN);
    read_byte -= UINT16_LEN;
    if(!payload.empty())
    {
        payload.clear();
    }

    payload_len = buf->peekInt16();
    if(payload_len > remaining_length - UINT16_LEN)
    {
        return false;
    }
    buf->retrieve(UINT16_LEN);
    read_byte -= UINT16_LEN;

    payload.append(buf->peek(), payload_len);
    buf->retrieve(payload_len);
    read_byte -= payload_len;
    std::cout<<payload<<std::endl;

    uint8_t temp_qos_level = buf->peekInt8();
    subscribe_qos_level = (temp_qos_level & 0x03);
    buf->retrieve(UINT8_LEN);
    read_byte -= UINT8_LEN;
    subscribe_map[payload] = subscribe_qos_level;
    return true;
}

//解析发布
bool DeviceSeverLib::MQTT::parseOnPublish(muduo::net::Buffer *buf)
{
    if(topic_name.empty())
    {
        topic_name.clear();
    }

    if(payload.empty())
    {
        payload.clear();
    }

    //主题
    uint16_t topic_len = buf->peekInt16();
    if(topic_len + UINT16_LEN > remaining_length)
    {
        return false;
    }
    buf->retrieve(UINT16_LEN);
    topic_name.append(buf->peek(), topic_len);
    buf->retrieve(topic_len);

    //剩余长度 - topic标题长度所占的高低字节 - 主题长度
    payload_len = remaining_length - UINT16_LEN - topic_len;

    if(qos_level != QUALITY_LEVEL_ZERO)
    {
        message_id = buf->peekInt16();
        buf->retrieve(UINT16_LEN);
        payload_len -= UINT16_LEN;
    }



    //翻译被日本风
    payload.assign(buf->peek(), payload_len);
    return true;
}