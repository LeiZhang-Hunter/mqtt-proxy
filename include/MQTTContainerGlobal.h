//
// Created by zhanglei on 2020/7/2.
//

#ifndef DEVICE_SERVER_MQTTCONTAINERGLOBAL_H
#define DEVICE_SERVER_MQTTCONTAINERGLOBAL_H

#include "MQTTProxyConfig.h"
#include "Util.h"
#include "MQTTClientSessionPool.h"
#include "MQTTTopicTree.h"

namespace MQTTProxy
{
    class MQTTProxyClient;
}

typedef struct _MQTTContainerGlobal : public muduo::noncopyable
{
public:
    _MQTTContainerGlobal() :Mutex()
    {

    }

    bool globalInit()
    {
        SessionPool = std::make_shared<MQTTProxy::MQTTClientSessionPool>();
        TopicTree = std::make_shared<MQTTProxy::MQTTTopicTree>();
        return true;
    }

    std::shared_ptr<MQTTProxy::MQTTProxyClient> getProxyClient()
    {
        return ProxyMap[muduo::CurrentThread::tid()];
    }

    muduo::MutexLock Mutex;
    MQTTProxyLib::Util Util;
    MQTTProxy::MQTTProxyConfig Config;
    std::shared_ptr<MQTTProxy::MQTTClientSessionPool> SessionPool;
    std::shared_ptr<MQTTProxy::MQTTTopicTree> TopicTree;
    std::map<int, std::shared_ptr<MQTTProxy::MQTTProxyClient>> ProxyMap;
    std::shared_ptr<muduo::CountDownLatch> finished;
    ~_MQTTContainerGlobal() = default;
}MQTTContainerGlobal;

extern MQTTContainerGlobal MQTTContainer;

#endif //DEVICE_SERVER_MQTTCONTAINERGLOBAL_H
