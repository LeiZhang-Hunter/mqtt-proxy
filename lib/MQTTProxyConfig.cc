//
// Created by zhanglei on 2020/8/10.
//

#include "autoload.h"

bool MQTTProxy::MQTTProxyConfig::loadConfig()
{
    //检查配置路径是否是空的
    if(ConfigPath.empty())
    {
        std::cout<<"config path is empty"<<std::endl;
        exit(-1);
    }


}