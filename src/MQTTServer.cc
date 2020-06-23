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

void DeviceSever::MQTTServer::onConnection(const muduo::net::TcpConnectionPtr)
{

}

void DeviceSever::MQTTServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
        muduo::Timestamp time)
{

}

void DeviceSever::MQTTServer::start()
{
    server_.start();
}

