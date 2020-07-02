//
// Created by zhanglei on 2020/7/2.
//

#include "autoload.h"

DeviceSever::MQTTConnPool::MQTTConnPool()
:mutex_()
{

}

bool DeviceSever::MQTTConnPool::registerConn(const muduo::net::TcpConnectionPtr& conn,
        std::shared_ptr<DeviceSeverLib::MQTT>& data)
{
    muduo::MutexLockGuard guard(mutex_);
    MQTTConnectPool[conn] = data;
    return true;
}

DeviceSever::MQTTConnPool::MQTTConnMapValue DeviceSever::MQTTConnPool::getConnMQTTInfo(const muduo::net::TcpConnectionPtr& conn)
{
    muduo::MutexLockGuard guard(mutex_);
    MQTTConnMap::iterator dataIndex = MQTTConnectPool.find(conn);

    if(dataIndex == MQTTConnectPool.end())
    {
        return nullptr;
    }

    return MQTTConnectPool[conn];
}

bool DeviceSever::MQTTConnPool::deleteConn(const muduo::net::TcpConnectionPtr &conn)
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