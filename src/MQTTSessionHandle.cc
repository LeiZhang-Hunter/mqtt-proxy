//
// Created by zhanglei on 2020/7/14.
//
#include "autoload.h"
//连接建立
bool DeviceServer::MQTTSessionHandle::OnConnect(const DeviceServer::Callback::MQTTClientSessionPtr&  session)
{
    std::cout<<"connect"<<std::endl;
}

//订阅事件
bool DeviceServer::MQTTSessionHandle::OnSubscribe(const DeviceServer::Callback::MQTTClientSessionPtr&  session,
        const std::string& topic, muduo::Timestamp timestamp)
{

}

//取消订阅事件
bool DeviceServer::MQTTSessionHandle::OnUnSubscribe(const DeviceServer::Callback::MQTTClientSessionPtr&  session,
        const std::string& topic, muduo::Timestamp timestamp)
{

}

//收到相关主题的推送
void DeviceServer::MQTTSessionHandle::OnPublish(const DeviceServer::Callback::MQTTClientSessionPtr&  session,
        const std::string& topic, const std::string& message, muduo::Timestamp timestamp
)
{

}

//关闭会话触发的事件
bool DeviceServer::MQTTSessionHandle::OnDisConnect(const DeviceServer::Callback::MQTTClientSessionPtr&  session)
{

}