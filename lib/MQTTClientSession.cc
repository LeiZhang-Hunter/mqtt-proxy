//
// Created by zhanglei on 2020/7/12.
//

#include "autoload.h"
#include <TimingCell.h>
#include <TimingWheel.h>
#include <ProxyProtocol.h>
#include "MQTTClientSession.h"
#include "MQTTContainerGlobal.h"
#include "MQTTType.h"

using namespace std::placeholders;

bool MQTTProxy::MQTTClientSession::startSession() {
    std::weak_ptr<muduo::net::TcpConnection> weak_conn;
    if (weak_conn.expired()) {
        //设置协议处理器的回调处理函数
        OnConnect();
        protocol->setDisConnectCallback(std::bind(&MQTTClientSession::OnDisConnect, shared_from_this()));
        protocol->setSubscribeCallback(std::bind(&MQTTClientSession::OnSubscribe, shared_from_this(), _1));
        protocol->setUnSubscribeCallback(std::bind(&MQTTClientSession::OnUnSubscribe, shared_from_this(), _1));
        protocol->setPublishCallback(std::bind(&MQTTClientSession::OnPublish, shared_from_this(), _1, _2));
        //设置接收消息的事件
        Conn->setMessageCallback(std::bind(&MQTTClientSession::SessionOnMessage, shared_from_this(), _1, _2, _3));
        //设置关闭的回调事件
        Conn->setConnectionCallback(std::bind(&MQTTClientSession::SessionOnClose, shared_from_this(), _1));
        //标记为上线
        IsOnline = Online;
        TimingCell::TimeCellPtr cell(new TimingCell(Conn));
        cell->setMQTTProxyType(MQTT_PROXY);
        muduo::ThreadLocalSingleton<TimingWheel>::instance().bindTimeCell(cell);
        TimingCell::WeakTimeCellPtr weakTimeCell(cell);
        Conn->setContext(weakTimeCell);
        return true;
    } else {
        LOG_WARN << getClientId() << "Conn(" << getClientId() << ") has been destroy";
        return false;
    }
}

//收到消息的时候触发
void MQTTProxy::MQTTClientSession::SessionOnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
                                                    muduo::Timestamp) {
    TimingCell::WeakTimeCellPtr weakCell(boost::any_cast<TimingCell::WeakTimeCellPtr>(Conn->getContext()));

    TimingCell::TimeCellPtr cell(weakCell.lock());
    if (cell) {
        muduo::ThreadLocalSingleton<TimingWheel>::instance().bindTimeCell(cell);
    }
    protocol->parse(buf, conn);
}

//关闭连接的时候触发
void MQTTProxy::MQTTClientSession::SessionOnClose(const muduo::net::TcpConnectionPtr &conn) {
    Conn = nullptr;
    IsOnline = Offline;
}

/*=======================为了将协议处理结果传入到业务层，再次封装了一个托底的回调==============================*/
void MQTTProxy::MQTTClientSession::OnConnect() {
    OnConnectCallback(shared_from_this());
}

void MQTTProxy::MQTTClientSession::OnDisConnect() {
    OnConnectCallback(shared_from_this());
}

void MQTTProxy::MQTTClientSession::OnSubscribe(const MQTTProxy::MQTTSubscribe &subscribe) {
    //加入订阅树
    OnSubscribeCallback(shared_from_this(), subscribe);
}

void MQTTProxy::MQTTClientSession::OnUnSubscribe(const MQTTProxy::MQTTSubscribe &subscribe) {
    OnUnSubscribeCallback(shared_from_this(), subscribe);
}

void MQTTProxy::MQTTClientSession::OnPublish(const MQTTProxy::MQTTSubscribe &subscribe,
                                             const std::string &message) {
    OnPublishCallback(shared_from_this(), subscribe, message);
}
/*=======================为了将协议处理结果传入到业务层，再次封装了一个托底的回调==============================*/

//给会话推送消息
bool MQTTProxy::MQTTClientSession::publish(const MQTTMessage &message) {
    //建立一个发送缓冲
    std::vector<uint8_t> buffer;
    MessageId++;
    buffer.push_back(MQTT_PUBLISH | ((message.Dup & 0x1) << 3) | (message.QosLevel << 1) | message.Retain);
    //.
    uint32_t remain_len = 2 + message.topic.size() + message.Payload.length();
    if (message.QosLevel > 0) {
        remain_len += 2;
    }
    std::vector<uint8_t> remaingBytes = MQTTContainer.Util.encodeRemainingLength(remain_len);
    buffer.insert(buffer.end(), remaingBytes.begin(), remaingBytes.end());
    buffer.push_back(MSB(message.topic.size()));
    buffer.push_back(LSB(message.topic.size()));
    buffer.insert(buffer.end(), message.topic.begin(), message.topic.end());
    if (message.QosLevel > 0) {
        buffer.push_back(MSB(MessageId));
        buffer.push_back(LSB(MessageId));
    }
    buffer.insert(buffer.end(), message.Payload.begin(), message.Payload.end());
//    buffer.push_back(MQTT_PUBLISH | );
    if (Conn) {
        Conn->send(buffer.data(), buffer.size());
    }
    return true;
}
/**/