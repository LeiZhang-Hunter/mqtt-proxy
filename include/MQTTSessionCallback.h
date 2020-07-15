//
// Created by zhanglei on 2020/7/14.
//

#ifndef DEVICE_SERVER_SESSIONCALLBACK_H
#define DEVICE_SERVER_SESSIONCALLBACK_H

namespace DeviceServer{

class MQTTClientSession;

namespace Callback{
    typedef std::shared_ptr<DeviceServer::MQTTClientSession> MQTTClientSessionPtr;
    //会话建立连接的回调事件
    typedef std::function<void (const DeviceServer::Callback::MQTTClientSessionPtr&)> SessionConnectCallback;
    //收到会话消息的回调事件
    typedef std::function<void (const DeviceServer::Callback::MQTTClientSessionPtr&, muduo::net::Buffer*, muduo::Timestamp)> SessionMessageCallback;
    //会话关闭的回调事件
    typedef std::function<void (const DeviceServer::Callback::MQTTClientSessionPtr&)> SessionCloseCallback;
}
}
#endif //DEVICE_SERVER_SESSIONCALLBACK_H
