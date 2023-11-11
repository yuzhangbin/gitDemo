#ifndef __LIST_TIMER_H__
#define __LIST_TIMER_H__
#include <netinet/in.h>

#define BUFFERSIZE 64

class util_timer;

struct client_data
{
    // 用户数据结构
    sockaddr_in address;
    int sock_fd;
    char buffer[BUFFERSIZE];
    util_timer *timer; // 前向声明
};

class util_timer
{   // 定时器类
    public:
        util_timer():prev(nullptr), next(nullptr){}
    public:
        void (*cb_func)(client_data *);
        time_t expire;
        client_data *data;
        util_timer *prev;
        util_timer *next;
};

class listTimer
{
    public:

    private:
        util_timer *head;
        util_timer *tail;
};

#endif