//
// Created by zhanglei on 2020/7/12.
//

#ifndef DEVICE_SERVER_MQTTTOPIC_H
#define DEVICE_SERVER_MQTTTOPIC_H

namespace DeviceServer
{

//订阅树上的叶子节点
class MQTTSubscribeTreeNode
{

public:
    std::string topic;
    std::map<std::string, std::shared_ptr<DeviceServer::MQTTClientSession>> SessionMap;
    std::map<std::string, std::shared_ptr<DeviceServer::MQTTSubscribeTreeNode>> SonSubscribe;
};
//订阅树
class MQTTTopicTree
{

public:
    typedef std::map<std::string, std::shared_ptr<MQTTSubscribeTreeNode>> TopicTreeMapType;
    typedef std::shared_ptr<DeviceServer::MQTTSubscribeTreeNode> SubscribeNode;

    //加入订阅树
    bool addSubscribe(const DeviceServer::MQTTSubscribe& topic,
            const DeviceServer::Callback::MQTTClientSessionPtr& session);

    //从订阅树中取消
    bool unSubscribe(const DeviceServer::MQTTSubscribe& topic,
                      const DeviceServer::Callback::MQTTClientSessionPtr& session);


    SubscribeNode findSubscribe(const DeviceServer::MQTTSubscribe& topic);

private:
    //订阅树
    TopicTreeMapType SubscribeTree;
};
}

#endif //DEVICE_SERVER_MQTTTOPIC_H
