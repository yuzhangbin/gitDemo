#ifndef __SOCKETWORKER_H__
#define __SOCKETWORKER_H__

class SocketWorker
{
    public:
        int id;
        void init();
        void operator()();
};

#endif