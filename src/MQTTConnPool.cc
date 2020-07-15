//
// Created by zhanglei on 2020/7/2.
//

#include "autoload.h"

DeviceServer::MQTTConnPool::MQTTConnPool()
:mutex_()
{

}

bool DeviceServer::MQTTConnPool::registerConn(const muduo::net::TcpConnectionPtr& conn,
        std::shared_ptr<DeviceServerLib::MQTTProtocol>& data)
{
    muduo::MutexLockGuard guard(mutex_);
    MQTTConnectPool[conn] = data;
    return true;
}

DeviceServer::MQTTConnPool::MQTTConnMapValue DeviceServer::MQTTConnPool::getConnMQTTInfo(const muduo::net::TcpConnectionPtr& conn)
{
    muduo::MutexLockGuard guard(mutex_);
    MQTTConnMap::iterator dataIndex = MQTTConnectPool.find(conn);

    if(dataIndex == MQTTConnectPool.end())
    {
        return nullptr;
    }

    return MQTTConnectPool[conn];
}

bool DeviceServer::MQTTConnPool::deleteConn(const muduo::net::TcpConnectionPtr &conn)
{
    muduo::MutexLockGuard guard(mutex_);
    MQTTConnMap::iterator dataIndex = MQTTConnectPool.find(conn);
    if(dataIndex == MQTTConnectPool.end())
    {
        return false;
    }

    MQTTConnectPool.erase(dataIndex);
    return true;
}