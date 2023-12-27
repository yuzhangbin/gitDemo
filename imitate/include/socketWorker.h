#ifndef __SOCKETWORKER_H__
#define __SOCKETWORKER_H__

#include <iostream>

#include "socket_epoll.h"

using namespace std;

class SocketWorker
{
    public:
        int id;
        void init();
        void operator()();
    private:
        shared_ptr<epoller> efd;
    private:
        void addEvent(int fd);
};

#endif