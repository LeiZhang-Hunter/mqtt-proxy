//
// Created by zhanglei on 2020/9/4.
//

#ifndef MQTT_PROXY_TIMINGCELL_H
#define MQTT_PROXY_TIMINGCELL_H
namespace MQTTProxy {
    class TimingCell {
    public:
        typedef std::shared_ptr<TimingCell> TimeCellPtr;
        typedef std::weak_ptr<TimingCell> WeakTimeCellPtr;
        typedef std::weak_ptr<muduo::net::TcpConnection> WeakTcpConnectionPtr;

        TimingCell(const WeakTcpConnectionPtr& weakConn)
                : weakConn_(weakConn) {
        }

        void setMQTTProxyType(uint8_t param) {
            type = param;
        }

        ~TimingCell() {
            muduo::net::TcpConnectionPtr conn = weakConn_.lock();
            if (conn) {
                conn->shutdown();
            }
        }
        uint8_t type;
        WeakTcpConnectionPtr weakConn_;
    };

}

#endif //MQTT_PROXY_TIMINGCELL_H
