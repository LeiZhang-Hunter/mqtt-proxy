//
// Created by zhanglei on 2020/7/12.
//
#include "autoload.h"

using namespace std::placeholders;

bool DeviceServer::MQTTClientSession::startSession()
{
    std::weak_ptr<muduo::net::TcpConnection> weak_conn;
    if(weak_conn.expired()) {
        //设置接收消息的事件
        Conn->setMessageCallback(std::bind(&MQTTClientSession::SessionOnMessage, shared_from_this(), _1, _2, _3));
        //设置关闭的回调事件
        Conn->setCloseCallback(std::bind(&MQTTClientSession::SessionOnClose, shared_from_this(), _1));
        //标记为上线
        IsOnline = Online;
        return true;
    }else{
        LOG_WARN << getClientId()<< "Conn has been destroy";
        return  false;
    }
}

//收到消息的时候触发
void DeviceServer::MQTTClientSession::SessionOnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer* buf,
        muduo::Timestamp)
{
    auto protocol = muduo::Singleton<DeviceServerLib::MQTTProtocol>::instance();
    if(protocol.parse(buf, conn))
    {
    }
}

//关闭连接的时候触发
void DeviceServer::MQTTClientSession::SessionOnClose(const muduo::net::TcpConnectionPtr &conn)
{

}