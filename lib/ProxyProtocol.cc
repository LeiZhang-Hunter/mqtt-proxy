//
// Created by zhanglei on 2020/7/16.
//

#include "autoload.h"

MQTTProxy::ProxyProtocolHandle::ProxyProtocolHandle()
{
    response = std::make_shared<MQTTProxyLib::MQTTResponse>();
}

bool MQTTProxy::ProxyProtocolHandle::parse(muduo::net::Buffer* buffer)
{
    size_t read_bytes = buffer->readableBytes();

    if(read_bytes < 4)
    {
        return false;
    }

    size_t left_len = read_bytes;
    size_t read_len = 0;
    const char *byte;
    const char *start;
    std::vector<uint8_t> crc_string;
    uint8_t qos_level;

    while(left_len > 0)
    {
        std::shared_ptr<MQTTProxy::MQTTProxyProtocol> protocol = std::make_shared<MQTTProxy::MQTTProxyProtocol>();

        crc_string.clear();
        //解析出协议的类型，这里协议的类型必须
        uint8_t protocol_type = buffer->peekInt8();
        left_len -= UINT8_LEN;
        read_len += UINT8_LEN;
        buffer->retrieve(UINT8_LEN);
        crc_string.push_back(protocol_type);
        protocol->ProtocolType = protocol_type;

        //mqtt的消息类型
        uint8_t mqtt_type =  buffer->peekInt8();
        left_len -= UINT8_LEN;
        read_len += UINT8_LEN;
        crc_string.push_back(mqtt_type);
        buffer->retrieve(UINT8_LEN);
        protocol->MessageType = mqtt_type;

        uint8_t message_no = buffer->peekInt8();
        buffer->retrieve(UINT8_LEN);
        left_len -= UINT8_LEN;
        read_len += UINT8_LEN;
        crc_string.push_back(message_no);
        protocol->MessageNo = message_no;

        //解码出长度
        const char* client_id_len_byte = buffer->peek();
        uint32_t client_id_len = MQTTContainer.Util.decodeRemainingLength(client_id_len_byte);
        crc_string.push_back(*client_id_len_byte);
        buffer->retrieve(UINT8_LEN);
        left_len -= UINT8_LEN;
        read_len += UINT8_LEN;
        protocol->ClientIdLength = client_id_len;


        //解析出client_id
        std::string client_id;
        if(client_id_len)
        {
            if(client_id_len > left_len)
            {
                return false;
            }
            client_id.assign(buffer->peek(), client_id_len);
            buffer->retrieve(client_id_len);
            left_len -= client_id_len;
            read_len += client_id_len;
            crc_string.insert(crc_string.end(), client_id.begin(), client_id.end());
            protocol->ClientId = client_id;
        }

        //尝试解析载荷长度
        const char* payload_len_byte = buffer->peek();
        uint32_t payload_len = MQTTContainer.Util.decodeRemainingLength(payload_len_byte);
        crc_string.push_back(*payload_len_byte);
        buffer->retrieve(UINT8_LEN);
        left_len -= UINT8_LEN;
        std::string payload;
        if(payload_len)
        {
            if(payload_len > left_len)
            {
                return false;
            }
            payload.assign(buffer->peek(), payload_len);
            protocol->Payload.insert(protocol->Payload.end(), payload.begin(), payload.end());
            buffer->retrieve(payload_len);
            left_len -= payload_len;
            crc_string.insert(crc_string.end(), payload.begin(), payload.end());
        }

        //CRC校验包的正确性
        read_len += UINT16_LEN;
        left_len -= UINT16_LEN;
        uint16_t crc_check = buffer->peekInt16();
        uint16_t crc = MQTTContainer.Util.checkCRC16(crc_string.data(), crc_string.size());
        buffer->retrieve(UINT16_LEN);
        if(crc != crc_check)
        {
            return false;
        }

        std::shared_ptr<MQTTProxy::MQTTClientSession> session = MQTTContainer.SessionPool->findSession(client_id);
        if(!session)
        {
            continue;
        }

        switch (mqtt_type)
        {
            case CONNECT_MESSAGE:
                if(OnConnect(protocol))
                {
                    response->sendConnectAck(session->getConn(), CONNACK_ACCEPTED, 0);
                }else{
                    response->sendConnectAck(session->getConn(), CONNACK_REFUSED_IDENTIFIER_REJECTED, 0);
                }
                break;

            case SUBSCRIBE_MESSAGE:
                if(protocol->Payload.empty())
                {
                    break;
                }
                if(OnSubscribe(protocol))
                {
                    //解析payload
                    Json::Value subscribe_data;
                    if(MQTTContainer.Util.jsonDecode(payload, &subscribe_data))
                    {
                        if(!subscribe_data[SUBSCRIBE_TOPIC])
                        {
                            break;
                        }

                        if(subscribe_data[SUBSCRIBE_MESSAGE_ID] && subscribe_data[SUBSCRIBE_QOS_LEVEL])
                        {
                            MQTTProxy::MQTTSubscribe subscribe;
                            subscribe.messageId = subscribe_data[SUBSCRIBE_MESSAGE_ID].asUInt();
                            subscribe.topic = subscribe_data[SUBSCRIBE_TOPIC].asString();
                            subscribe.QosLevel = subscribe_data[SUBSCRIBE_QOS_LEVEL].asUInt();
                            if(MQTTContainer.TopicTree->addSubscribe(subscribe, session))
                            {
                                response->sendSubscribeAck(session->getConn(), subscribe_data[SUBSCRIBE_MESSAGE_ID].asUInt(),
                                                           subscribe_data[SUBSCRIBE_QOS_LEVEL].asUInt());
                            }
                        }
                    }
                }
                break;

            case UNSUBSCRIBE_MESSAGE:
                break;

            case PUBLISH_MESSAGE:
                if(protocol->Payload.empty())
                {
                    break;
                }

                if(OnPublish(protocol))
                {
                    Json::Value publish_data;

                    if(MQTTContainer.Util.jsonDecode(payload, &publish_data))
                    {
                        qos_level = publish_data[PUBLISH_QOS_LEVEL].asUInt();
                        //推送消息
                        MQTTProxy::MQTTSubscribe topic;
                        topic.QosLevel = qos_level;
                        topic.messageId = publish_data[PUBLISH_MSG_ID].asUInt();
                        topic.topic = publish_data[SUBSCRIBE_TOPIC].asString();
                        topic.sessionPtr = session;
                        MQTTContainer.TopicTree->publish(topic, publish_data[PUBLISH_MSG].asString());
                        if(qos_level == QUALITY_LEVEL_ONE)
                        {
                            response->sendPublishAck(session->getConn(), publish_data[PUBLISH_MSG_ID].asUInt());
                        }else if(qos_level == QUALITY_LEVEL_TWO)
                        {
                            response->sendPublishRec(session->getConn(), publish_data[PUBLISH_MSG_ID].asUInt());
                        }
                    }
                }
                break;

            case DISCONNECT_MESSAGE:
                break;

            case PUBLISH:
                break;
        }
    }

    return true;
}

void MQTTProxy::ProxyProtocolHandle::setOnConnectMessage(const MQTTProxy::Callback::ProxyOnConnect& cb)
{
    OnConnect = cb;
}

void MQTTProxy::ProxyProtocolHandle::setOnDisConnectMessage(const MQTTProxy::Callback::MQTTProtocolOnDisConnect& cb)
{

}

void MQTTProxy::ProxyProtocolHandle::setOnSubscribeMessage(const MQTTProxy::Callback::ProxyOnSubscribe & cb)
{
    OnSubscribe = cb;
}

void MQTTProxy::ProxyProtocolHandle::setOnUnSubscribeMessage()
{

}

void MQTTProxy::ProxyProtocolHandle::setOnPublishMessage(const MQTTProxy::Callback::ProxyOnPublish & cb)
{
    OnPublish = cb;
}

void MQTTProxy::ProxyProtocolHandle::setOnPublish()
{

}