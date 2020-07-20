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
            SubscribeTree[subscribe_key]->SessionMap[session->getClientId()] = session;
            return true;
        }
    }

    node = SubscribeTree[subscribe_key];

    //只有一级 绑定成功就结束了
    if(find_pos < 0)
    {
        session_iterator = node->SessionMap.find(session->getClientId());
        if(session_iterator == node->SessionMap.end())
        {
            node->SessionMap[session->getClientId()] = session;
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
            node_iterator = node->SonSubscribe.find(subscribe_key);
            if(node_iterator == node->SonSubscribe.end())
            {
                node->SonSubscribe[subscribe_key] = std::make_shared<DeviceServer::MQTTSubscribeTreeNode>();
                node->SonSubscribe[subscribe_key]->topic = subscribe_key;
            }

            //查找订阅树下面的会话是否存在
            session_iterator = node->SessionMap.find(session->getClientId());
            if(session_iterator == node->SessionMap.end())
            {
                node->SonSubscribe[subscribe_key]->SessionMap[session->getClientId()] = session;
            }else{
                //之前已经订阅过了不能重复的订阅
                return false;
            }
            return true;
        }
        subscribe_key = subscribe_topic.substr(pos, find_pos - pos);
        node_iterator = node->SonSubscribe.find(subscribe_key);
        //找不到节点则初始化节点
        if(node_iterator == node->SonSubscribe.end())
        {
            node->SonSubscribe[subscribe_key] = std::make_shared<MQTTSubscribeTreeNode>();
            node->SonSubscribe[subscribe_key]->topic = subscribe_key;
        }
        node = node->SonSubscribe[subscribe_key];
        //查找树节点是否存在如果不存在的话则初始化树节点
        pos = find_pos + delim_len;
    }
    return false;
}

//从订阅树中取消这个会话
bool DeviceServer::MQTTTopicTree::unSubscribe(const DeviceServer::MQTTSubscribe& topic,
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
    //没有发现节点,则说明之前没有订阅过 直接返回false就可以了
    if(tree_iterator == SubscribeTree.end())
    {
        return false;
    }

    node = SubscribeTree[subscribe_key];

    //只有一级 绑定成功就结束了
    if(find_pos < 0)
    {
        session_iterator = node->SessionMap.find(session->getClientId());
        if(session_iterator == node->SessionMap.end())
        {
            return false;
        }
        //从订阅树中直接清除掉会话
        node->SessionMap.erase(session_iterator);
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
            node_iterator = node->SonSubscribe.find(subscribe_key);
            //之前没有订阅过 直接返回false
            if(node_iterator == node->SonSubscribe.end())
            {
                return false;
            }

            //查找订阅树下面的会话是否存在
            session_iterator = node->SessionMap.find(session->getClientId());
            if(session_iterator == node->SessionMap.end())
            {
                return  false;
            }
            node->SessionMap.erase(session_iterator);
            return true;
        }
        subscribe_key = subscribe_topic.substr(pos, find_pos - pos);
        node_iterator = node->SonSubscribe.find(subscribe_key);
        //找不到节点则初始化节点
        if(node_iterator == node->SonSubscribe.end())
        {
            return false;
        }
        node = node->SonSubscribe[subscribe_key];
        //查找树节点是否存在如果不存在的话则初始化树节点
        pos = find_pos + delim_len;
    }
    return true;
}

//发现订阅池下面的所有会话
DeviceServer::MQTTTopicTree::SubscribeNode DeviceServer::MQTTTopicTree::findSubscribe(const DeviceServer::MQTTSubscribe& topic)
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
    std::shared_ptr<DeviceServer::MQTTSubscribeTreeNode> result;

    subscribe_key = subscribe_topic.substr(pos, find_pos - pos);
    tree_iterator = SubscribeTree.find(subscribe_key);
    //没有发现节点
    if(tree_iterator == SubscribeTree.end())
    {
        return SubscribeNode();
    }

    node = SubscribeTree[subscribe_key];

    //只有一级 绑定成功就结束了
    if(find_pos < 0)
    {
        return node;
    }

    pos = find_pos + delim_len;

    //多个等级的处理
    while (pos < len)
    {
        find_pos = subscribe_topic.find(delimiter, pos);

        if (find_pos < 0)
        {
            subscribe_key = subscribe_topic.substr(pos, len - pos);
            node_iterator = node->SonSubscribe.find(subscribe_key);
            if(node_iterator == node->SonSubscribe.end())
            {
                return SubscribeNode();
            }

            return node->SonSubscribe[subscribe_key];
        }
        subscribe_key = subscribe_topic.substr(pos, find_pos - pos);
        node_iterator = node->SonSubscribe.find(subscribe_key);
        //找不到节点则初始化节点
        if(node_iterator == node->SonSubscribe.end())
        {
           return SubscribeNode();
        }
        node = node->SonSubscribe[subscribe_key];
        //查找树节点是否存在如果不存在的话则初始化树节点
        pos = find_pos + delim_len;
    }
    return SubscribeNode();
}