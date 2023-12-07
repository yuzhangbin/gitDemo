#ifndef __WORKER_H__
#define __WORKER_H__

class Worker
{
    public:
        int id;
        int eachNum;
        void operator()();
};

#endif