PROJECT(device-server)

cmake_minimum_required(VERSION 3.4.1)

SET(MUDUO_BASE_INCLUDE /home/zhanglei/ourc/muduo-master/muduo/base)
SET(MUDUO_NET_INCLUDE /home/zhanglei/ourc/muduo-master/muduo/net)
SET(MUDUO_LIBRARY_DIR /home/zhanglei/ourc/build/release-cpp11/lib)

link_directories(${MUDUO_LIBRARY_DIR})

#指定我自己的头文件包含目录
INCLUDE_DIRECTORIES(
        "include/controller"
        "include/system"
        ${PROJECT_SOURCE_DIR}
        ${MUDUO_NET_INCLUDE}
        ${MUDUO_NET_INCLUDE}
)


add_executable(device-server "main.cc")

#定义连接文件
target_link_libraries(device-server)