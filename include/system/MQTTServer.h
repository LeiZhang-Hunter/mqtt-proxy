//
// Created by zhanglei on 2020/6/23.
//

#ifndef DEVICE_SERVER_MQTTSERVER_H
#define DEVICE_SERVER_MQTTSERVER_H



namespace MQTTProxy
{

class MQTTServer
{
public:
    MQTTServer(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr);

    void start();

private:

    void onServerStart(muduo::net::EventLoop* loop);

    void onConnection(const muduo::net::TcpConnectionPtr& conn);

    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp time
            );

    void onClose(const muduo::net::TcpConnectionPtr& conn);

    muduo::net::EventLoop* loop_;
    muduo::net::TcpServer server_;
};
}

#endif //DEVICE_SERVER_MQTTSERVER_H
