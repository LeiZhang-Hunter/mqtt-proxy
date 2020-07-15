//
// Created by zhanglei on 2020/7/12.
//

#include "autoload.h"

DeviceServer::MQTTClientSessionPool::MQTTClientSessionPool()
:Lock_()
{

}

std::shared_ptr<DeviceServer::MQTTClientSession> DeviceServer::MQTTClientSessionPool::bindSession(const std::string& client_id,
        const muduo::net::TcpConnectionPtr& conn) {
    if(client_id.empty())
    {
        return nullptr;
    }

    //上锁保护
    muduo::MutexLockGuard guard(Lock_);

    //初始化会话信息
    ClientIdMapType::iterator session = ClientIdMap.find(client_id);
    //找到了这个客户端id
    if(session != ClientIdMap.end())
    {
        //强制关闭
        ClientIdMap[client_id]->getConn()->forceClose();
    }else{
        std::shared_ptr<DeviceServer::MQTTClientSession> new_session(new DeviceServer::MQTTClientSession());
        ClientIdMap[client_id] = new_session;
    }
    ClientIdMap[client_id]->setConn(conn);
    ClientIdMap[client_id]->setClientId(client_id);
    return ClientIdMap[client_id];
}

DeviceServer::MQTTClientSessionPool::~MQTTClientSessionPool()
{

}