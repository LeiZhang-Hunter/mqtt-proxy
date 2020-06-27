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
    LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
              << conn->localAddress().toIpPort() << " is "
              << (conn->connected() ? "UP" : "DOWN");
    conn->setTcpNoDelay(true);
}

void DeviceSever::MQTTServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
        muduo::Timestamp time)
{
    DeviceSeverLib::MQTT mqtt;
    mqtt.parse(buf);




//    switch (msg_type) {
//        case MQTT_CONNECT:
//            /**
//             * 如果服务端收到一个CleanSession为0的连接，当前会话标志的值取决于服务端是否已经保存了ClientId对应客户端的会话状态。
//             * 如果服务端已经保存了会话状态，它必须将CONNACK报文中的当前会话标志设置为1。
//             * 如果服务端没有已保存的会话状态，它必须将CONNACK报文中的当前会话设置为0。还需要将CONNACK报文中的返回码设置为0 。
//             */
//            conn->send();
//            break;
//    }
}

void DeviceSever::MQTTServer::start()
{
    server_.start();
}

