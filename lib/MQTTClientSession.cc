//
// Created by zhanglei on 2020/7/12.
//
#include "autoload.h"

using namespace std::placeholders;

bool DeviceServer::MQTTClientSession::startSession()
{
    //设置接收消息的事件
    Conn->setMessageCallback(std::bind(&MQTTClientSession::SessionOnMessage, shared_from_this(), _1, _2, _3));
    //设置关闭的回调事件
    Conn->setCloseCallback(std::bind(&MQTTClientSession::SessionOnClose, shared_from_this(), _1));
}