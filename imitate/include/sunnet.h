#ifndef __SUNNET_H__
#define __SUNNET_H__

#include <vector>
#include <thread>
#include <unordered_map>
#include "worker.h"
#include "service.h"
#include "serviceMsg.h"
#include "socketWorker.h"
#include "conn.h"

using namespace std;

class Worker;
class SockerWorker;

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
        SocketWorker *socketWorker;
        thread *socketWorkerThread;
    private:
        Sunnet();
        ~Sunnet();
        void startWorker();
        void startSocket();
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
    private:
        pthread_mutex_t sleepMtx;
        pthread_cond_t sleepCond;
        int sleepCount = 0; // 记录休眠工作线程数量
    public:
        void checkAndWeakUp();
        void workerWait();
    public:
        int addConn(int fd, uint32_t id, Conn::TYPE type);
        shared_ptr<Conn> getConn(int fd);
        bool removeConn(int fd);
    private:
        unordered_map<uint32_t, shared_ptr<Conn>> conns;
        pthread_rwlock_t connsLock;
};

#endif