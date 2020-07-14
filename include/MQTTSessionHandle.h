//
// Created by zhanglei on 2020/7/14.
//

#ifndef DEVICE_SERVER_MQTTSESSIONHANDLE_H
#define DEVICE_SERVER_MQTTSESSIONHANDLE_H
namespace DeviceServer{

class MQTTSessionHandle : public muduo::noncopyable{
public:
    //收到信息触发的事件
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp time
    );

    //关闭会话触发的事件
    void onClose(const muduo::net::TcpConnectionPtr& conn);
};
}
#endif //DEVICE_SERVER_MQTTSESSIONHANDLE_H
