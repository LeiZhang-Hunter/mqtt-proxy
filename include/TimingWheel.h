//
// Created by zhanglei on 2020/9/4.
//

#ifndef MQTT_PROXY_TIMINGWHEEL_H
#define MQTT_PROXY_TIMINGWHEEL_H

#include "TimingCell.h"

namespace MQTTProxy {
    class TimingWheel {

    public:

        typedef std::unordered_set<TimingCell::TimeCellPtr> Bucket;
        typedef boost::circular_buffer<Bucket> WeakConnectionList;

        void onTimer() {
            timingWheelConnectionBucket.push_back(Bucket());
        }

        void resizeWheelingSize(int size) {
            timingWheelConnectionBucket.resize(size);
        }

        void bindTimeCell(const TimingCell::TimeCellPtr& cell) {
            timingWheelConnectionBucket.back().insert(cell);
        }

    private:
        WeakConnectionList timingWheelConnectionBucket;
    };
}

#endif //MQTT_PROXY_TIMINGWHEEL_H
