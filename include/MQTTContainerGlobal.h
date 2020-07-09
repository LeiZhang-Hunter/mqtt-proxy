//
// Created by zhanglei on 2020/7/2.
//

#ifndef DEVICE_SERVER_MQTTCONTAINERGLOBAL_H
#define DEVICE_SERVER_MQTTCONTAINERGLOBAL_H

typedef struct _MQTTContainerGlobal : public muduo::noncopyable
{
public:
    _MQTTContainerGlobal() :mutex()
    {

    }

    bool globalInit()
    {
        pool = std::make_shared<DeviceSever::MQTTConnPool>();
        return true;
    }

    muduo::MutexLock mutex;
    DeviceSeverLib::Util util;
    std::shared_ptr<DeviceSever::MQTTConnPool> pool GUARDED_BY(mutex);

    ~_MQTTContainerGlobal() = default;
}MQTTContainerGlobal;

extern MQTTContainerGlobal MQTTContainer;

#endif //DEVICE_SERVER_MQTTCONTAINERGLOBAL_H
