//
// Created by zhanglei on 2020/7/12.
//

#include "autoload.h"

bool DeviceServer::MQTTTopicTree::addSubscribe(const DeviceServer::MQTTSubscribe& topic,
        const DeviceServer::Callback::MQTTClientSessionPtr& session)
{
    std::vector<std::string> topic_key;
    std::string delimiter("/");
    size_t pos = 0;
    std::string subscribe_topic = topic.topic;
    int find_pos = subscribe_topic.find(delimiter, pos);
    size_t len = subscribe_topic.length();
    size_t delim_len = delimiter.length();
    std::string subscribe_key;
    TopicTreeMapType::iterator tree_iterator;
    //会话的迭代器
    std::map<std::string, std::shared_ptr<DeviceServer::MQTTClientSession>>::iterator session_iterator;
    //根节点
    TopicTreeMapType &tree = SubscribeTree;
    //订阅树节点
    std::shared_ptr<DeviceServer::MQTTSubscribeTreeNode> node;
    //订阅树节点的迭代器
    std::map<std::string, std::shared_ptr<DeviceServer::MQTTSubscribeTreeNode>>::iterator node_iterator;

    subscribe_key = subscribe_topic.substr(pos, find_pos - pos);
    tree_iterator = SubscribeTree.find(subscribe_key);
    //没有发现节点
    if(tree_iterator == SubscribeTree.end())
    {
        SubscribeTree[subscribe_key] = std::make_shared<DeviceServer::MQTTSubscribeTreeNode>();
        SubscribeTree[subscribe_key]->topic = subscribe_key;

        //只有一级 绑定成功就结束了
        if(find_pos < 0)
        {
            SubscribeTree[subscribe_key]->sessionMap[session->getClientId()] = session;
            return true;
        }
    }

    node = SubscribeTree[subscribe_key];

    //只有一级 绑定成功就结束了
    if(find_pos < 0)
    {
        session_iterator = node->sessionMap.find(session->getClientId());
        if(session_iterator == node->sessionMap.end())
        {
            node->sessionMap[session->getClientId()] = session;
        }
        return true;
    }

    pos = find_pos + delim_len;

    //多个等级的处理
    while (pos < len)
    {
        find_pos = subscribe_topic.find(delimiter, pos);

        if (find_pos < 0)
        {
            subscribe_key = subscribe_topic.substr(pos, len - pos);
            node_iterator = node->sonSubscribe.find(subscribe_key);
            if(node_iterator == node->sonSubscribe.end())
            {
                node->sonSubscribe[subscribe_key] = std::make_shared<DeviceServer::MQTTSubscribeTreeNode>();
                node->sonSubscribe[subscribe_key]->topic = subscribe_key;

                //查找订阅树下面的会话是否存在
                session_iterator = node->sessionMap.find(session->getClientId());
                if(session_iterator == node->sessionMap.end())
                {
                    node->sonSubscribe[subscribe_key]->sessionMap[session->getClientId()] = session;
                }
            }
            return true;
        }
        subscribe_key = subscribe_topic.substr(pos, find_pos - pos);
        node_iterator = node->sonSubscribe.find(subscribe_key);
        //找不到节点则初始化节点
        if(node_iterator == node->sonSubscribe.end())
        {
            node->sonSubscribe[subscribe_key] = std::make_shared<MQTTSubscribeTreeNode>();
            node->sonSubscribe[subscribe_key]->topic = subscribe_key;
        }
        node = node->sonSubscribe[subscribe_key];
        //查找树节点是否存在如果不存在的话则初始化树节点
        pos = find_pos + delim_len;
    }
    return true;
}