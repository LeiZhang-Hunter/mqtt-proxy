//
// Created by zhanglei on 2020/7/2.
//

#ifndef DEVICE_SERVER_MQTTCONTAINERGLOBAL_H
#define DEVICE_SERVER_MQTTCONTAINERGLOBAL_H

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
        SessionPool = std::make_shared<DeviceServer::MQTTClientSessionPool>();
        return true;
    }

    std::shared_ptr<MQTTProxy::MQTTProxyClient> getProxyClient()
    {
        return ProxyMap[muduo::CurrentThread::tid()];
    }

    muduo::MutexLock Mutex;
    DeviceServerLib::Util Util;
    std::shared_ptr<DeviceServer::MQTTClientSessionPool> SessionPool;
    std::map<int, std::shared_ptr<MQTTProxy::MQTTProxyClient>> ProxyMap;
    ~_MQTTContainerGlobal() = default;
}MQTTContainerGlobal;

extern MQTTContainerGlobal MQTTContainer;

#endif //DEVICE_SERVER_MQTTCONTAINERGLOBAL_H
