//
// Created by zhanglei on 2020/7/2.
//

#ifndef DEVICE_SERVER_MQTTCONNPOOL_H
#define DEVICE_SERVER_MQTTCONNPOOL_H

namespace DeviceServer{

class MQTTConnPool : public muduo::noncopyable
{

public:
    explicit MQTTConnPool();

    typedef muduo::net::TcpConnectionPtr MQTTConnMapKey;
    typedef std::shared_ptr<DeviceServerLib::MQTTProtocol> MQTTConnMapValue;
    typedef std::map<MQTTConnMapKey, MQTTConnMapValue> MQTTConnMap;

    //注册连接
    bool registerConn(const muduo::net::TcpConnectionPtr &conn, std::shared_ptr<DeviceServerLib::MQTTProtocol>&);

    //获取连接信息
    MQTTConnMapValue getConnMQTTInfo(const muduo::net::TcpConnectionPtr& conn);

    //删除连接
    bool deleteConn(const muduo::net::TcpConnectionPtr &conn);

    ~MQTTConnPool() = default;

private:
    muduo::MutexLock mutex_;
    MQTTConnMap MQTTConnectPool GUARDED_BY(mutex_);
};
}

#endif //DEVICE_SERVER_MQTTCONNPOOL_H
