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
    size_t len = topic.topic.length();
    size_t delim_len = delimiter.length();
    std::string search_result;
    TopicTreeMapType::iterator iterator;
    TopicTreeMapType &tree = SubscribeTree;

    int find_pos = topic.topic.find(delimiter, pos);

    search_result = topic.topic.substr(pos, len - pos);
    iterator = SubscribeTree.find(search_result);
    if(iterator == SubscribeTree.end())
    {
        SubscribeTree[search_result] = std::make_shared<DeviceServer::MQTTSubscribeTreeNode>();
        SubscribeTree[search_result]->topic = topic.topic;

        //只有一级 绑定成功就结束了
        if(find_pos < 0)
        {
            SubscribeTree[search_result]->sessionMap[session->getClientId()] = session;
            return true;
        }
    }

    //只有一级 绑定成功就结束了
    if(find_pos < 0)
    {
        std::shared_ptr<DeviceServer::MQTTSubscribeTreeNode> node = SubscribeTree[search_result];

        auto session_iterator = node->sessionMap.find(session->getClientId());
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

        if (find_pos < 0)
        {

            iterator = tree.find(search_result);
            if(iterator != tree.end())
            {
                tree[search_result] = std::make_shared<DeviceServer::MQTTSubscribeTreeNode>();
            }
            return true;
        }
        pos = find_pos + delim_len;
    }
    return true;
}