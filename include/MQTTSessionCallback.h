//
// Created by zhanglei on 2020/7/14.
//

#ifndef DEVICE_SERVER_SESSIONCALLBACK_H
#define DEVICE_SERVER_SESSIONCALLBACK_H

namespace DeviceServer{

class MQTTClientSession;
class MQTTSubscribe;

namespace Callback{
    //会话的智能指针
    typedef std::shared_ptr<DeviceServer::MQTTClientSession> MQTTClientSessionPtr;
    /**
     * 协议层处理的回调
     */
    typedef std::function<void()> MQTTProtocolOnConnect;
    typedef std::function<void(const DeviceServer::MQTTSubscribe&)> MQTTProtocolOnSubscribe;
    typedef std::function<void(const DeviceServer::MQTTSubscribe&)> MQTTProtocolOnUnSubscribe;
    typedef std::function<void(const DeviceServer::MQTTSubscribe&, const std::string& message)> MQTTProtocolOnPublish;
    typedef std::function<void()> MQTTProtocolOnDisConnect;


    /**
     * 业务层处理的回调
     */
    //会话建立连接的回调事件
    typedef std::function<void (const DeviceServer::Callback::MQTTClientSessionPtr&)> SessionConnectCallback;
    //会话断开连接的回调事件
    typedef std::function<void (const DeviceServer::Callback::MQTTClientSessionPtr&)> SessionDisConnectCallback;
    //订阅事件的回调函数
    typedef std::function<void (const DeviceServer::Callback::MQTTClientSessionPtr&,
            const DeviceServer::MQTTSubscribe&)> SessionSubscribeCallback;
    //取消订阅的回调函数
    typedef std::function<void (const DeviceServer::Callback::MQTTClientSessionPtr&,
            const DeviceServer::MQTTSubscribe&)> SessionUnSubscribeCallback;
    //推送事件的回调函数
    typedef std::function<void (const DeviceServer::Callback::MQTTClientSessionPtr&, const DeviceServer::MQTTSubscribe&,
            const std::string&)> SessionPublishCallback;
}
}
#endif //DEVICE_SERVER_SESSIONCALLBACK_H
