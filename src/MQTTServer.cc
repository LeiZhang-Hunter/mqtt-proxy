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
    //绑定一个mqtt处理器
}

void DeviceSever::MQTTServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
        muduo::Timestamp time)
{
    std::shared_ptr<DeviceServer::MQTTClientSession> session;
    DeviceSeverLib::MQTTProtocol protocol = muduo::Singleton<DeviceSeverLib::MQTTProtocol>::instance();
    if(protocol.parse(buf, conn))
    {
        if(protocol.getMsgType() == MQTT_CONNECT_TYPE)
        {
            //绑定进入会话，这里会绑定设备ID和连接，所以在下面不需要再重复绑定了
            if(!(session = MQTTContainer.sessionPool->bindSession(protocol.getClientId(), conn)))
            {
                conn->forceClose();
            }else{
                std::shared_ptr<DeviceServer::MQTTSessionHandle> handle;
                //绑定会话信息
                session->setRetain(protocol.getRetain());
                session->setDupFlag(protocol.getDupFlag());
                session->setQosLevel(protocol.getQosLevel());
                session->setCleanSession(protocol.will_clean_session);
                session->setWillFlag(protocol.will_flag);
                session->setWillQos(protocol.will_qos);
                session->setWillRetain(protocol.will_retain);
                session->setUserNameFlag(protocol.username_flag);
                session->setPasswordFlag(protocol.password_flag);
                session->setKeepAliveTime(protocol.keep_live_time);
                session->setProtoName(protocol.getProtocolName());
                session->setSessionConnectCallBack(std::bind(&DeviceServer::MQTTSessionHandle::onClose, handle, _1));
                session->setSessionMessageCallBack(std::bind(&DeviceServer::MQTTSessionHandle::onMessage, handle,
                        _1, _2, _3));
                session->setSessionCloseCallBack(std::bind(&DeviceServer::MQTTSessionHandle::onClose, handle, _1));
                //开启会话
                session->startSession();
            }
        } else {
            conn->forceClose();
        }
    } else {
        //强制关闭
        conn->forceClose();
    }
}

void DeviceSever::MQTTServer::start()
{
    server_.start();
}

void DeviceSever::MQTTServer::onClose(const muduo::net::TcpConnectionPtr& conn)
{
    std::cout<<"end"<<std::endl;
}
