//
// Created by zhanglei on 2020/7/16.
//

#include "autoload.h"

void MQTTProxy::MQTTProxyClient::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
        muduo::Timestamp receiveTime)
{
    buf->retrieveAll();
    std::cout << "receive:" <<syscall(SYS_gettid) << std::endl;
}

void MQTTProxy::MQTTProxyClient::onConnection(const muduo::net::TcpConnectionPtr &conn)
{

    //conn->setCloseCallback(std::bind(&MQTTProxyClient::onClose, this, _1));
}

void MQTTProxy::MQTTProxyClient::onClose(const muduo::net::TcpConnectionPtr &conn)
{

    if (!conn->connected()) {
        std::cout<<"close"<<std::endl;

    }
}