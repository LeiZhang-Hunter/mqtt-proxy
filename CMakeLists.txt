PROJECT(device-server)

cmake_minimum_required(VERSION 3.4.1)

SET(MUDUO_DIR ../muduo-master/)
link_directories("../build/release-cpp11/lib")
message(STATUS ${MUDUO_LIB_DIR})
#指定我自己的头文件包含目录
INCLUDE_DIRECTORIES(
        "${PROJECT_SOURCE_DIR}/include/"
        "${PROJECT_SOURCE_DIR}/include/lib/"
        "${PROJECT_SOURCE_DIR}/include/controller"
        "${PROJECT_SOURCE_DIR}/include/system"
        ${PROJECT_SOURCE_DIR}
        ${MUDUO_DIR}
)

file(GLOB_RECURSE LIB_SRC ${PROJECT_SOURCE_DIR}/lib/*.cc)

add_library(device-server-lib STATIC ${LIB_SRC})

set(SOURCE_SRC
    main.cc
    include/autoload.h
    src/MQTTServer.cc
    src/MQTTSessionHandle.cc
    src/MQTTProxyClient.cc
    src/MQTTConnPool.cc
    )

add_executable(device-server ${SOURCE_SRC})

#定义连接文件
target_link_libraries(device-server  muduo_net muduo_base pthread device-server-lib)