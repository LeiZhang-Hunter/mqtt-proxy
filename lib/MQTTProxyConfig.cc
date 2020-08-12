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

    int fileFd = open(ConfigPath.c_str(), O_RDWR);

    if (!fileFd) {
        std::cout<<"open log failed:"<<__FILE__<<";line:"<<__LINE__<<std::endl;
        exit(-1);
    }


    std::string content;
    char buf[BUFSIZ];

    read(fileFd, buf, BUFSIZ);
}