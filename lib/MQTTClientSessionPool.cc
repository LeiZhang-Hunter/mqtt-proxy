//
// Created by zhanglei on 2020/7/12.
//

#include "autoload.h"

DeviceSever::MQTTClientSessionPool::MQTTClientSessionPool()
{

}

std::shared_ptr<DeviceServer::MQTTClientSession> DeviceSever::MQTTClientSessionPool::bindSession(const std::string& client_id,
        const muduo::net::TcpConnectionPtr& conn) {
    sessionMapType::iterator session = clientMap.find(client_id);
    if(session != clientMap.end())
    {
        clientMap[client_id] = std::make_shared<DeviceServer::MQTTClientSession>();
    }
    return clientMap[client_id];
}

DeviceSever::MQTTClientSessionPool::~MQTTClientSessionPool()
{

}