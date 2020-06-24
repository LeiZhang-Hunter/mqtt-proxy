//
// Created by zhanglei on 2020/6/23.
//

#ifndef DEVICE_SERVER_AUTOLOAD_H
#define DEVICE_SERVER_AUTOLOAD_H

//c++
#include <string>
#include <functional>
#include <iostream>

//muduo
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Logging.h>

//business
#include "MQTT.h"
#include "MQTTServer.h"

#endif //DEVICE_SERVER_AUTOLOAD_H
