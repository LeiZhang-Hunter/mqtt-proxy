//
// Created by zhanglei on 2020/7/1.
//

#ifndef DEVICE_SERVER_UTIL_H
#define DEVICE_SERVER_UTIL_H
namespace DeviceServerLib {

class Util{
public:
    std::vector<uint8_t> encodeRemainingLength(uint32_t remainingLength);
};
}
#endif //DEVICE_SERVER_UTIL_H
