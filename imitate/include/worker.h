#ifndef __WORKER_H__
#define __WORKER_H__

#include "sunnet.h"

class Worker
{
    public:
        int id;
        int eachNum;
        void operator()();
        void checkAndPutGlobal(shared_ptr<Service> service);
};

#endif