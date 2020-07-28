//
// Created by zhanglei on 2020/7/28.
//

#include "autoload.h"

DeviceServer::MQTTProxyHandle::MQTTProxyHandle()
{

}

bool DeviceServer::MQTTProxyHandle::OnConnectMessage(const std::shared_ptr<MQTTProxy::MQTTProxyProtocol>& protocol)
{
    if(protocol->MessageNo == 0)
        return true;

    return false;
}

bool DeviceServer::MQTTProxyHandle::OnDisConnectMessage()
{

}

bool DeviceServer::MQTTProxyHandle::OnSubscribeMessage(const std::shared_ptr<MQTTProxy::MQTTProxyProtocol>& protocol)
{
    if(protocol->MessageNo == 0)
        return true;



    return false;
}

bool DeviceServer::MQTTProxyHandle::OnUnSubscribeMessage()
{

}

bool DeviceServer::MQTTProxyHandle::OnPublishMessage()
{

}

bool DeviceServer::MQTTProxyHandle::OnPublish()
{

}

DeviceServer::MQTTProxyHandle::~MQTTProxyHandle()
{

}