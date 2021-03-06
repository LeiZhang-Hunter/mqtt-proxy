//
// Created by zhanglei on 2020/7/2.
//

#include "autoload.h"
#include "MQTTConnPool.h"

MQTTProxy::MQTTConnPool::MQTTConnPool()
        : mutex_() {

}

bool MQTTProxy::MQTTConnPool::registerConn(const muduo::net::TcpConnectionPtr &conn,
                                           std::shared_ptr<MQTTProxyLib::MQTTProtocol> &data) {
    muduo::MutexLockGuard guard(mutex_);
    MQTTConnectPool[conn] = data;
    return true;
}

MQTTProxy::MQTTConnPool::MQTTConnMapValue
MQTTProxy::MQTTConnPool::getConnMQTTInfo(const muduo::net::TcpConnectionPtr &conn) {
    muduo::MutexLockGuard guard(mutex_);
    MQTTConnMap::iterator dataIndex = MQTTConnectPool.find(conn);

    if (dataIndex == MQTTConnectPool.end()) {
        return nullptr;
    }

    return MQTTConnectPool[conn];
}

bool MQTTProxy::MQTTConnPool::deleteConn(const muduo::net::TcpConnectionPtr &conn) {
    muduo::MutexLockGuard guard(mutex_);
    MQTTConnMap::iterator dataIndex = MQTTConnectPool.find(conn);
    if (dataIndex == MQTTConnectPool.end()) {
        return false;
    }

    MQTTConnectPool.erase(dataIndex);
    return true;
}