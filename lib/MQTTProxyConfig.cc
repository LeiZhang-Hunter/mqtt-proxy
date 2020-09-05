//
// Created by zhanglei on 2020/8/10.
//

#include "autoload.h"
#include "MQTTProxyConfig.h"
#include "MQTTContainerGlobal.h"

bool MQTTProxy::MQTTProxyConfig::loadConfig() {
    //检查配置路径是否是空的
    if (ConfigPath.empty()) {
        std::cerr << "config path is empty" << std::endl;
        exit(-1);
    }

    int fileFd = open(ConfigPath.c_str(), O_RDWR);

    if (!fileFd) {
        std::cerr << "open log failed:" << __FILE__ << ";line:" << __LINE__ << std::endl;
        exit(-1);
    }

    //讲描述符设置为非阻塞的
    int flags = fcntl(fileFd, F_GETFL, 0);

    fcntl(fileFd, F_SETFL, flags | O_NONBLOCK);

    std::string content;
    char buf[BUFSIZ];

    while (1) {
        size_t byte = read(fileFd, buf, BUFSIZ);
        buf[byte] = '\0';
        if (byte <= 0) {
            if (errno == EINTR) {
                continue;
            } else {
                break;
            }
        }
        content.append(buf);
    }

    //对于获取到的json串进行解包
    if (!MQTTContainer.Util.jsonDecode(content, &configValue)) {
        std::cerr << "Config Json Parse failed;" << __FILE__ << ";line:" << __LINE__ << std::endl;
        exit(-1);
    }

    if (!configValue) {
        std::cerr << "Config (configValue) must not be nullptr;" << __FILE__ << ";line:" << __LINE__ << std::endl;
        exit(-1);
    }
    close(fileFd);
    return true;
}

//获取配置
std::string MQTTProxy::MQTTProxyConfig::getConfig(const std::string &key) {
    if (!configValue.isMember(key)) {
        return "";
    } else {
        return configValue[key.c_str()].asString();
    }
}