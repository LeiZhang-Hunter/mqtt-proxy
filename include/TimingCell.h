//
// Created by zhanglei on 2020/9/4.
//

#ifndef MQTT_PROXY_TIMINGCELL_H
#define MQTT_PROXY_TIMINGCELL_H
namespace MQTTProxy {
    class TimingCell;

    typedef std::shared_ptr<TimingCell> TimeCellPtr;
    typedef std::weak_ptr<TimingCell> WeakTimeCellPtr;

    class TimingCell {
        typedef std::weak_ptr<muduo::net::TcpConnection> WeakTcpConnectionPtr;

    public:
        explicit TimingCell(WeakTcpConnectionPtr &weakConn)
                : weakConn_(weakConn) {
        }

        ~TimingCell() {
            muduo::net::TcpConnectionPtr conn = weakConn_.lock();
            if (conn) {
                conn->shutdown();
            }
        }

        WeakTcpConnectionPtr weakConn_;
    };

}

#endif //MQTT_PROXY_TIMINGCELL_H
