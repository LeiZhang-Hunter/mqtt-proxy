//
// Created by zhanglei on 2020/7/1.
//

#ifndef DEVICE_SERVER_UTIL_H
#define DEVICE_SERVER_UTIL_H
#include "json/json.h"
#include "MQTTProtocol.h"

namespace MQTTProxyLib {

class Util : public muduo::noncopyable{
public:

    //字节编码
    std::vector<uint8_t> encodeRemainingLength(uint32_t remainingLength);

    std::pair<uint32_t ,uint32_t> decodeRemainingLength(const char* byte);

    //CRC16的校验
    uint16_t checkCRC16(uint8_t * pMsg ,uint16_t u16_DataLen);

    //json解压
    bool jsonDecode(const std::string& strJsonMess,Json::Value* root);

    //json编码
    Json::String jsonEncode(Json::Value proto_value);
private:
};
}
#endif //DEVICE_SERVER_UTIL_H
