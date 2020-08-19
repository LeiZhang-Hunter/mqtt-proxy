//
// Created by zhanglei on 2020/6/23.
//

#ifndef DEVICE_SERVER_AUTOLOAD_H
#define DEVICE_SERVER_AUTOLOAD_H

//c
#include <fcntl.h>

//c++
#include <csignal>
#include <string>
#include <functional>
#include <iostream>

//muduo
#include <muduo/base/Singleton.h>
#include <muduo/base/noncopyable.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Logging.h>
#include <muduo/net/TcpClient.h>
#include <muduo/base/ThreadLocalSingleton.h>

//json-cpp
#include <json/json.h>

//business
#include "MQTTProxyConfig.h"
#include "Util.h"
#include "MQTTType.h"
#include "MQTTMessage.h"
#include "MQTTSessionCallback.h"
#include "ProxyProtocol.h"
#include "MQTTSubscribe.h"
#include "MQTTResponse.h"
#include "MQTTProtocol.h"
#include "MQTTClientSession.h"
#include "MQTTProxyHandle.h"
#include "MQTTClientSessionPool.h"
#include "MQTTTopicTree.h"
#include "MQTTConnPool.h"
#include "MQTTContainerGlobal.h"
#include "MQTTSessionHandle.h"
#include "MQTTProxyClient.h"
#include "MQTTServer.h"

#endif //DEVICE_SERVER_AUTOLOAD_H
