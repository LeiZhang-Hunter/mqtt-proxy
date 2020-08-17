//
// Created by zhanglei on 2020/8/10.
//

#ifndef MQTT_PROXY_MQTTPROXYCONFIG_H
#define MQTT_PROXY_MQTTPROXYCONFIG_H

namespace MQTTProxy
{

class MQTTProxyConfig : public muduo::noncopyable
{

public:
    bool setPath(const std::string& path)
    {
        ConfigPath = path;
        return true;
    }

    bool loadConfig();

    std::string getConfig(const std::string& key);

private:
    std::string ConfigPath;
    Json::Value configValue;
};
}

#endif //MQTT_PROXY_MQTTPROXYCONFIG_H
