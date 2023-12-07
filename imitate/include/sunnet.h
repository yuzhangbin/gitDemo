#ifndef __SUNNET_H__
#define __SUNNET_H__

#include <vector>
#include <thread>
#include "worker.h"

using namespace std;

class Sunnet
{
    public:
        static Sunnet * instance();
        void start();
        void wait();
    private:
        static Sunnet *inst;
        int WORKER_NUM = 3;
        vector<Worker *> workers;
        vector<thread *> workerThreads;
    private:
        Sunnet();
        void startWorker();
};

#endif