//
// Created by zhanglei on 2020/7/28.
//

#include "autoload.h"

MQTTProxy::MQTTProxyHandle::MQTTProxyHandle()
{

}

bool MQTTProxy::MQTTProxyHandle::OnConnectMessage(const std::shared_ptr<MQTTProxy::MQTTProxyProtocol>& protocol)
{
    if(protocol->MessageNo == 0)
        return true;

    return false;
}

bool MQTTProxy::MQTTProxyHandle::OnDisConnectMessage()
{
    return true;
}

bool MQTTProxy::MQTTProxyHandle::OnSubscribeMessage(const std::shared_ptr<MQTTProxy::MQTTProxyProtocol>& protocol)
{
    if(protocol->MessageNo == 0)
        return true;



    return false;
}

bool MQTTProxy::MQTTProxyHandle::OnUnSubscribeMessage()
{
    return true;
}

bool MQTTProxy::MQTTProxyHandle::OnPublishMessage(const std::shared_ptr<MQTTProxy::MQTTProxyProtocol>& protocol)
{
    if(protocol->MessageNo == 0)
    {
        return true;
    }

    return false;
}

bool MQTTProxy::MQTTProxyHandle::OnPublish()
{
    return true;
}

MQTTProxy::MQTTProxyHandle::~MQTTProxyHandle()
{

}