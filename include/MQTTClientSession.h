//
// Created by zhanglei on 2020/7/12.
//

#ifndef DEVICE_SERVER_MQTTCLIENTSESSION_H
#define DEVICE_SERVER_MQTTCLIENTSESSION_H

namespace DeviceServer
{

class MQTTClientSession : public muduo::noncopyable,std::enable_shared_from_this<MQTTClientSession>
{
public:

    MQTTClientSession()
    {

    }

    bool setConn(const muduo::net::TcpConnectionPtr& connection)
    {
        Conn = connection;
        return true;
    }

    const muduo::net::TcpConnectionPtr& getConn()
    {
        return Conn;
    }

    const std::string& getClientId()
    {
        return ClientId;
    }

    const std::string& setClientId(const std::string& client_id)
    {
        return ClientId = client_id;
    }

    const std::string& setProtoName(const std::string& protocol_name)
    {
        return ProtoName = protocol_name;
    }

    const std::string& getProtoName()
    {
        return ProtoName;
    }

    bool setRetain(uint8_t retain)
    {
        return Retain = retain;
    }

    uint8_t getRetain()
    {
        return Retain;
    }

    bool setQosLevel(uint16_t qos_level)
    {
        return QosLevel = qos_level;
    }

    uint16_t getQosLevel()
    {
        return QosLevel;
    }

    bool setDupFlag(uint8_t dup_flag)
    {
        return DupFlag = dup_flag;
    }

    bool setCleanSession(uint8_t clean_flag)
    {
        return CleanSession = clean_flag;
    }

    uint8_t getCleanSession()
    {
        return CleanSession;
    }

    bool setWillFlag(uint8_t will_flag)
    {
        return WillFlag = will_flag;
    }

    bool getWillFlag()
    {
        return WillFlag;
    }

    bool setWillQos(uint16_t will_qos)
    {
        return WillQos = will_qos;
    }

    uint16_t getWillQos()
    {
        return WillQos;
    }

    bool setWillRetain(uint8_t will_retain)
    {
        return WillRetain = will_retain;
    }

    uint8_t getWillRetain()
    {
        return WillRetain;
    }

    bool setUserNameFlag(uint8_t user_name_flag)
    {
        return UserNameFlag = user_name_flag;
    }

    uint8_t getUserNameFlag()
    {
        return UserNameFlag;
    }

    bool setPasswordFlag(uint8_t password_flag)
    {
        return PasswordFlag = password_flag;
    }

    uint8_t getPasswordFlag()
    {
        return PasswordFlag;
    }

    bool setKeepAliveTime(uint16_t keep_alive_time)
    {
        return KeepAliveTime = keep_alive_time;
    }

    uint16_t getKeepAliveTime()
    {
        return KeepAliveTime;
    }

    uint32_t getProtocolVersion()
    {
        return ProtocolVersion;
    }

    bool setProtocolVersion(uint32_t version)
    {
        return ProtocolVersion = version;
    }

    bool setSessionConnectCallBack(const DeviceServer::Callback::SessionConnectCallback& closure)
    {
        OnConnect = closure;
        return true;
    }

    bool setSessionMessageCallBack(const DeviceServer::Callback::SessionMessageCallback& closure)
    {
        OnMessage = closure;
        return true;
    }

    bool setSessionCloseCallBack(const DeviceServer::Callback::SessionCloseCallback& closure)
    {
        OnClose = closure;
        return true;
    }

    bool startSession();

    void SessionOnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer*, muduo::Timestamp)
    {

    }

    void SessionOnClose(const muduo::net::TcpConnectionPtr& conn)
    {

    }

    ~MQTTClientSession()
    {

    }
private:
    //维持的连接
    muduo::net::TcpConnectionPtr Conn;

    //MQTT的固定报文头
    uint8_t Retain;
    uint16_t QosLevel;
    uint8_t DupFlag;

    /**
     *可变报文
     */
    //是否要清理回话
    uint8_t CleanSession;
    //遗嘱标志
    uint8_t WillFlag;
    //遗嘱的Qos 连接标志位的第三位和第四位
    uint16_t WillQos;
    //遗嘱保留
    uint8_t WillRetain;
    //用户名标志
    uint8_t UserNameFlag;
    //密码标志
    uint8_t PasswordFlag;
    //维持连接的时间
    uint16_t KeepAliveTime;
    //版本版本
    uint32_t ProtocolVersion;
    //协议名称
    std::string ProtoName;
    //客户端id
    std::string ClientId;
    //会话建立的回调
    DeviceServer::Callback::SessionConnectCallback OnConnect;
    //会话有消息的时候的回调
    DeviceServer::Callback::SessionMessageCallback OnMessage;
    //会话关闭时候的回调
    DeviceServer::Callback::SessionCloseCallback OnClose;
};
}
#endif //DEVICE_SERVER_MQTTCLIENTSESSION_H
