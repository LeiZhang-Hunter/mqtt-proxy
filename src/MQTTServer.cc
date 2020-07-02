//
// Created by zhanglei on 2020/6/23.
//

#include "autoload.h"

using namespace std::placeholders;

DeviceSever::MQTTServer::MQTTServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr)
        : loop_(loop),
          server_(loop, listenAddr, "MQTTServer")
{
    server_.setConnectionCallback(std::bind(&MQTTServer::onConnection, this, _1));
    server_.setMessageCallback(std::bind(&MQTTServer::onMessage, this, _1, _2, _3));
}

void DeviceSever::MQTTServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
//    LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
//              << conn->localAddress().toIpPort() << " is "
//              << (conn->connected() ? "UP" : "DOWN");
    conn->setTcpNoDelay(true);
    conn->setCloseCallback(std::bind(&MQTTServer::onClose, this, _1));
}

void DeviceSever::MQTTServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
        muduo::Timestamp time)
{
    std::shared_ptr<DeviceSeverLib::MQTT> mqtt_package = std::make_shared<DeviceSeverLib::MQTT>();
    bool res = mqtt_package->parse(buf, conn);
    if(!res)
    {
        conn->forceClose();
    }

    switch (mqtt_package->getMsgType())
    {
        case MQTT_CONNECT:
            //对包进行绑定
            MQTTContainer.pool->registerConn(conn, mqtt_package);
            break;
    }
}

void DeviceSever::MQTTServer::start()
{
    server_.start();
}

void DeviceSever::MQTTServer::onClose(const muduo::net::TcpConnectionPtr& conn)
{
    MQTTContainer.pool->deleteConn(conn);
    std::cout<<"end"<<std::endl;
}
