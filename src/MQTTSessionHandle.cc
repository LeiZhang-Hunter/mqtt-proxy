//
// Created by zhanglei on 2020/7/14.
//
#include "autoload.h"
//连接建立
bool DeviceServer::MQTTSessionHandle::OnConnect(const DeviceServer::Callback::MQTTClientSessionPtr&  session)
{
    uint8_t connectAck = 0;
    if(session->getCleanSession() == 0)
        connectAck |= 0x01;
    MQTTProxy::MQTTProxyProtocol protocol;
    //连接的标志
    protocol.MessageType = MQTTProxy::CONNECT;
    //返回码
    protocol.MessageNo = MQTTProxy::RETURN_OK;
    //客户端id的长度
    protocol.ClientIdLength = session->getClientId().length();
    //客户端id
    protocol.ClientId = session->getClientId();
    //查看是否要输入账号密码，如果要输入则需要在载荷中加入账号和密码
    Json::Value proto_builder;
    if(session->getUserNameFlag())
    {
        proto_builder["username"] = session->getUserName();
    }
    if(session->getPasswordFlag())
    {
        proto_builder["password"] = session->getPassword();
    }
    if(proto_builder.size() > 0)
    {
        Json::String encode_string = MQTTContainer.Util.jsonEncode(proto_builder);
        protocol.Payload.insert(
                protocol.Payload.end(),
                encode_string.begin(),
                encode_string.end());
    }
    protocol.MessageLength = protocol.Payload.size();
    //发送消息到设备中心
    MQTTContainer.getProxyClient()->sendProxyData(protocol);
    //response.sendConnectAck(session->getConn(), connectAck, CONNACK_ACCEPTED);
}

//订阅事件
bool DeviceServer::MQTTSessionHandle::OnSubscribe(const DeviceServer::Callback::MQTTClientSessionPtr&  session,
        const DeviceServer::MQTTSubscribe& subscribe)
{
    MQTTProxy::MQTTProxyProtocol protocol;
    //连接的标志
    protocol.MessageType = MQTTProxy::SUBSCRIBE;
    //返回码
    protocol.MessageNo = MQTTProxy::RETURN_OK;
    //客户端id的长度
    protocol.ClientIdLength = session->getClientId().length();
    //客户端id
    protocol.ClientId = session->getClientId();
    //加入订阅的主题
    Json::Value proto_builder;
    proto_builder[SUBSCRIBE_TOPIC] = subscribe.topic;
    proto_builder[SUBSCRIBE_MESSAGE_ID] = subscribe.messageId;
    proto_builder[SUBSCRIBE_QOS_LEVEL] = subscribe.QosLevel;
    if(proto_builder.size() > 0)
    {
        Json::String encode_string = MQTTContainer.Util.jsonEncode(proto_builder);
        protocol.Payload.insert(
                protocol.Payload.end(),
                encode_string.begin(),
                encode_string.end());
    }
    protocol.MessageLength = protocol.Payload.size();
    //发送消息到设备中心
    MQTTContainer.getProxyClient()->sendProxyData(protocol);
}

//取消订阅事件
bool DeviceServer::MQTTSessionHandle::OnUnSubscribe(const DeviceServer::Callback::MQTTClientSessionPtr&  session,
        const DeviceServer::MQTTSubscribe& subscribe)
{
    MQTTProxy::MQTTProxyProtocol protocol;
    //连接的标志
    protocol.MessageType = MQTTProxy::UNSUBSCRIBE;
    //返回码
    protocol.MessageNo = MQTTProxy::RETURN_OK;
    //客户端id的长度
    protocol.ClientIdLength = session->getClientId().length();
    //客户端id
    protocol.ClientId = session->getClientId();
    //加入取消订阅的主题
    Json::Value proto_builder;
    proto_builder[SUBSCRIBE_TOPIC] = subscribe.topic;
    proto_builder[SUBSCRIBE_MESSAGE_ID] = subscribe.messageId;
    proto_builder[SUBSCRIBE_QOS_LEVEL] = subscribe.QosLevel;
    if(proto_builder.size() > 0)
    {
        Json::String encode_string = MQTTContainer.Util.jsonEncode(proto_builder);
        protocol.Payload.insert(
                protocol.Payload.end(),
                encode_string.begin(),
                encode_string.end());
    }
    protocol.MessageLength = protocol.Payload.size();
    //发送消息到设备中心
    MQTTContainer.getProxyClient()->sendProxyData(protocol);
}

//收到相关主题的推送
void DeviceServer::MQTTSessionHandle::OnPublish(const DeviceServer::Callback::MQTTClientSessionPtr&  session,
        const DeviceServer::MQTTSubscribe& subscribe, const std::string& message
)
{
    MQTTProxy::MQTTProxyProtocol protocol;
    //连接的标志
    protocol.MessageType = MQTTProxy::PUBLISH;
    //返回码
    protocol.MessageNo = MQTTProxy::RETURN_OK;
    //客户端id的长度
    protocol.ClientIdLength = session->getClientId().length();
    //客户端id
    protocol.ClientId = session->getClientId();
    Json::Value proto_builder;
    proto_builder["topic"] = subscribe.topic;
    proto_builder["message"] = message;
    proto_builder["message_id"] = subscribe.messageId;
    if(proto_builder.size() > 0)
    {
        Json::String encode_string = MQTTContainer.Util.jsonEncode(proto_builder);
        protocol.Payload.insert(
                protocol.Payload.end(),
                encode_string.begin(),
                encode_string.end());
    }
    protocol.MessageLength = protocol.Payload.size();
    //发送消息到设备中心
    MQTTContainer.getProxyClient()->sendProxyData(protocol);
}

//关闭会话触发的事件
bool DeviceServer::MQTTSessionHandle::OnDisConnect(const DeviceServer::Callback::MQTTClientSessionPtr&  session)
{
    MQTTProxy::MQTTProxyProtocol protocol;
    //连接的标志
    protocol.MessageType = MQTTProxy::DISCONNECT;
    //返回码
    protocol.MessageNo = MQTTProxy::RETURN_OK;
    //客户端id的长度
    protocol.ClientIdLength = session->getClientId().length();
    //客户端id
    protocol.ClientId = session->getClientId();
    //发送消息到设备中心
    MQTTContainer.getProxyClient()->sendProxyData(protocol);
}