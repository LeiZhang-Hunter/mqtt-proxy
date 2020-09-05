//
// Created by zhanglei on 2020/6/1.
//
#include "autoload.h"
#include "MQTTContainerGlobal.h"
#include "MQTTServer.h"

MQTTContainerGlobal MQTTContainer;


int main(int argc, char **argv) {
    //解析命令行的配置选项 为了解析出配置文件的路径
    int opt;
    while ((opt = getopt(argc, argv, "c")) != -1) {
        switch (opt) {
            case 'c':

                break;
        }
    }
    std::string path;
    if (argv[optind]) {
        path = (argv[optind]);
    } else {
        std::cout << "please input config path" << std::endl;
        exit(-1);
    }

    //初始化mqtt全局容器
    ::signal(SIGPIPE, SIG_IGN);
    MQTTContainer.globalInit();
    MQTTContainer.Config.setPath(path);
    MQTTContainer.Config.loadConfig();
    std::string proxy_ip;
    std::string proxy_port;
    proxy_ip = MQTTContainer.Config.getConfig("proxy_ip");
    if (proxy_ip.empty()) {
        std::cerr << "proxy ip must not be empty" << std::endl;
        exit(-1);
    }
    proxy_port = MQTTContainer.Config.getConfig("proxy_port");
    if (proxy_port.empty()) {
        std::cerr << "proxy port must not be empty" << std::endl;
        exit(-1);
    }
    //LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(proxy_ip, atoi(proxy_port.c_str()));
    MQTTProxy::MQTTServer server(&loop, listenAddr);
    server.start();
    loop.loop();
    return 0;
}