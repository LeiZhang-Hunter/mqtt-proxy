//
// Created by zhanglei on 2020/7/12.
//

#include "autoload.h"

DeviceServer::MQTTClientSessionPool::MQTTClientSessionPool()
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

    //初始化回话信息
    ClientIdMapType::iterator session = ClientIdMap.find(client_id);
    //找到了这个客户端id
    if(session != ClientIdMap.end())
    {
        //强制关闭
        ClientIdMap[client_id]->getConn()->forceClose();
        ClientIdMap[client_id]->setConn(conn);
    }else{
        ClientIdMap[client_id] = std::make_shared<DeviceServer::MQTTClientSession>();
    }
    ClientIdMap[client_id]->setClientId(client_id);
    return ClientIdMap[client_id];
}

DeviceServer::MQTTClientSessionPool::~MQTTClientSessionPool()
{

}