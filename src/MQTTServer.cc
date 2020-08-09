//
// Created by zhanglei on 2020/6/23.
//

#include "autoload.h"

using namespace std::placeholders;

DeviceServer::MQTTServer::MQTTServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr)
        : loop_(loop),
          server_(loop, listenAddr, "MQTTServer")
{
    server_.setConnectionCallback(std::bind(&MQTTServer::onConnection, this, _1));
    server_.setMessageCallback(std::bind(&MQTTServer::onMessage, this, _1, _2, _3));
    server_.setThreadNum(4);
}

void DeviceServer::MQTTServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
//    LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
//              << conn->localAddress().toIpPort() << " is "
//              << (conn->connected() ? "UP" : "DOWN");
    conn->setTcpNoDelay(true);
    conn->setConnectionCallback(std::bind(&MQTTServer::onClose, this, _1));
    //绑定一个mqtt处理器
}

void DeviceServer::MQTTServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
        muduo::Timestamp time)
{
    std::shared_ptr<DeviceServer::MQTTClientSession> session;
    DeviceServerLib::MQTTProtocol protocol = muduo::Singleton<DeviceServerLib::MQTTProtocol>::instance();
    if(protocol.parse(buf, conn))
    {
        if(protocol.getMsgType() == MQTT_CONNECT_TYPE)
        {
            //绑定进入会话，这里会绑定设备ID和连接，所以在下面不需要再重复绑定了
            if(!(session = MQTTContainer.SessionPool->bindSession(protocol.getClientId(), conn)))
            {
                LOG_ERROR << "bind session error (" << conn->peerAddress().toIpPort() <<
                "->" <<  conn->localAddress().toIpPort();
                conn->forceClose();
            }else{
                std::shared_ptr<DeviceServer::MQTTSessionHandle> handle(new DeviceServer::MQTTSessionHandle);
                //绑定会话信息
                session->setRetain(protocol.getRetain());
                session->setDupFlag(protocol.getDupFlag());
                session->setQosLevel(protocol.getQosLevel());
                session->setCleanSession(protocol.will_clean_session);
                session->setWillFlag(protocol.will_flag);
                session->setWillQos(protocol.will_qos);
                session->setWillRetain(protocol.will_retain);
                session->setUserNameFlag(protocol.username_flag);
                if(protocol.username_flag)
                {
                    session->setUserName(protocol.username);
                }
                session->setPasswordFlag(protocol.password_flag);
                if(protocol.password_flag)
                {
                    session->setPassword(protocol.password);
                }
                session->setKeepAliveTime(protocol.keep_live_time);
                session->setProtoName(protocol.getProtocolName());
                //设置mqtt的主要事件
                //触发设备上线事件
                session->setConnectCallback(std::bind(&MQTTSessionHandle::OnConnect, handle, _1));
                session->setSubscribeCallback(std::bind(&MQTTSessionHandle::OnSubscribe, handle, _1, _2));
                session->setUnSubscribeCallback(std::bind(&MQTTSessionHandle::OnUnSubscribe, handle, _1, _2));
                session->setPublishCallback(std::bind(&MQTTSessionHandle::OnPublish, handle, _1, _2, _3));
                session->setDisConnectCallback(std::bind(&MQTTSessionHandle::OnDisConnect, handle, _1));
                //会话启动失败了直接强制关闭吧
                bool res = session->startSession();
                if(!res)
                {
                    LOG_ERROR << "start session error (" << conn->peerAddress().toIpPort() <<
                              "->" <<  conn->localAddress().toIpPort() <<
                              ";client id :" << session->getClientId();
                    conn->forceClose();
                }
            }
        } else {
            LOG_ERROR << "connect msg type (" << conn->peerAddress().toIpPort() <<
                      "->" <<  conn->localAddress().toIpPort() <<
                      ";error type :" << protocol.getMsgType();
            conn->forceClose();
        }
    } else {
        //强制关闭
        LOG_ERROR << "protocol parse error (" << conn->peerAddress().toIpPort() <<
                  "->" <<  conn->localAddress().toIpPort();
        conn->forceClose();
    }
}

void DeviceServer::MQTTServer::onServerStart(muduo::net::EventLoop* loop)
{
    std::shared_ptr<MQTTProxy::MQTTProxyClient> client(new MQTTProxy::MQTTProxyClient());
    client->handle = std::make_shared<MQTTProxy::ProxyProtocolHandle>();

    //设置回调,这里是设备中心做出反应之后会触发这里的逻辑来确认是否要给出回应建立连接
    std::shared_ptr<DeviceServer::MQTTProxyHandle> proxy_server = std::make_shared<DeviceServer::MQTTProxyHandle>();
    client->handle->setOnConnectMessage(std::bind(&MQTTProxyHandle::OnConnectMessage, proxy_server, _1));
    client->handle->setOnSubscribeMessage(std::bind(&MQTTProxyHandle::OnSubscribeMessage, proxy_server, _1));
    client->handle->setOnPublishMessage(std::bind(&MQTTProxyHandle::OnPublishMessage, proxy_server, _1));

    MQTTContainer.ProxyMap[muduo::CurrentThread::tid()] = client;
    muduo::net::InetAddress deviceServerListenAddr(9800);
    MQTTContainer.ProxyMap[muduo::CurrentThread::tid()]->setEventLoop(loop);
    MQTTContainer.ProxyMap[muduo::CurrentThread::tid()]->setConnectAddr(deviceServerListenAddr);
    MQTTContainer.ProxyMap[muduo::CurrentThread::tid()]->start();
}

void DeviceServer::MQTTServer::start()
{
    server_.setThreadInitCallback(std::bind(&MQTTServer::onServerStart, this, _1));
    server_.start();
}

void DeviceServer::MQTTServer::onClose(const muduo::net::TcpConnectionPtr& conn)
{
}
