#ifndef __SOCKETWORKER_H__
#define __SOCKETWORKER_H__

#include "socket_kqueue.h"

class SocketWorker
{
    public:
        int id;
        void init();
        void operator()();
    private:
        poll_fd epollFd;
};

#endif