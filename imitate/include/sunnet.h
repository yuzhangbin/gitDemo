#ifndef __SUNNET_H__
#define __SUNNET_H__

#include <vector>
#include <thread>
#include <unordered_map>
#include "worker.h"
#include "service.h"
#include "serviceMsg.h"

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
        ~Sunnet();
        void startWorker();
    public:
        unordered_map<uint32_t, shared_ptr<Service>> services;
        uint32_t maxServiceID = 0;
        pthread_rwlock_t servicesLock; //读写锁
    public:
        uint32_t newService(shared_ptr<string> type);
        void killService(uint32_t serviceID);
    private:
        shared_ptr<Service> getService(uint32_t serviceID);
    private:
        queue<shared_ptr<Service>> globalQueue;
        int globalLen = 0;
        struct spinlock globalSpinLock;
    public:
        void send(uint32_t serviceID, shared_ptr<BaseMsg> msg);
        shared_ptr<Service> popGlobalQueue();
        void pushGlobalQueue(shared_ptr<Service> svr);
    public:
        shared_ptr<BaseMsg> makeMsg(uint32_t source, char *buff, int len); // 创建消息，这个仅做测试使用
};

#endif