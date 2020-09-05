//
// Created by zhanglei on 2020/7/14.
//

#ifndef DEVICE_SERVER_SESSIONCALLBACK_H
#define DEVICE_SERVER_SESSIONCALLBACK_H

namespace MQTTProxy {

    class MQTTClientSession;

    class MQTTSubscribe;

    class MQTTProxyProtocol;

    namespace Callback {
        //会话的智能指针
        typedef std::shared_ptr<MQTTProxy::MQTTClientSession> MQTTClientSessionPtr;
        /**
         * 协议层处理的回调
         */
        typedef std::function<void()> MQTTProtocolOnConnect;
        typedef std::function<void(const MQTTProxy::MQTTSubscribe &)> MQTTProtocolOnSubscribe;
        typedef std::function<void(const MQTTProxy::MQTTSubscribe &)> MQTTProtocolOnUnSubscribe;
        typedef std::function<void(const MQTTProxy::MQTTSubscribe &, const std::string &message)> MQTTProtocolOnPublish;
        typedef std::function<void()> MQTTProtocolOnDisConnect;

        typedef std::function<bool const(std::shared_ptr<MQTTProxy::MQTTProxyProtocol> &)> ProxyOnConnect;
        typedef std::function<bool const(std::shared_ptr<MQTTProxy::MQTTProxyProtocol> &)> ProxyOnSubscribe;
        typedef std::function<bool const(std::shared_ptr<MQTTProxy::MQTTProxyProtocol> &)> ProxyOnPublish;

        //代理转发

        /**
         * 业务层处理的回调
         */
        //会话建立连接的回调事件
        typedef std::function<void(const MQTTProxy::Callback::MQTTClientSessionPtr &)> SessionConnectCallback;
        //会话断开连接的回调事件
        typedef std::function<void(const MQTTProxy::Callback::MQTTClientSessionPtr &)> SessionDisConnectCallback;
        //订阅事件的回调函数
        typedef std::function<void(const MQTTProxy::Callback::MQTTClientSessionPtr &,
                                   const MQTTProxy::MQTTSubscribe &)> SessionSubscribeCallback;
        //取消订阅的回调函数
        typedef std::function<void(const MQTTProxy::Callback::MQTTClientSessionPtr &,
                                   const MQTTProxy::MQTTSubscribe &)> SessionUnSubscribeCallback;
        //推送事件的回调函数
        typedef std::function<void(const MQTTProxy::Callback::MQTTClientSessionPtr &, const MQTTProxy::MQTTSubscribe &,
                                   const std::string &)> SessionPublishCallback;
    }
}
#endif //DEVICE_SERVER_SESSIONCALLBACK_H
