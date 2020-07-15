//
// Created by zhanglei on 2020/7/14.
//
#include "autoload.h"

void DeviceServer::MQTTSessionHandle::onConnect(const DeviceServer::Callback::MQTTClientSessionPtr& session)
{
    std::cout<<"connect"<<std::endl;
}

void DeviceServer::MQTTSessionHandle::onMessage(const DeviceServer::Callback::MQTTClientSessionPtr& session, muduo::net::Buffer *buf,muduo::Timestamp time)
{
    std::cout<<"message"<<std::endl;
}

void DeviceServer::MQTTSessionHandle::onClose(const DeviceServer::Callback::MQTTClientSessionPtr& session)
{
    std::cout<<"close"<<std::endl;
}