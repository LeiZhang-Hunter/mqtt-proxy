//
// Created by zhanglei on 2020/7/16.
//
#include "autoload.h"
#include "MQTTContainerGlobal.h"
#include "ProxyProtocol.h"
#include "MQTTResponse.h"
#include "MQTTClientSession.h"
#include "MQTTSubscribe.h"

MQTTProxy::ProxyProtocolHandle::ProxyProtocolHandle() {
    response = std::make_shared<MQTTProxyLib::MQTTResponse>();
}

bool MQTTProxy::ProxyProtocolHandle::parse(muduo::net::Buffer *buffer) {
    size_t read_bytes = buffer->readableBytes();

    if (read_bytes < 4) {
        return false;
    }

    size_t left_len = read_bytes;
    size_t read_len = 0;
    const char *byte;
    const char *start;
    std::vector<uint8_t> crc_string;
    uint8_t qos_level;
    std::string client_id;
    std::string payload;
    uint32_t payload_len;

    while (left_len > 0) {
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
        uint8_t mqtt_type = buffer->peekInt8();
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
        uint32_t client_id_len = buffer->peekInt32();
        if (client_id_len) {
            protocol->ClientIdLength = client_id_len;

            //解析出client_id
            if (client_id_len > left_len) {
                return false;
            }

            uint32_t netClientLength = htonl(client_id_len);

            for (uint8_t l = 0; l < 4; l++) {
                crc_string.push_back(*((uint8_t *) &netClientLength + l));
                buffer->retrieve(UINT8_LEN);
                left_len -= UINT8_LEN;
                read_len += UINT8_LEN;
            }

            client_id.assign(buffer->peek(), client_id_len);
            crc_string.insert(crc_string.end(), client_id.begin(), client_id.end());
            protocol->ClientId = client_id;
            buffer->retrieve(client_id_len);
            left_len -= client_id_len;
            read_len += client_id_len;
            std::cout << protocol->ClientId << std::endl;
            std::cout << protocol->ClientId.length() << std::endl;
        } else {
            buffer->retrieve(UINT32_LEN);
            //大端必须填充4个0
            crc_string.push_back(0);
            crc_string.push_back(0);
            crc_string.push_back(0);
            crc_string.push_back(0);
            left_len -= UINT32_LEN;
            read_len += UINT32_LEN;
        }


        //尝试解析载荷长度
        const char *payload_len_byte = buffer->peek();
        std::pair<uint32_t, uint32_t> payload_len_result = MQTTContainer.Util.decodeRemainingLength(payload_len_byte);
        if (payload_len_result.first) {
            payload_len = payload_len_result.first;

            //将载荷长度放入到crc校验中去
            for(int i = 0; i < payload_len_result.second; i++)
            {
                crc_string.push_back(payload_len_byte[i]);
            }
            buffer->retrieve(payload_len_result.second);
            left_len -= payload_len_result.second;
            read_len += payload_len_result.second;
            if (payload_len > left_len) {
                return false;
            }

            payload.assign(buffer->peek(), payload_len);
            std::cout << payload << std::endl;
            protocol->Payload.insert(protocol->Payload.end(), payload.begin(), payload.end());
            buffer->retrieve(payload_len);
            left_len -= payload_len;
            crc_string.insert(crc_string.end(), payload.begin(), payload.end());


        } else {
            buffer->retrieve(UINT8_LEN);
            crc_string.push_back(0);
            left_len -= UINT8_LEN;
            read_len += UINT8_LEN;
        }

        //CRC校验包的正确性
        read_len += UINT16_LEN;
        left_len -= UINT16_LEN;
        uint16_t crc_check = buffer->peekInt16();
        uint16_t crc = MQTTContainer.Util.checkCRC16(crc_string.data(), crc_string.size());
        buffer->retrieve(UINT16_LEN);
        if (crc != crc_check) {
            LOG_ERROR << "crc parsing error;";
            return false;
        }

        if (mqtt_type == PROXY_PINGREQ) {
            //心跳包直接过滤掉
            return true;
        }

        std::shared_ptr<MQTTProxy::MQTTClientSession> session = MQTTContainer.SessionPool->findSession(client_id);

        switch (mqtt_type) {
            case CONNECT_MESSAGE:
                if (OnConnect(protocol)) {
                    response->sendConnectAck(session->getConn(), CONNACK_ACCEPTED, 0);
                } else {
                    response->sendConnectAck(session->getConn(), CONNACK_REFUSED_IDENTIFIER_REJECTED, 0);
                }
                break;

            case SUBSCRIBE_MESSAGE:
                if (protocol->Payload.empty()) {
                    break;
                }
                if (OnSubscribe(protocol)) {
                    //解析payload
                    Json::Value subscribe_data;
                    if (MQTTContainer.Util.jsonDecode(payload, &subscribe_data)) {
                        if (!subscribe_data[SUBSCRIBE_TOPIC]) {
                            break;
                        }

                        if (subscribe_data[SUBSCRIBE_MESSAGE_ID] && subscribe_data[SUBSCRIBE_QOS_LEVEL]) {
                            MQTTSubscribe subscribe;
                            subscribe.messageId = subscribe_data[SUBSCRIBE_MESSAGE_ID].asUInt();
                            subscribe.topic = subscribe_data[SUBSCRIBE_TOPIC].asString();
                            subscribe.QosLevel = subscribe_data[SUBSCRIBE_QOS_LEVEL].asUInt();
                            if (MQTTContainer.TopicTree->addSubscribe(subscribe, session)) {
                                response->sendSubscribeAck(session->getConn(),
                                                           subscribe_data[SUBSCRIBE_MESSAGE_ID].asUInt(),
                                                           subscribe_data[SUBSCRIBE_QOS_LEVEL].asUInt());
                            }
                        }
                    }
                }
                break;

            case UNSUBSCRIBE_MESSAGE:
                break;

            case PUBLISH_MESSAGE:
                if (protocol->Payload.empty()) {
                    break;
                }

                if (OnPublish(protocol)) {
                    Json::Value publish_data;

                    if (MQTTContainer.Util.jsonDecode(payload, &publish_data)) {
                        qos_level = publish_data[PUBLISH_QOS_LEVEL].asUInt();
                        //推送消息
                        MQTTProxy::MQTTSubscribe topic;
                        topic.QosLevel = qos_level;
                        topic.messageId = publish_data[PUBLISH_MSG_ID].asUInt();
                        topic.topic = publish_data[SUBSCRIBE_TOPIC].asString();
                        topic.sessionPtr = session;
                        MQTTContainer.TopicTree->publish(topic, publish_data[PUBLISH_MSG].asString());
                        if (qos_level == QUALITY_LEVEL_ONE) {
                            response->sendPublishAck(session->getConn(), publish_data[PUBLISH_MSG_ID].asUInt());
                        } else if (qos_level == QUALITY_LEVEL_TWO) {
                            response->sendPublishRec(session->getConn(), publish_data[PUBLISH_MSG_ID].asUInt());
                        }
                    }
                }
                break;

            case DISCONNECT_MESSAGE:
                break;

            case PUBLISH:
                break;

            case PROXY_CONNECT_MESSAGE:
                MQTTContainer.finished->countDown();
                break;
        }
    }

    return true;
}

void MQTTProxy::ProxyProtocolHandle::setOnConnectMessage(const Callback::ProxyOnConnect &cb) {
    OnConnect = cb;
}

void MQTTProxy::ProxyProtocolHandle::setOnDisConnectMessage(const Callback::MQTTProtocolOnDisConnect &cb) {

}

void MQTTProxy::ProxyProtocolHandle::setOnSubscribeMessage(const Callback::ProxyOnSubscribe &cb) {
    OnSubscribe = cb;
}

void MQTTProxy::ProxyProtocolHandle::setOnUnSubscribeMessage() {

}

void MQTTProxy::ProxyProtocolHandle::setOnPublishMessage(const Callback::ProxyOnPublish &cb) {
    OnPublish = cb;
}

void MQTTProxy::ProxyProtocolHandle::setOnPublish() {

}