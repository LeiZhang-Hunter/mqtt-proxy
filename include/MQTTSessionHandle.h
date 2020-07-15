//
// Created by zhanglei on 2020/7/14.
//

#ifndef DEVICE_SERVER_MQTTSESSIONHANDLE_H
#define DEVICE_SERVER_MQTTSESSIONHANDLE_H
namespace DeviceServer{

class MQTTSessionHandle : public muduo::noncopyable{
public:
    //关闭会话触发的事件
    void onConnect(const DeviceServer::Callback::MQTTClientSessionPtr&  session);

    //收到信息触发的事件
    void onMessage(const DeviceServer::Callback::MQTTClientSessionPtr&  session,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp time
    );

    //关闭会话触发的事件
    void onClose(const DeviceServer::Callback::MQTTClientSessionPtr&  session);
};
}
#endif //DEVICE_SERVER_MQTTSESSIONHANDLE_H
