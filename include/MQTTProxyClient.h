//
// Created by zhanglei on 2020/7/16.
//

#ifndef DEVICE_SERVER_MQTTPROXYCLIENT_H
#define DEVICE_SERVER_MQTTPROXYCLIENT_H

using namespace std::placeholders;
namespace MQTTProxy
{

class MQTTProxyClient :public muduo::noncopyable
{
public:
    MQTTProxyClient()
    {

    }

    void onConnection(const muduo::net::TcpConnectionPtr& conn);

    void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp receiveTime);

    void onClose(const muduo::net::TcpConnectionPtr& conn);


    bool setEventLoop(muduo::net::EventLoop* loop)
    {
        return Loop = loop;
    }

    bool setConnectAddr(const muduo::net::InetAddress& listenAddr)
    {
        ConnectAddr = listenAddr;
        return true;
    }

    void connect()
    {
        Client->connect();
    }

    bool start()
    {
        Client = std::make_shared<muduo::net::TcpClient>(Loop, ConnectAddr, "MQTTProxy");
        Client->setConnectionCallback(
                std::bind(&MQTTProxyClient::onConnection, this, _1));
        Client->setMessageCallback(
                std::bind(&MQTTProxyClient::onMessage, this, _1, _2, _3));
        Client->enableRetry();
        connect();
        return true;
    }

    bool sendProxyData(const MQTTProxy::MQTTProxyProtocol& protocol);

    ~MQTTProxyClient()
    {
        std::cout<<"finish"<<std::endl;
    }

    std::shared_ptr<MQTTProxy::ProxyProtocolHandle> handle;
private:
    muduo::net::EventLoop* Loop;
    std::shared_ptr<muduo::net::TcpClient> Client;
    muduo::net::InetAddress ConnectAddr;
    muduo::net::TcpConnectionPtr Conn;

};
}

#endif //DEVICE_SERVER_MQTTPROXYCLIENT_H
