//
// Created by zhanglei on 2020/6/1.
//
#include "autoload.h"
MQTTContainerGlobal MQTTContainer;


int main(int argc,char** argv)
{
    //解析命令行的配置选项 为了解析出配置文件的路径
    int opt;
    while ((opt = getopt(argc, argv, "c"))!= -1)
    {
        switch (opt)
        {
            case 'c':

                break;
        }
    }
    std::string path;
    if(argv[optind])
    {
        path = (argv[optind]);
    }else{
        std::cout<<"please input config path"<<std::endl;
        exit(-1);
    }

    MQTTProxy::MQTTProxyConfig config;
    config.setPath(path);
    config.loadConfig();
    //初始化mqtt全局容器
    ::signal(SIGPIPE, SIG_IGN);
    MQTTContainer.globalInit();
    //LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(9500);
    MQTTProxy::MQTTServer server(&loop, listenAddr);
    server.start();
    loop.loop();
    return 0;
}