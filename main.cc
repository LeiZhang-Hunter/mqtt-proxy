//
// Created by zhanglei on 2020/6/1.
//
#include "autoload.h"

int main()
{
    printf("%ld\n",sizeof(int));
    //LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(9500);
    DeviceSever::MQTTServer server(&loop, listenAddr);
    server.start();
    loop.loop();
    return 0;
}