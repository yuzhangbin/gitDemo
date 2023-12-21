#ifndef __WORKER_H__
#define __WORKER_H__

#include "sunnet.h"
#include "service.h"

class Service;

class Worker
{
    public:
        int id;
        int eachNum;
        void operator()();
        void checkAndPutGlobal(shared_ptr<Service> service);
};

#endif