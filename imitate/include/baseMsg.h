#ifndef __BASEMSG_H__
#define __BASEMSG_H__

#include <stdint.h>

using namespace std;

class BaseMsg
{
    public:
        enum TYPE {
            SERVICE = 1,
            SOCKET_ACCEPT = 2,
            SOCKET_RW = 3,
        };
        uint8_t type;
        char load[999999]; // 用于检测内存泄漏
        virtual ~BaseMsg(){}
};

class SocketAcceptMsg : public BaseMsg
{
    public:
        int listenFd; // 监听
        int clientFd; // 新连接
};

class SocketRWMsg : public BaseMsg
{
    public:
        int fd;
        int isWrite = false;
        int isRead = false;
};

#endif