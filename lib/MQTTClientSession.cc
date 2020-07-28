//
// Created by zhanglei on 2020/7/12.
//
#include "autoload.h"

using namespace std::placeholders;

bool DeviceServer::MQTTClientSession::startSession()
{
    std::weak_ptr<muduo::net::TcpConnection> weak_conn;
    if(weak_conn.expired()) {
        //设置协议处理器的回调处理函数
        OnConnect();
        protocol->setDisConnectCallback(std::bind(&MQTTClientSession::OnDisConnect, shared_from_this()));
        protocol->setSubscribeCallback(std::bind(&MQTTClientSession::OnSubscribe, shared_from_this(), _1));
        protocol->setUnSubscribeCallback(std::bind(&MQTTClientSession::OnUnSubscribe, shared_from_this(), _1));
        protocol->setPublishCallback(std::bind(&MQTTClientSession::OnPublish, shared_from_this(), _1, _2));
        //设置接收消息的事件
        Conn->setMessageCallback(std::bind(&MQTTClientSession::SessionOnMessage, shared_from_this(), _1, _2, _3));
        //设置关闭的回调事件
        Conn->setCloseCallback(std::bind(&MQTTClientSession::SessionOnClose, shared_from_this(), _1));
        //标记为上线
        IsOnline = Online;
        return true;
    }else{
        LOG_WARN << getClientId()<< "Conn("<<getClientId()<<") has been destroy";
        return  false;
    }
}

//收到消息的时候触发
void DeviceServer::MQTTClientSession::SessionOnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer* buf,
        muduo::Timestamp)
{
    protocol->parse(buf, conn);
}

//关闭连接的时候触发
void DeviceServer::MQTTClientSession::SessionOnClose(const muduo::net::TcpConnectionPtr &conn)
{

}

/*=======================为了将协议处理结果传入到业务层，再次封装了一个托底的回调==============================*/
void DeviceServer::MQTTClientSession::OnConnect()
{
    OnConnectCallback(shared_from_this());
}

void DeviceServer::MQTTClientSession::OnDisConnect()
{
    OnConnectCallback(shared_from_this());
}

void DeviceServer::MQTTClientSession::OnSubscribe(const DeviceServer::MQTTSubscribe &subscribe)
{
    //加入订阅树
    OnSubscribeCallback(shared_from_this(), subscribe);
}

void DeviceServer::MQTTClientSession::OnUnSubscribe(const DeviceServer::MQTTSubscribe &subscribe)
{
    OnUnSubscribeCallback(shared_from_this(), subscribe);
}

void DeviceServer::MQTTClientSession::OnPublish(const DeviceServer::MQTTSubscribe &subscribe,
        const std::string &message)
{
    OnPublishCallback(shared_from_this(), subscribe, message);
}
/**/