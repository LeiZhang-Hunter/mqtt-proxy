//
// Created by zhanglei on 2020/8/10.
//

#ifndef MQTT_PROXY_MQTTPROXYCONFIG_H
#define MQTT_PROXY_MQTTPROXYCONFIG_H

namespace MQTTProxy
{

class MQTTProxyConfig
{

public:
    bool setPath(const std::string& path)
    {
        ConfigPath = path;
        return true;
    }

    bool loadConfig();

private:
    std::string ConfigPath;
};
}

#endif //MQTT_PROXY_MQTTPROXYCONFIG_H
