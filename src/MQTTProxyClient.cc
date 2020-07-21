//
// Created by zhanglei on 2020/7/16.
//

#include "autoload.h"

void MQTTProxy::MQTTProxyClient::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
        muduo::Timestamp receiveTime)
{
    //解析返回的协议数据如果说连接成功则给下端发送连接确认
    buf->retrieveAll();
}

void MQTTProxy::MQTTProxyClient::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
    Conn = conn;
}

void MQTTProxy::MQTTProxyClient::onClose(const muduo::net::TcpConnectionPtr &conn)
{

    if (!conn->connected()) {
        std::cout<<"close"<<std::endl;

    }
}

void MQTTProxy::MQTTProxyClient::sendProxyData(const MQTTProxy::MQTTProxyProtocol& protocol)
{
    //导入协议类型

    //压缩封装协议到设备中台去查询设备是否可以接入
    std::vector<uint8_t> buffer;
    buffer.push_back(protocol.ProtocolType);
    //消息类型
    buffer.push_back(protocol.MessageType);
    //消息的错误码
    buffer.push_back(protocol.MessageNo);
    //客户端id的长度
    std::vector<__uint8_t > encodeRemainingLength = MQTTContainer.Util.encodeRemainingLength(protocol.ClientIdLength);
    buffer.insert(buffer.end(), encodeRemainingLength.begin(), encodeRemainingLength.end());
    //客户端id的内容
    buffer.insert(buffer.end(), protocol.ClientId.begin(), protocol.ClientId.end());
    //载荷的长度
    std::vector<__uint8_t > message_len = MQTTContainer.Util.encodeRemainingLength(protocol.MessageLength);
    buffer.insert(buffer.end(), message_len.begin(), message_len.end());
    //载荷的内容
    buffer.insert(buffer.end(), protocol.Payload.begin(), protocol.Payload.end());
    //CRC校验的具体信息
    uint16_t crc16 =  MQTTContainer.Util.checkCRC16(buffer.data(), buffer.size());
    buffer.push_back(MSB(crc16));
    buffer.push_back(LSB(crc16));
    //发送buffer
    Conn->send(buffer.data(), buffer.size());
}