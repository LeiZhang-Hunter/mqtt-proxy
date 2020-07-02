//
// Created by zhanglei on 2020/6/1.
//
#include "autoload.h"
MQTTContainerGlobal MQTTContainer;
int main()
{
    printf("%ld\n",sizeof(int));
    //初始化mqtt全局容器
    MQTTContainer.globalInit();
    //LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(9500);
    DeviceSever::MQTTServer server(&loop, listenAddr);
    server.start();
    loop.loop();
    return 0;
}