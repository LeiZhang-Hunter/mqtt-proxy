//
// Created by zhanglei on 2020/6/23.
//

#include "autoload.h"

//循环解析mqtt报文
bool DeviceSeverLib::MQTTProtocol::parse(muduo::net::Buffer *buf, const muduo::net::TcpConnectionPtr &conn)
{
    //这个包可能并不完整我门首先要从不完整这个角度来分析，如果并不完整那么我们选择不解析
    dest_read_byte = buf->readableBytes();
    last_read_byte = dest_read_byte;

    //可读的字节数
    while(last_read_byte > 0){
        //重置事务读取的字节数
        transaction_read_byte = 0;

        if(last_read_byte < UINT16_LEN)
        {
            std::cout<<"error1"<<std::endl;
            bufferRollback(buf);
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
                std::cout<<"error2"<<std::endl;
                bufferRollback(buf);
                return false;
            }
            remaining_length += (*byte & 127) * multiplier;
            multiplier *= 128;
        }while((*byte & 128) != 0);

        if(remaining_length < 0)
        {
            std::cout<<"error3"<<std::endl;
            bufferRollback(buf);
            return  false;
        }

        //说明这是一个不完整的包,不要继续读了
        if(last_read_byte < remaining_length +i + UINT8_LEN)
        {
            std::cout<<"error4"<<std::endl;
            bufferRollback(buf);
            return false;
        }

        //检查剩余的可读的长度是否满足当前mqtt的剩余包长，如果满足则继续读出来如果不满足则等待下一个包
        buf->retrieve(i + UINT8_LEN);
        last_read_byte -= i + UINT8_LEN;
        transaction_read_byte += UINT8_LEN;

        bool res;

        MQTTResponse response;

        switch (msg_type) {
            //如果说消息类型是连接消息，那么不需要考虑粘包问题因为下一步我们是需要发送ack的
            case MQTT_CONNECT_TYPE:
                res = parseOnConnect(buf);
                if(res)
                    response.sendConnectAck(conn, 0, 0);
                else
                    return false;
                break;

            case MQTT_SUBSCRIBE_TYPE:
                if(parseOnSubscribe(buf))
                    response.sendSubscribeAck(conn, message_id, subscribe_qos_level);
                else
                    return false;
                break;

            case MQTT_PUBLISH_TYPE:
                if(parseOnPublish(buf))
                {
                    if (qos_level == QUALITY_LEVEL_ONE) {
                        response.sendPublishAck(conn, message_id);
                    } else if (qos_level == QUALITY_LEVEL_TWO) {
                        response.sendPublishRec(conn, message_id);
                    }
                }else{
                    std::cout<<"error6"<<std::endl;
                    bufferRollback(buf);
                    return false;
                }
                break;

            case MQTT_PUBREL_TYPE:
                //解析出message_id 然后做出回应
                parseMessageId(buf);
                response.sendPublishComp(conn, message_id);
                break;

            case MQTT_DISCONNECT_TYPE:
                break;

            case MQTT_PINGREQ_TYPE:
                response.sendPingResp(conn);
                break;

            default:
                std::cout<<"error5"<<std::endl;
                bufferRollback(buf);
                return false;
        }
    }

    return true;
}

//解析mqtt连接协议
bool DeviceSeverLib::MQTTProtocol::parseOnConnect(muduo::net::Buffer *buf)
{
    //variable header/可变的报头
    protocol_name_len = (buf->peekInt16());
    buf->retrieve(UINT16_LEN);
    last_read_byte -= UINT16_LEN;
    transaction_read_byte += UINT16_LEN;

    if(protocol_name_len < 4)
        return  false;

    if(protocol_name_len > last_read_byte)
    {
        return false;
    }

    //protocol name
    protocol_name.assign(buf->peek(), protocol_name_len);
    buf->retrieve(protocol_name_len);
    last_read_byte -= protocol_name_len;
    transaction_read_byte += protocol_name_len;
    if(last_read_byte < 0)
    {
        return false;
    }

    protocol_version = buf->peekInt8();
    buf->retrieve(UINT8_LEN);
    last_read_byte -= UINT8_LEN;
    transaction_read_byte += UINT8_LEN;

    connect_flag = buf->peekInt8();
    buf->retrieve(UINT8_LEN);
    last_read_byte -= UINT8_LEN;
    transaction_read_byte += UINT8_LEN;

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
    last_read_byte -= UINT16_LEN;
    transaction_read_byte += UINT16_LEN;

    topic_name_len = buf->peekInt16();
    if(topic_name_len > last_read_byte)
    {
        return false;
    }
    buf->retrieve(UINT16_LEN);
    last_read_byte -= UINT16_LEN;
    transaction_read_byte += UINT16_LEN;

    client_id.assign(buf->peek(), topic_name_len);
    buf->retrieve(topic_name_len);
    last_read_byte-=topic_name_len;
    transaction_read_byte += topic_name_len;

    return true;
}

//解析订阅协议
bool DeviceSeverLib::MQTTProtocol::parseOnSubscribe(muduo::net::Buffer *buf)
{
    message_id = buf->peekInt16();
    buf->retrieve(UINT16_LEN);
    last_read_byte -= UINT16_LEN;
    transaction_read_byte += UINT16_LEN;
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
    last_read_byte -= UINT16_LEN;
    transaction_read_byte += UINT16_LEN;

    payload.append(buf->peek(), payload_len);
    buf->retrieve(payload_len);
    last_read_byte -= payload_len;
    transaction_read_byte += payload_len;

    uint8_t temp_qos_level = buf->peekInt8();
    subscribe_qos_level = (temp_qos_level & 0x03);
    buf->retrieve(UINT8_LEN);
    last_read_byte -= UINT8_LEN;
    transaction_read_byte += UINT8_LEN;

    subscribe_map[payload] = subscribe_qos_level;
    return true;
}

//解析发布
bool DeviceSeverLib::MQTTProtocol::parseOnPublish(muduo::net::Buffer *buf)
{
    if(!topic_name.empty())
    {
        topic_name.clear();
    }

    if(!payload.empty())
    {
        payload.clear();
    }

    //主题长度
    uint16_t topic_len = buf->peekInt16();
    if(topic_len + UINT16_LEN > remaining_length)
    {
        return false;
    }
    buf->retrieve(UINT16_LEN);
    last_read_byte -= UINT16_LEN;
    transaction_read_byte += UINT16_LEN;

    topic_name.append(buf->peek(), topic_len);
    buf->retrieve(topic_len);
    last_read_byte -= topic_len;
    transaction_read_byte += topic_len;

    //剩余长度 - topic标题长度所占的高低字节 - 主题长度
    payload_len = remaining_length - UINT16_LEN - topic_len;

    if(qos_level != QUALITY_LEVEL_ZERO)
    {
        message_id = buf->peekInt16();
        buf->retrieve(UINT16_LEN);
        payload_len -= UINT16_LEN;
        last_read_byte -= UINT16_LEN;
        transaction_read_byte += UINT16_LEN;
    }

    //payload解析
    payload.append(buf->peek(), payload_len);
    buf->retrieve(payload_len);
    last_read_byte -= payload_len;
    transaction_read_byte += payload_len;
    return true;
}

//解析出message_id
uint16_t DeviceSeverLib::MQTTProtocol::parseMessageId(muduo::net::Buffer *buf)
{
    message_id = buf->peekInt16();
    buf->retrieve(UINT16_LEN);
    last_read_byte -= UINT16_LEN;
    transaction_read_byte += UINT16_LEN;
}
