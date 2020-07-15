//
// Created by zhanglei on 2020/7/1.
//

#include "autoload.h"

std::vector<uint8_t> DeviceServerLib::Util::encodeRemainingLength(uint32_t remainingLength)
{
    int i=0;
    uint8_t digit;
    std::vector<uint8_t > value;

    do{
        digit = remainingLength % 128;
        remainingLength = remainingLength/128;

        if(remainingLength > 0)
        {
            digit = digit | 0x80;
        }

        //两个字节最多4位了
        if(i >= 4)
        {
            return std::vector<uint8_t >();
        }

        value.push_back(digit);
        i++;
    }while (remainingLength > 0);

    return value;
}