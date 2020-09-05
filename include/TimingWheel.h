//
// Created by zhanglei on 2020/9/4.
//

#ifndef MQTT_PROXY_TIMINGWHEEL_H
#define MQTT_PROXY_TIMINGWHEEL_H

#include "TimingCell.h"

namespace MQTTProxy {
    class TimingWheel {

    public:

        typedef std::unordered_set<TimeCellPtr> Bucket;
        typedef boost::circular_buffer<Bucket> WeakConnectionList;

        void onTimer() {
            timingWheelConnectionBucket.push_back(Bucket());
        }

        void resizeWheelingSize(int size) {
            timingWheelConnectionBucket.resize(size);
        }

        void bindTimeCell(const muduo::net::TcpConnectionPtr &conn) {
//            TimeCellPtr cell(new TimingCell(conn.lock()));
//            timingWheelConnectionBucket.back().insert(cell);
//            //绑定一个mqtt处理器
//            WeakTimeCellPtr weakTimeCell(cell);
//            conn.setContext(weakTimeCell);
        }

    private:
        WeakConnectionList timingWheelConnectionBucket;
    };
}

#endif //MQTT_PROXY_TIMINGWHEEL_H
