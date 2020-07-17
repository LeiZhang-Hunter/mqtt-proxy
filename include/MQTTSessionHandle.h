//
// Created by zhanglei on 2020/7/14.
//

#ifndef DEVICE_SERVER_MQTTSESSIONHANDLE_H
#define DEVICE_SERVER_MQTTSESSIONHANDLE_H
namespace DeviceServer{

/**
 * 我想了一下其实我们的客户端只需要处理的是 CONNECT DISCONNECT PUBLISH SUBSCRIBE UNSUBSCRIBE这些mqtt事件,无需关注底层的
 * 其他事件
 */
class MQTTSessionHandle : public muduo::noncopyable{
public:
    //开启会话触发的事件
    bool OnConnect(const DeviceServer::Callback::MQTTClientSessionPtr&  session);

    //订阅事件
    bool OnSubscribe(const DeviceServer::Callback::MQTTClientSessionPtr&  session, const DeviceServer::MQTTSubscribe& subscribe);

    //取消订阅事件
    bool OnUnSubscribe(const DeviceServer::Callback::MQTTClientSessionPtr&  session, const DeviceServer::MQTTSubscribe& subscribe);

    //收到相关主题的推送
    void OnPublish(const DeviceServer::Callback::MQTTClientSessionPtr&  session,
                   const DeviceServer::MQTTSubscribe& subscribe,
                   const std::string& message);

    //关闭会话触发的事件
    bool OnDisConnect(const DeviceServer::Callback::MQTTClientSessionPtr&  session);

private:
    //响应处理器用来处理mqtt的响应,现阶段是用来测试我认为这个响应器应该是放在代理节点的
    DeviceServerLib::MQTTResponse response;
};
}
#endif //DEVICE_SERVER_MQTTSESSIONHANDLE_H
