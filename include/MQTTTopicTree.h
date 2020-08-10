//
// Created by zhanglei on 2020/7/12.
//

#ifndef DEVICE_SERVER_MQTTTOPIC_H
#define DEVICE_SERVER_MQTTTOPIC_H

namespace MQTTProxy
{

//订阅树上的叶子节点
class MQTTSubscribeTreeNode
{

public:
    std::string topic;
    std::map<std::string, std::shared_ptr<MQTTProxy::MQTTClientSession>> SessionMap;
    std::map<std::string, std::shared_ptr<MQTTProxy::MQTTSubscribeTreeNode>> SonSubscribe;
};
//订阅树
class MQTTTopicTree : public muduo::noncopyable
{

public:
    MQTTTopicTree():lock()
    {

    }
    typedef std::map<std::string, std::shared_ptr<MQTTSubscribeTreeNode>> TopicTreeMapType;
    typedef std::shared_ptr<MQTTProxy::MQTTSubscribeTreeNode> SubscribeNode;

    //加入订阅树
    bool addSubscribe(const MQTTProxy::MQTTSubscribe& topic,
            const MQTTProxy::Callback::MQTTClientSessionPtr& session);

    //从订阅树中取消
    bool unSubscribe(const MQTTProxy::MQTTSubscribe& topic,
                      const MQTTProxy::Callback::MQTTClientSessionPtr& session);


    SubscribeNode findSubscribe(const MQTTProxy::MQTTSubscribe& topic);

    void publish(const MQTTProxy::MQTTSubscribe &topic, const std::string& message);

private:
    muduo::MutexLock lock;
    //订阅树
    TopicTreeMapType SubscribeTree;
};
}

#endif //DEVICE_SERVER_MQTTTOPIC_H
