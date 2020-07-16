//
// Created by zhanglei on 2020/6/23.
//

#ifndef DEVICE_SERVER_MQTTPROTOCOL_H
#define DEVICE_SERVER_MQTTPROTOCOL_H

namespace DeviceServerLib {
/**
 * 这个类主要是负责mqtt协议内容解析的
 */
class MQTTProtocol {
public:
    MQTTProtocol() = default;

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

    bool bufferRollback(muduo::net::Buffer *buf)
    {
        std::cout<<"back:"<<transaction_read_byte<<std::endl;
        buf->retrieve(-transaction_read_byte);
        return true;
    }

    std::string& getClientId()
    {
        return client_id;
    }

    std::string& getProtocolName()
    {
        return protocol_name;
    }

    uint8_t getRetain()
    {
        return retain;
    }

    uint16_t getQosLevel()
    {
        return qos_level;
    };

    uint8_t getDupFlag()
    {
        return dup_flag;
    }

    //设置连接的回调
    bool setConnectCallback(const DeviceServer::Callback::MQTTProtocolOnConnect& cb)
    {
        OnConnect = cb;
        return true;
    }

    //设置断开连接的回调
    bool setDisConnectCallback(const DeviceServer::Callback::MQTTProtocolOnDisConnect& cb)
    {
        OnDisConnect = cb;
        return true;
    }

    //设置订阅的回调
    bool setSubscribeCallback(const DeviceServer::Callback::MQTTProtocolOnSubscribe& cb)
    {
        OnSubscribe = cb;
        return true;
    }

    //设置取消事件的回调
    bool setUnSubscribeCallback(const DeviceServer::Callback::MQTTProtocolOnUnSubscribe& cb)
    {
        OnUnSubscribe = cb;
        return true;
    }

    //设置推送事件的回调
    bool setPublishCallback(const DeviceServer::Callback::MQTTProtocolOnPublish& cb)
    {
        OnPublish = cb;
        return true;
    }


    ~MQTTProtocol() = default;

    //目标要读取字节数
    size_t dest_read_byte = 0;
    //当前已经读取的字节数
    size_t now_read_byte = 0;
    //剩余的字节数
    size_t last_read_byte = 0;
    //事务读取的字节数，如果出现解析失败方便回滚到正要的位置，让制读取失败之后下次拼接包的时候出现的问题
    size_t transaction_read_byte = 0;

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

    std::string protocol_name;
    std::string client_id;

    //这里用来存储tcp的连接用来发送反馈的
    bool parseOnConnect(muduo::net::Buffer *buf);
    bool parseOnSubscribe(muduo::net::Buffer *buf);
    bool parseOnPublish(muduo::net::Buffer *buf);
    uint16_t parseMessageId(muduo::net::Buffer *buf);

    //会话建立的回调
    DeviceServer::Callback::MQTTProtocolOnConnect OnConnect;
    //会话断开连接的回调
    DeviceServer::Callback::MQTTProtocolOnDisConnect OnDisConnect;
    //订阅的回调
    DeviceServer::Callback::MQTTProtocolOnSubscribe OnSubscribe;
    //取消订阅的回调
    DeviceServer::Callback::MQTTProtocolOnUnSubscribe OnUnSubscribe;
    //推送的时候的回调
    DeviceServer::Callback::MQTTProtocolOnPublish OnPublish;
};
}
#endif //DEVICE_SERVER_MQTTPROTOCOL_H
