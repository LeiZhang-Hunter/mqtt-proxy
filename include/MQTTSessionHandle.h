//
// Created by zhanglei on 2020/7/14.
//

#ifndef DEVICE_SERVER_MQTTSESSIONHANDLE_H
#define DEVICE_SERVER_MQTTSESSIONHANDLE_H

#include "MQTTSessionCallback.h"
#include "MQTTResponse.h"
#include "MQTTSubscribe.h"

namespace MQTTProxy {

/**
 * 我想了一下其实我们的客户端只需要处理的是 CONNECT DISCONNECT PUBLISH SUBSCRIBE UNSUBSCRIBE这些mqtt事件,无需关注底层的
 * 其他事件
 */
    class MQTTSessionHandle : public muduo::noncopyable {
    public:
        //开启会话触发的事件
        bool OnConnect(const Callback::MQTTClientSessionPtr &session);

        //订阅事件
        bool OnSubscribe(const Callback::MQTTClientSessionPtr &session, const MQTTProxy::MQTTSubscribe &subscribe);

        //取消订阅事件
        bool OnUnSubscribe(const Callback::MQTTClientSessionPtr &session, const MQTTProxy::MQTTSubscribe &subscribe);

        //收到相关主题的推送
        void OnPublish(const MQTTProxy::Callback::MQTTClientSessionPtr &session,
                       const MQTTProxy::MQTTSubscribe &subscribe,
                       const std::string &message);

        //关闭会话触发的事件
        bool OnDisConnect(const MQTTProxy::Callback::MQTTClientSessionPtr &session);

    private:
        //响应处理器用来处理mqtt的响应,现阶段是用来测试我认为这个响应器应该是放在代理节点的
        MQTTProxyLib::MQTTResponse response;
    };
}
#endif //DEVICE_SERVER_MQTTSESSIONHANDLE_H
