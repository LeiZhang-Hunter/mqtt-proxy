//
// Created by zhanglei on 2020/7/12.
//

#ifndef DEVICE_SERVER_MQTTCLIENTSESSIONPOOL_H
#define DEVICE_SERVER_MQTTCLIENTSESSIONPOOL_H

namespace DeviceServer
{

class MQTTClientSessionPool : public muduo::noncopyable
{
public:
    MQTTClientSessionPool();

    typedef std::map<std::string, std::shared_ptr<DeviceServer::MQTTClientSession>> ClientIdMapType;

    std::shared_ptr<DeviceServer::MQTTClientSession> bindSession(const std::string& client_id,
            const muduo::net::TcpConnectionPtr& conn);

    std::shared_ptr<DeviceServer::MQTTClientSession> findSession(const std::string& client_id);

    ~MQTTClientSessionPool();
private:

    muduo::MutexLock Lock_;
    ClientIdMapType ClientIdMap;
    //std::map<>

};

}

#endif //DEVICE_SERVER_MQTTCLIENTSESSIONPOOL_H
