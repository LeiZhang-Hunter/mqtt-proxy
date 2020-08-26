//
// Created by zhanglei on 2020/6/23.
//

#include "autoload.h"

using namespace std::placeholders;

MQTTProxy::MQTTServer::MQTTServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr)
        : loop_(loop),
          server_(loop, listenAddr, "MQTTServer")
{
    server_.setConnectionCallback(std::bind(&MQTTServer::onConnection, this, _1));
    server_.setMessageCallback(std::bind(&MQTTServer::onMessage, this, _1, _2, _3));
    std::string thread_num = MQTTContainer.Config.getConfig("thread_num");
    if (thread_num.empty()) {
        std::cerr << "proxy thread_num must not be empty" << std::endl;
        exit(-1);
    }

    thread_number = atoi(thread_num.c_str());
    if (thread_number <= 0) {
        std::cerr << "The number of threads cannot be less than 0" << std::endl;
        exit(-1);
    }
    server_.setThreadNum(thread_number);
    MQTTContainer.finished = std::make_shared<muduo::CountDownLatch>(thread_number);
}

void MQTTProxy::MQTTServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
//    LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
//              << conn->localAddress().toIpPort() << " is "
//              << (conn->connected() ? "UP" : "DOWN");
    conn->setTcpNoDelay(true);
    conn->setConnectionCallback(std::bind(&MQTTServer::onClose, this, _1));
    //绑定一个mqtt处理器
}

void MQTTProxy::MQTTServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
        muduo::Timestamp time)
{
    std::shared_ptr<MQTTProxy::MQTTClientSession> session;
    MQTTProxyLib::MQTTProtocol protocol = muduo::Singleton<MQTTProxyLib::MQTTProtocol>::instance();
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
                std::shared_ptr<MQTTProxy::MQTTSessionHandle> handle(new MQTTProxy::MQTTSessionHandle);
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

void MQTTProxy::MQTTServer::onServerStart(muduo::net::EventLoop* loop)
{
    std::shared_ptr<MQTTProxy::MQTTProxyClient> client(new MQTTProxy::MQTTProxyClient());
    client->handle = std::make_shared<MQTTProxy::ProxyProtocolHandle>();

    //设置回调,这里是设备中心做出反应之后会触发这里的逻辑来确认是否要给出回应建立连接
    std::shared_ptr<MQTTProxy::MQTTProxyHandle> proxy_server = std::make_shared<MQTTProxy::MQTTProxyHandle>();
    client->handle->setOnConnectMessage(std::bind(&MQTTProxyHandle::OnConnectMessage, proxy_server, _1));
    client->handle->setOnSubscribeMessage(std::bind(&MQTTProxyHandle::OnSubscribeMessage, proxy_server, _1));
    client->handle->setOnPublishMessage(std::bind(&MQTTProxyHandle::OnPublishMessage, proxy_server, _1));

    MQTTContainer.ProxyMap[muduo::CurrentThread::tid()] = client;

    std::string notify_ip;
    std::string notify_port;
    notify_ip = MQTTContainer.Config.getConfig("notify_ip");
    if(notify_ip.empty())
    {
        std::cerr << "notify ip must not be empty" << std::endl;
        exit(-1);
    }
    notify_port = MQTTContainer.Config.getConfig("notify_port");
    if(notify_port.empty())
    {
        std::cerr << "notify port must not be empty" << std::endl;
        exit(-1);
    }

    muduo::net::InetAddress MQTTProxyListenAddr(9800);
    MQTTContainer.ProxyMap[muduo::CurrentThread::tid()]->setEventLoop(loop);
    MQTTContainer.ProxyMap[muduo::CurrentThread::tid()]->setConnectAddr(MQTTProxyListenAddr);
    MQTTContainer.ProxyMap[muduo::CurrentThread::tid()]->start();
}

void MQTTProxy::MQTTServer::start()
{
    server_.setThreadInitCallback(std::bind(&MQTTServer::onServerStart, this, _1));
    server_.start();
    //在开始循环之前要检查代理是否已经成功接入设备中心，设备中心接入成功之后才会继续执行
    MQTTContainer.finished->wait();
    std::cout<<"finished"<<std::endl;
}

void MQTTProxy::MQTTServer::onClose(const muduo::net::TcpConnectionPtr& conn)
{
}
